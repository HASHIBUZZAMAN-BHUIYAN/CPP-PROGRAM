// ADVANCED ROBOTICS_CV: Linear Algebra for Robotics with Eigen
//
// Eigen is a header-only C++ template library for linear algebra.
// It is used by ROS2, OpenCV, and virtually every serious robotics stack.
//
// NOTE: This file self-contains a minimal Vec/Mat implementation as fallback
// if Eigen is not installed. If you have Eigen headers, compile with:
//   g++ -std=c++17 -O2 -I/path/to/eigen -DUSE_EIGEN -o ...
// Otherwise (default, no Eigen required):
//   g++ -std=c++17 -O2 -o eigen_linear_algebra_robotics.exe eigen_linear_algebra_robotics.cpp
//
// CONCEPTS:
// - Homogeneous transformation matrices (4x4)
// - Rotation matrices (Rz, Ry, Rx)
// - Translation vectors
// - Forward kinematics: chain T = T1 * T2 * T3
// - Inverse (for rotation matrix = transpose; for T4x4 = block inverse)

#include <iostream>
#include <cmath>
#include <iomanip>
#include <array>
#include <string>

const double PI = 3.14159265358979;
double deg2rad(double d) { return d * PI / 180.0; }

// ============================================================
// Minimal 4x4 matrix for transformation math (no Eigen needed)
// ============================================================
struct Mat4 {
    std::array<std::array<double,4>,4> m{};

    Mat4() { for (auto& r : m) r.fill(0.0); }

    static Mat4 identity() {
        Mat4 I;
        I.m[0][0] = I.m[1][1] = I.m[2][2] = I.m[3][3] = 1.0;
        return I;
    }

    double  operator()(int r, int c) const { return m[r][c]; }
    double& operator()(int r, int c)       { return m[r][c]; }

    Mat4 operator*(const Mat4& B) const {
        Mat4 C;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                for (int k = 0; k < 4; ++k)
                    C.m[i][j] += m[i][k] * B.m[k][j];
        return C;
    }

    void print(const std::string& label = "") const {
        if (!label.empty()) std::cout << label << "\n";
        for (int r = 0; r < 4; ++r) {
            std::cout << "  [";
            for (int c = 0; c < 4; ++c)
                std::cout << std::fixed << std::setprecision(4) << std::setw(9) << m[r][c];
            std::cout << " ]\n";
        }
    }
};

// Vec3 for end-effector position extraction
struct Vec3 { double x, y, z; };

// ============================================================
// Transformation matrix constructors
// ============================================================

// Translation
Mat4 translation(double tx, double ty, double tz) {
    Mat4 T = Mat4::identity();
    T(0,3) = tx; T(1,3) = ty; T(2,3) = tz;
    return T;
}

// Rotation about Z axis (yaw)
Mat4 rot_z(double theta) {
    Mat4 R = Mat4::identity();
    double c = std::cos(theta), s = std::sin(theta);
    R(0,0) =  c; R(0,1) = -s;
    R(1,0) =  s; R(1,1) =  c;
    return R;
}

// Rotation about Y axis (pitch)
Mat4 rot_y(double theta) {
    Mat4 R = Mat4::identity();
    double c = std::cos(theta), s = std::sin(theta);
    R(0,0) =  c; R(0,2) =  s;
    R(2,0) = -s; R(2,2) =  c;
    return R;
}

// Rotation about X axis (roll)
Mat4 rot_x(double theta) {
    Mat4 R = Mat4::identity();
    double c = std::cos(theta), s = std::sin(theta);
    R(1,1) =  c; R(1,2) = -s;
    R(2,1) =  s; R(2,2) =  c;
    return R;
}

// Extract end-effector position from T matrix (last column)
Vec3 position(const Mat4& T) { return {T(0,3), T(1,3), T(2,3)}; }

// ============================================================
// Demo 1: Rotation matrices
// ============================================================
void demo_rotations() {
    std::cout << "\n=== Rotation Matrices ===\n";

    Mat4 Rz45 = rot_z(deg2rad(45));
    Rz45.print("Rz(45 deg) — rotate 45 degrees around Z:");

    Mat4 Ry30 = rot_y(deg2rad(30));
    Ry30.print("Ry(30 deg) — rotate 30 degrees around Y:");

    // Compose: first Rz, then Ry
    Mat4 composed = Ry30 * Rz45;
    composed.print("Ry(30) * Rz(45) — composed rotation:");

    std::cout << "\nKey property: rotation matrices are orthogonal (R^T = R^-1)\n";
    std::cout << "Determinant of rotation matrix = 1.0 (always)\n";
    // Verify: R * R^T should be identity
    // (checking M[0][0]: cos^2 + sin^2 = 1)
}

// ============================================================
// Demo 2: Homogeneous transforms
// ============================================================
void demo_homogeneous() {
    std::cout << "\n=== Homogeneous Transformation (4x4) ===\n";
    std::cout << "T = [R | t]  where R=3x3 rotation, t=3x1 translation\n";
    std::cout << "    [0 | 1]\n\n";

    // Transform: rotate 90 degrees around Z, then translate (1, 2, 0)
    Mat4 Rz90   = rot_z(deg2rad(90));
    Mat4 Trans  = translation(1, 2, 0);
    Mat4 T      = Trans * Rz90;
    T.print("T = Translate(1,2,0) * Rz(90deg):");

    // Apply T to a point p = (1, 0, 0, 1)^T
    double px = T(0,0)*1 + T(0,1)*0 + T(0,2)*0 + T(0,3)*1;
    double py = T(1,0)*1 + T(1,1)*0 + T(1,2)*0 + T(1,3)*1;
    std::cout << "\nApply T to point (1,0,0): -> ("
              << std::fixed << std::setprecision(4) << px << ", " << py << ", 0)\n";
    std::cout << "Expected: Rz(90) turns (1,0,0) -> (0,1,0), then translate -> (1, 3, 0)\n";
}

// ============================================================
// Demo 3: 3-DOF Robot Arm Forward Kinematics
// ============================================================
struct JointConfig { double theta1, theta2, theta3; };  // joint angles in degrees

void demo_forward_kinematics() {
    std::cout << "\n=== 3-DOF Robot Arm Forward Kinematics ===\n";
    std::cout << "Robot: 3 revolute joints, link lengths L1=1.0, L2=0.8, L3=0.5\n\n";

    const double L1 = 1.0, L2 = 0.8, L3 = 0.5;

    auto fk = [&](JointConfig cfg) -> Vec3 {
        // Joint 1: rotate around Z, then extend along X by L1
        Mat4 T1 = rot_z(deg2rad(cfg.theta1)) * translation(L1, 0, 0);
        // Joint 2: rotate around Z relative to joint 1, extend by L2
        Mat4 T2 = rot_z(deg2rad(cfg.theta2)) * translation(L2, 0, 0);
        // Joint 3: rotate around Z relative to joint 2, extend by L3
        Mat4 T3 = rot_z(deg2rad(cfg.theta3)) * translation(L3, 0, 0);

        // Total: T_base_to_end = T1 * T2 * T3
        Mat4 T_total = T1 * T2 * T3;
        return position(T_total);
    };

    // Configuration 1: all joints at 0 — fully extended
    JointConfig home{0, 0, 0};
    Vec3 p = fk(home);
    std::cout << "Home (0,0,0):         end-effector = ("
              << std::fixed << std::setprecision(4) << p.x << ", " << p.y << ", " << p.z << ")\n";
    std::cout << "  Expected: (2.3, 0, 0) — fully extended along X\n";

    // Configuration 2: all joints at 90 degrees
    JointConfig bent{90, 90, 0};
    p = fk(bent);
    std::cout << "Bent (90,90,0):       end-effector = ("
              << p.x << ", " << p.y << ", " << p.z << ")\n";

    // Configuration 3: reaching left
    JointConfig reach{45, -30, 15};
    p = fk(reach);
    std::cout << "Reach (45,-30,15):    end-effector = ("
              << p.x << ", " << p.y << ", " << p.z << ")\n";

    // Configuration 4: folded back
    JointConfig fold{0, 180, 0};
    p = fk(fold);
    std::cout << "Folded (0,180,0):     end-effector = ("
              << p.x << ", " << p.y << ", " << p.z << ")\n";
    std::cout << "  Expected: (~0.3, 0, 0) — L1 - L2 + L3\n";
}

// ============================================================
// Demo 4: Denavit-Hartenberg (DH) intuition
// ============================================================
void demo_dh_intuition() {
    std::cout << "\n=== Denavit-Hartenberg Parameters (Intuition) ===\n";
    std::cout << "DH represents each joint-to-joint transform with 4 parameters:\n";
    std::cout << "  a     (link length)   — distance along X_i\n";
    std::cout << "  alpha (link twist)    — rotation around X_i\n";
    std::cout << "  d     (link offset)   — distance along Z_{i-1}\n";
    std::cout << "  theta (joint angle)   — rotation around Z_{i-1}  <-- this is the variable\n\n";
    std::cout << "T_i = Rot_z(theta) * Trans_z(d) * Trans_x(a) * Rot_x(alpha)\n\n";

    // DH transform for one joint
    auto dh_transform = [](double theta, double d, double a, double alpha) -> Mat4 {
        return rot_z(theta) * translation(0,0,d) * translation(a,0,0) * rot_x(alpha);
    };

    // 2-DOF planar robot using DH
    double theta1 = deg2rad(30), theta2 = deg2rad(60);
    double L1 = 1.0, L2 = 0.8;

    Mat4 T1 = dh_transform(theta1, 0, L1, 0);
    Mat4 T2 = dh_transform(theta2, 0, L2, 0);
    Mat4 T_total = T1 * T2;

    Vec3 p = position(T_total);
    std::cout << "2-DOF DH robot (theta1=30, theta2=60, L1=1, L2=0.8):\n";
    std::cout << "  End-effector: (" << std::fixed << std::setprecision(4)
              << p.x << ", " << p.y << ", " << p.z << ")\n";

    // Verify with trig formula: x = L1*cos(t1) + L2*cos(t1+t2)
    double x_check = L1*std::cos(theta1) + L2*std::cos(theta1+theta2);
    double y_check = L1*std::sin(theta1) + L2*std::sin(theta1+theta2);
    std::cout << "  Trig check:   (" << x_check << ", " << y_check << ", 0)\n";
    std::cout << "  (Should match)\n";
}

int main() {
    std::cout << "=== Linear Algebra for Robotics (C++ without Eigen) ===\n";
    std::cout << "Using self-contained 4x4 matrix implementation.\n";
    std::cout << "In production robotics code, replace Mat4 with Eigen::Matrix4d.\n";

    demo_rotations();
    demo_homogeneous();
    demo_forward_kinematics();
    demo_dh_intuition();

    std::cout << "\n=== How to use Eigen instead ===\n";
    std::cout << "Install: vcpkg install eigen3  OR  download header-only from eigen.tuxfamily.org\n";
    std::cout << "Then replace Mat4 with Eigen::Matrix4d, Vec3 with Eigen::Vector3d,\n";
    std::cout << "and use Eigen::AngleAxisd for cleaner rotation construction.\n";
    std::cout << "The math and concepts are identical.\n";

    return 0;
}
