// ADVANCED ROBOTICS_CV: ROS2-Style Node/Topic Pattern in Pure C++
//
// HONEST FRAMING:
// Real ROS2 requires a full installation (Ubuntu preferred, Windows supported
// but complex). This is NOT a real ROS2 program.
//
// This simulates the rclcpp API design pattern in plain C++17 so you understand:
// - The Node abstraction (named, isolated processing unit)
// - Topics (typed channels): publishers and subscribers
// - The publish/subscribe message-passing model
// - Callback-based reactive programming
// - Timer-driven publishing
//
// After studying this, real ROS2 code will look familiar.
// The actual rclcpp API looks nearly identical.
//
// HOW REAL ROS2 WORKS:
//   rclcpp::Node     -> this Node class
//   rclcpp::Publisher -> this Publisher<T>
//   rclcpp::Subscription -> this Subscriber<T>
//   rclcpp::TimerBase -> this Timer
//   rclcpp::spin()   -> this Executor::spin()
//   std_msgs::msg::Float64 -> float (simplified here)

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <queue>
#include <chrono>
#include <thread>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
// Message types (in real ROS2 these are auto-generated from .msg files)
// ============================================================
struct Float64Msg    { double data; };
struct Vector3Msg    { double x, y, z; };
struct TwistMsg      { Vector3Msg linear, angular; };  // robot velocity command
struct OdometryMsg   { Vector3Msg position; Vector3Msg velocity; double heading; };

// ============================================================
// Topic bus — the "DDS middleware" simulation
// In real ROS2, DDS (Data Distribution Service) routes messages between nodes.
// Here we use a simple in-process map of topic name -> message queue.
// ============================================================
class TopicBus {
    struct TopicEntry {
        std::vector<std::function<void(const void*)>> subscribers;
    };
    std::map<std::string, TopicEntry> topics;

public:
    void subscribe(const std::string& topic, std::function<void(const void*)> cb) {
        topics[topic].subscribers.push_back(std::move(cb));
    }

    void publish(const std::string& topic, const void* msg) {
        auto it = topics.find(topic);
        if (it == topics.end()) return;
        for (auto& cb : it->second.subscribers) cb(msg);
    }

    int subscriber_count(const std::string& topic) const {
        auto it = topics.find(topic);
        return it != topics.end() ? static_cast<int>(it->second.subscribers.size()) : 0;
    }
};

// Singleton bus (simulating the DDS layer)
static TopicBus g_bus;

// ============================================================
// Publisher<T> — typed topic publisher
// ============================================================
template<typename T>
class Publisher {
    std::string topic;
    std::string node_name;
    int pub_count{0};

public:
    Publisher(const std::string& node, const std::string& topic)
        : topic(topic), node_name(node) {}

    void publish(const T& msg) {
        g_bus.publish(topic, static_cast<const void*>(&msg));
        ++pub_count;
    }

    int count() const { return pub_count; }
    const std::string& get_topic() const { return topic; }
};

// ============================================================
// Subscriber<T> — typed topic subscriber
// ============================================================
template<typename T>
class Subscriber {
    std::string topic;
    int msg_count{0};

public:
    Subscriber(const std::string& topic, std::function<void(const T&)> callback)
        : topic(topic)
    {
        g_bus.subscribe(topic, [callback, this](const void* raw) {
            callback(*static_cast<const T*>(raw));
            ++msg_count;
        });
    }

    int count() const { return msg_count; }
};

// ============================================================
// Timer — periodic callback
// ============================================================
class Timer {
    std::function<void()> callback;
    double period_ms;
    double accum_ms{0};

public:
    Timer(double period_ms, std::function<void()> cb)
        : callback(std::move(cb)), period_ms(period_ms) {}

    void tick(double dt_ms) {
        accum_ms += dt_ms;
        while (accum_ms >= period_ms) {
            callback();
            accum_ms -= period_ms;
        }
    }
};

// ============================================================
// Node base class
// ============================================================
class Node {
    std::string name_;
    std::vector<std::shared_ptr<Timer>> timers_;

protected:
    template<typename T>
    std::shared_ptr<Publisher<T>> create_publisher(const std::string& topic) {
        return std::make_shared<Publisher<T>>(name_, topic);
    }

    template<typename T>
    std::shared_ptr<Subscriber<T>> create_subscription(
            const std::string& topic, std::function<void(const T&)> cb) {
        return std::make_shared<Subscriber<T>>(topic, std::move(cb));
    }

    std::shared_ptr<Timer> create_timer(double period_ms, std::function<void()> cb) {
        auto t = std::make_shared<Timer>(period_ms, std::move(cb));
        timers_.push_back(t);
        return t;
    }

    void log(const std::string& msg) const {
        std::cout << "[" << name_ << "] " << msg << "\n";
    }

public:
    explicit Node(const std::string& name) : name_(name) {}
    virtual ~Node() {}
    const std::string& get_name() const { return name_; }

    void tick_timers(double dt_ms) {
        for (auto& t : timers_) t->tick(dt_ms);
    }
};

// ============================================================
// Example nodes
// ============================================================

// SensorNode: publishes IMU-like data on /imu topic
class SensorNode : public Node {
    std::shared_ptr<Publisher<Vector3Msg>> imu_pub;
    std::shared_ptr<Publisher<Float64Msg>> battery_pub;
    double time{0};
    int tick_count{0};

public:
    SensorNode() : Node("sensor_node") {
        imu_pub     = create_publisher<Vector3Msg>("/imu");
        battery_pub = create_publisher<Float64Msg>("/battery");

        // Publish IMU at 50Hz
        create_timer(20.0, [this]{ publish_imu(); });
        // Publish battery at 1Hz
        create_timer(1000.0, [this]{ publish_battery(); });

        log("Started — publishing /imu @ 50Hz, /battery @ 1Hz");
    }

private:
    void publish_imu() {
        time += 0.02;
        // Simulate gentle rotation + vibration
        Vector3Msg gyro;
        gyro.x = 0.05 * std::sin(time);
        gyro.y = 0.02 * std::cos(time * 2);
        gyro.z = 0.01;
        imu_pub->publish(gyro);
        ++tick_count;
    }

    void publish_battery() {
        Float64Msg batt;
        batt.data = 12.6 - 0.001 * tick_count;  // slowly draining
        battery_pub->publish(batt);
    }
};

// LocalizationNode: subscribes to /imu, publishes /odom (dead reckoning)
class LocalizationNode : public Node {
    std::shared_ptr<Subscriber<Vector3Msg>> imu_sub;
    std::shared_ptr<Publisher<OdometryMsg>> odom_pub;
    OdometryMsg state{};

public:
    LocalizationNode() : Node("localization_node") {
        odom_pub = create_publisher<OdometryMsg>("/odom");

        imu_sub = create_subscription<Vector3Msg>("/imu", [this](const Vector3Msg& msg) {
            // Dead reckoning: integrate angular velocity to get heading
            const double DT = 0.02;
            state.heading      += msg.z * DT;
            state.velocity.x    = 0.5 * std::cos(state.heading);
            state.velocity.y    = 0.5 * std::sin(state.heading);
            state.position.x   += state.velocity.x * DT;
            state.position.y   += state.velocity.y * DT;
            odom_pub->publish(state);
        });

        log("Started — subscribing /imu, publishing /odom");
    }

    OdometryMsg current_state() const { return state; }
};

// NavigationNode: subscribes to /odom, publishes /cmd_vel (velocity commands)
class NavigationNode : public Node {
    std::shared_ptr<Subscriber<OdometryMsg>> odom_sub;
    std::shared_ptr<Publisher<TwistMsg>> cmd_vel_pub;

    double target_x{2.0}, target_y{1.5};
    TwistMsg last_cmd{};

public:
    NavigationNode() : Node("navigation_node") {
        cmd_vel_pub = create_publisher<TwistMsg>("/cmd_vel");

        odom_sub = create_subscription<OdometryMsg>("/odom", [this](const OdometryMsg& odom) {
            double dx = target_x - odom.position.x;
            double dy = target_y - odom.position.y;
            double dist = std::sqrt(dx*dx + dy*dy);

            TwistMsg cmd{};
            if (dist > 0.05) {
                // Simple proportional controller: drive toward goal
                double desired_heading = std::atan2(dy, dx);
                double heading_err = desired_heading - odom.heading;
                // Normalize heading error to [-pi, pi]
                while (heading_err >  M_PI) heading_err -= 2*M_PI;
                while (heading_err < -M_PI) heading_err += 2*M_PI;

                cmd.linear.x  = std::min(0.5, dist * 0.5);
                cmd.angular.z = heading_err * 2.0;
            }
            last_cmd = cmd;
            cmd_vel_pub->publish(cmd);
        });

        log("Started — navigating to (" + std::to_string(target_x) + ", "
            + std::to_string(target_y) + ")");
    }
};

// MonitorNode: subscribes to /battery and /odom, prints status
class MonitorNode : public Node {
    std::shared_ptr<Subscriber<Float64Msg>>  battery_sub;
    std::shared_ptr<Subscriber<OdometryMsg>> odom_sub;
    int report_count{0};

public:
    MonitorNode() : Node("monitor_node") {
        battery_sub = create_subscription<Float64Msg>("/battery", [this](const Float64Msg& msg) {
            if (msg.data < 11.0) {
                log("WARNING: Low battery! " + std::to_string(msg.data) + "V");
            }
        });

        odom_sub = create_subscription<OdometryMsg>("/odom", [this](const OdometryMsg& odom) {
            if (++report_count % 50 == 0) {  // report every 50 messages (~1s)
                log("Position: (" +
                    std::to_string(odom.position.x).substr(0,5) + ", " +
                    std::to_string(odom.position.y).substr(0,5) + ")  heading=" +
                    std::to_string(odom.heading).substr(0,5));
            }
        });

        log("Started — monitoring /battery and /odom");
    }
};

// ============================================================
// Executor — runs all nodes (in real ROS2 this is rclcpp::spin)
// ============================================================
class Executor {
    std::vector<std::shared_ptr<Node>> nodes;

public:
    void add_node(std::shared_ptr<Node> node) {
        nodes.push_back(std::move(node));
    }

    void spin(double duration_ms, double step_ms = 10.0) {
        std::cout << "\n[Executor] Running " << nodes.size()
                  << " nodes for " << duration_ms << "ms...\n\n";
        for (double t = 0; t < duration_ms; t += step_ms) {
            for (auto& n : nodes) n->tick_timers(step_ms);
        }
        std::cout << "\n[Executor] Spin complete.\n";
    }
};

int main() {
    std::cout << "=== ROS2-Style Node/Topic Pattern in C++ ===\n";
    std::cout << "This simulates rclcpp's publish/subscribe architecture.\n";
    std::cout << "Real ROS2 uses DDS middleware; here we use an in-process bus.\n\n";

    // Create nodes (same as: rclcpp::Node::make_shared<...>())
    auto sensor   = std::make_shared<SensorNode>();
    auto localiz  = std::make_shared<LocalizationNode>();
    auto nav      = std::make_shared<NavigationNode>();
    auto monitor  = std::make_shared<MonitorNode>();

    // Show topic graph
    std::cout << "\n--- Topic Graph ---\n";
    std::cout << "sensor_node   -> /imu -> localization_node\n";
    std::cout << "sensor_node   -> /battery -> monitor_node\n";
    std::cout << "localization  -> /odom -> navigation_node, monitor_node\n";
    std::cout << "navigation    -> /cmd_vel -> (robot actuators)\n\n";

    // Run for 3 seconds of simulated time
    Executor exec;
    exec.add_node(sensor);
    exec.add_node(localiz);
    exec.add_node(nav);
    exec.add_node(monitor);
    exec.spin(3000.0, 10.0);

    // Final state
    OdometryMsg final_state = localiz->current_state();
    std::cout << "\n--- Final State ---\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Position: (" << final_state.position.x << ", "
                               << final_state.position.y << ")\n";
    std::cout << "Heading:  " << final_state.heading << " rad\n";

    std::cout << "\n=== Mapping to Real rclcpp ===\n";
    std::cout << "This simulation       -> Real ROS2\n";
    std::cout << "  Node                -> rclcpp::Node\n";
    std::cout << "  Publisher<T>        -> rclcpp::Publisher<T>\n";
    std::cout << "  Subscriber<T>       -> rclcpp::Subscription<T>\n";
    std::cout << "  create_timer()      -> create_wall_timer()\n";
    std::cout << "  Executor::spin()    -> rclcpp::spin(node)\n";
    std::cout << "  TopicBus (in-proc)  -> DDS (network, multi-process)\n";
    std::cout << "\nReal install: https://docs.ros.org/en/humble/Installation/Windows.html\n";

    return 0;
}
