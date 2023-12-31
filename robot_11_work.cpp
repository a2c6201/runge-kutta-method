#include <stdio.h>
#include <iostream>
#include <fstream>

// 問題の理解
// パラメーターの意味
// ルンゲクッタ法の意味
// コメントの追加

// パラメーター
const double theta_d = 1.0;  // 目標角度 (rad)
const double I = 2.0;       // 慣性モーメント (kg.m^2)
const double D = 3.0;       // 粘性係数 (kg.m^2/s)
const double h = 0.001;     // 刻み幅 (s)
const double kp = 100.0;    // 比例ゲイン
const double kd = 20.0;     // 微分ゲイン
const double t_max = 5.0;   // シミュレーション時間 (s)

// Function to calculate the torque
double control_law(double theta, double omega) {
    return kp * (theta_d - theta) - kd * omega;
}

// Function to calculate the derivative of state
void derivative(double y[], double dy[], double torque) {
    dy[0] = y[1];
    dy[1] = torque / I - (D / I) * y[1];
}

// Runge-Kutta 4th order method
void runge_kutta(double y[], double h) {
    double k1[2], k2[2], k3[2], k4[2], y_temp[2];

    double torque = control_law(y[0], y[1]);
    derivative(y, k1, torque);

    y_temp[0] = y[0] + k1[0] * h / 2;
    y_temp[1] = y[1] + k1[1] * h / 2;
    torque = control_law(y_temp[0], y_temp[1]);
    derivative(y_temp, k2, torque);

    y_temp[0] = y[0] + k2[0] * h / 2;
    y_temp[1] = y[1] + k2[1] * h / 2;
    torque = control_law(y_temp[0], y_temp[1]);
    derivative(y_temp, k3, torque);

    y_temp[0] = y[0] + k3[0] * h;
    y_temp[1] = y[1] + k3[1] * h;
    torque = control_law(y_temp[0], y_temp[1]);
    derivative(y_temp, k4, torque);

    y[0] += (k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]) * h / 6;
    y[1] += (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]) * h / 6;
}

int main() {
    std::ofstream data("data.txt");// Start the interpreter

    double y[2] = {0, 0};  // Initial conditions: [theta, omega]
    for (double t = 0; t <= t_max; t += h) {
        printf("Time: %.3f, Theta: %.5f\n", t, y[0]);
        // グラフ描画
        data << t << " " << y[0] << std::endl; // Write the data to the file
        // ランゲクッタ法によるシミュレーション
        runge_kutta(y, h);
    }
    data.close();

    // グラフ描画
    FILE *gp = popen("gnuplot -persist", "w");
    if (gp == NULL) {
        std::cerr << "Cannot open pipe to gnuplot" << std::endl;
        return 1;
    }

    fprintf(gp, "plot 'data.txt' with lines\n");
    fflush(gp);

    pclose(gp);
    return 0;
}
