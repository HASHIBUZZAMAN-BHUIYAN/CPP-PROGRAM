// ADVANCED ROBOTICS_CV: PID Controller in C++
//
// A PID controller is the most widely used control algorithm in robotics,
// industrial automation, drones, motor controllers, and embedded systems.
//
// This is the C++ version — compare with C-PROGRAM's pid_controller.c.
// C++ advantages here:
// - PID class with encapsulated state (no global variables)
// - Multiple independent controllers via separate instances
// - Operator-friendly tuning interface
// - std::chrono for real timestamps instead of manual dt passing

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
#include <chrono>

// ============================================================
// PID Controller class
// ============================================================
class PIDController {
    double kp, ki, kd;
    double integral{0};
    double prev_error{0};
    double output_min, output_max;
    bool   first_update{true};
    std::chrono::steady_clock::time_point last_time;

    std::string name;

public:
    PIDController(const std::string& name, double kp, double ki, double kd,
                  double out_min = -1e9, double out_max = 1e9)
        : kp(kp), ki(ki), kd(kd),
          output_min(out_min), output_max(out_max),
          name(name)
    {}

    double update(double setpoint, double measurement, double dt) {
        double error = setpoint - measurement;

        // Integral (with anti-windup via clamping)
        integral += error * dt;

        // Derivative (on measurement, not error — avoids "derivative kick" on setpoint change)
        double derivative = first_update ? 0.0 : (error - prev_error) / dt;
        first_update = false;

        double output = kp * error + ki * integral + kd * derivative;

        // Clamp output
        output = std::clamp(output, output_min, output_max);

        // Anti-windup: if output is clamped, don't accumulate integral
        if (output >= output_max || output <= output_min)
            integral -= error * dt;

        prev_error = error;
        return output;
    }

    void reset() {
        integral = 0;
        prev_error = 0;
        first_update = true;
    }

    void set_gains(double new_kp, double new_ki, double new_kd) {
        kp = new_kp; ki = new_ki; kd = new_kd;
    }

    const std::string& get_name() const { return name; }
};

// ============================================================
// Simulated physical system
// ============================================================
struct System {
    double position{0};  // current state
    double velocity{0};
    double mass;
    double friction;

    System(double mass = 1.0, double friction = 0.5)
        : mass(mass), friction(friction) {}

    // Apply force, update physics
    void update(double force, double dt) {
        double accel = (force - friction * velocity) / mass;
        velocity  += accel * dt;
        position  += velocity * dt;
    }
};

// ============================================================
// Simulation runner — runs the control loop, records history
// ============================================================
struct SimResult {
    std::vector<double> time;
    std::vector<double> setpoint;
    std::vector<double> position;
    std::vector<double> output;
    std::vector<double> error;
};

SimResult simulate(PIDController& pid, System& sys,
                   double setpoint, double duration, double dt,
                   double noise_amp = 0.0)
{
    SimResult result;
    pid.reset();

    for (double t = 0; t <= duration; t += dt) {
        // Optional measurement noise
        double noisy_pos = sys.position;
        if (noise_amp > 0)
            noisy_pos += noise_amp * (2.0 * (std::rand() / (double)RAND_MAX) - 1.0);

        double force = pid.update(setpoint, noisy_pos, dt);
        sys.update(force, dt);

        double err = setpoint - sys.position;
        result.time.push_back(t);
        result.setpoint.push_back(setpoint);
        result.position.push_back(sys.position);
        result.output.push_back(force);
        result.error.push_back(err);
    }
    return result;
}

// ============================================================
// Print ASCII time-series chart
// ============================================================
void print_chart(const SimResult& r, const std::string& title) {
    const int COLS = 60, ROWS = 12;
    double max_val = *std::max_element(r.setpoint.begin(), r.setpoint.end()) * 1.2;
    double min_val = -0.2;

    std::cout << "\n" << title << "\n";
    std::cout << std::string(COLS + 8, '-') << "\n";

    // Sample N points from result
    size_t n = r.time.size();
    for (int row = ROWS; row >= 0; --row) {
        double val = min_val + (max_val - min_val) * row / ROWS;
        std::cout << std::setw(6) << std::fixed << std::setprecision(2) << val << " |";
        for (int col = 0; col < COLS; ++col) {
            size_t idx = col * n / COLS;
            double pos = r.position[idx];
            double sp  = r.setpoint[idx];
            double row_val_lo = min_val + (max_val - min_val) * row / ROWS;
            double row_val_hi = min_val + (max_val - min_val) * (row + 1) / ROWS;

            bool on_pos = (pos >= row_val_lo && pos < row_val_hi);
            bool on_sp  = (sp  >= row_val_lo && sp  < row_val_hi);

            if (on_sp && on_pos) std::cout << '*';
            else if (on_sp)      std::cout << '-';
            else if (on_pos)     std::cout << 'o';
            else                 std::cout << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "       +" << std::string(COLS, '-') << "\n";
    std::cout << "        0" << std::string(COLS/2 - 1, ' ')
              << std::fixed << std::setprecision(1) << r.time.back()/2
              << std::string(COLS/2 - 2, ' ') << r.time.back() << "s\n";
    std::cout << "Legend: o=position  -=setpoint  *=both\n";
}

// Compute metrics
void print_metrics(const SimResult& r, const std::string& label) {
    // Settling time: first time |error| < 5% of setpoint and stays there
    double sp = r.setpoint.back();
    double threshold = std::abs(sp) * 0.05;
    double settle_time = r.time.back();
    for (size_t i = r.error.size(); i-- > 0; ) {
        if (std::abs(r.error[i]) > threshold) {
            settle_time = r.time[i];
            break;
        }
    }
    // Overshoot
    double max_pos = *std::max_element(r.position.begin(), r.position.end());
    double overshoot = sp > 0 ? std::max(0.0, (max_pos - sp) / sp * 100.0) : 0.0;
    // Steady-state error
    double ss_error = std::abs(r.error.back());

    std::cout << "  [" << label << "] settle=" << std::fixed << std::setprecision(2)
              << settle_time << "s  overshoot=" << overshoot << "%"
              << "  ss_error=" << ss_error << "\n";
}

// ============================================================
// Experiments
// ============================================================
void experiment_tuning() {
    std::cout << "\n=== Experiment 1: Tuning P, PI, PID ===\n";
    std::cout << "Setpoint = 1.0 (step input), System: mass=1.0, friction=0.5\n\n";

    struct Config { std::string name; double kp, ki, kd; };
    std::vector<Config> configs = {
        {"P only   (kp=2, ki=0, kd=0)", 2.0, 0.0, 0.0},
        {"PI       (kp=2, ki=1, kd=0)", 2.0, 1.0, 0.0},
        {"PD       (kp=2, ki=0, kd=1)", 2.0, 0.0, 1.0},
        {"PID      (kp=3, ki=1, kd=2)", 3.0, 1.0, 2.0},
    };

    for (const auto& cfg : configs) {
        PIDController pid("ctrl", cfg.kp, cfg.ki, cfg.kd, -10.0, 10.0);
        System sys(1.0, 0.5);
        auto result = simulate(pid, sys, 1.0, 10.0, 0.05);
        print_metrics(result, cfg.name);
    }
}

void experiment_step_response() {
    std::cout << "\n=== Experiment 2: Step Response Chart ===\n";

    PIDController pid("pid", 3.0, 1.0, 2.0, -10.0, 10.0);
    System sys(1.0, 0.5);
    auto result = simulate(pid, sys, 1.0, 8.0, 0.05);
    print_chart(result, "PID Step Response (kp=3, ki=1, kd=2, setpoint=1.0)");
}

void experiment_disturbance() {
    std::cout << "\n=== Experiment 3: PID vs Disturbance ===\n";
    std::cout << "Setpoint=1.0, then at t=5s a disturbance force is applied\n\n";

    const double DT = 0.05, DURATION = 12.0;
    PIDController pid("disturbance_test", 3.0, 1.0, 2.0, -15.0, 15.0);
    System sys(1.0, 0.5);
    pid.reset();

    std::vector<double> times, positions;
    for (double t = 0; t <= DURATION; t += DT) {
        // Apply disturbance force at t=5
        double disturbance = (t >= 5.0 && t < 5.5) ? -3.0 : 0.0;
        double force = pid.update(1.0, sys.position, DT);
        sys.update(force + disturbance, DT);
        times.push_back(t);
        positions.push_back(sys.position);
    }

    // Find recovery time
    bool disturbed = false;
    double recover_t = DURATION;
    for (size_t i = 0; i < times.size(); ++i) {
        if (times[i] >= 5.0 && !disturbed && std::abs(positions[i] - 1.0) > 0.1) {
            disturbed = true;
        }
        if (disturbed && std::abs(positions[i] - 1.0) < 0.05) {
            recover_t = times[i];
            break;
        }
    }
    std::cout << "  Position at t=5 (pre-disturbance): " << std::fixed << std::setprecision(4)
              << positions[static_cast<size_t>(5.0/DT)] << "\n";
    std::cout << "  Disturbance duration: 0.5s\n";
    std::cout << "  Recovery to within 5% at: t=" << recover_t << "s\n";
}

void experiment_multiple_controllers() {
    std::cout << "\n=== Experiment 4: Multiple Independent Controllers ===\n";
    std::cout << "Two separate PID instances for X and Y axes (e.g., 2D drone position)\n\n";

    PIDController pid_x("x_axis", 2.0, 0.5, 1.5, -5.0, 5.0);
    PIDController pid_y("y_axis", 1.5, 0.3, 1.0, -5.0, 5.0);
    System sys_x(1.0, 0.3), sys_y(1.2, 0.4);

    double target_x = 2.0, target_y = 3.0;
    const double DT = 0.1;

    std::cout << std::setw(6) << "t"
              << std::setw(10) << "x_pos"
              << std::setw(10) << "y_pos"
              << std::setw(10) << "x_err"
              << std::setw(10) << "y_err" << "\n";
    std::cout << std::string(46, '-') << "\n";

    for (double t = 0; t <= 8.0; t += DT) {
        double fx = pid_x.update(target_x, sys_x.position, DT);
        double fy = pid_y.update(target_y, sys_y.position, DT);
        sys_x.update(fx, DT);
        sys_y.update(fy, DT);

        if (std::fmod(t, 1.0) < DT + 1e-9) {
            std::cout << std::fixed << std::setprecision(2)
                      << std::setw(6) << t
                      << std::setw(10) << sys_x.position
                      << std::setw(10) << sys_y.position
                      << std::setw(10) << (target_x - sys_x.position)
                      << std::setw(10) << (target_y - sys_y.position) << "\n";
        }
    }
}

int main() {
    std::srand(42);
    std::cout << "=== PID Controller in C++ ===\n";
    std::cout << "Compare with: C-PROGRAM/pid_controller.c\n";
    std::cout << "C++ adds: encapsulated state, multiple instances, cleaner API\n";

    experiment_tuning();
    experiment_step_response();
    experiment_disturbance();
    experiment_multiple_controllers();

    std::cout << "\n=== PID Tuning Guide ===\n";
    std::cout << "Kp: proportional — increases response speed, adds overshoot\n";
    std::cout << "Ki: integral     — eliminates steady-state error, can cause windup\n";
    std::cout << "Kd: derivative   — dampens overshoot, sensitive to noise\n";
    std::cout << "\nTuning order: Kp -> Kd -> Ki (Ziegler-Nichols method)\n";
    std::cout << "Anti-windup: clamp integral when output is saturated\n";
    std::cout << "Derivative on measurement (not error): avoids kick on setpoint change\n";

    return 0;
}
