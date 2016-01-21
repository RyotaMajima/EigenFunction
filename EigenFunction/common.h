#pragma once
#include "define.h"

double i2x(int i){
    return X_BEGIN + i * DELTA_X;
}
double i2k(int i){
    return 2 * M_PI * ((i < N / 2) ? i : i - N) / L;
}
int x2i(double x){
    return (int)((x - X_BEGIN) / DELTA_X);
}

double i2E(double begin, int index, double width){
    return begin + index * width;
}

double groundState(double x, double X){
    return pow(1 / (S*S * M_PI), 1.0 / 4.0) * exp(-(x - X) * (x - X) / (2 * S*S));
}
double firstExcited(double x, double X){
    return ((sqrt(2.0) * (x - X)) / S) * pow(1 / (S*S * M_PI), 1.0 / 4.0) * exp(-(x - X) * (x - X) / (2 * S*S));
}
double secondExcited(double x, double X){
    return sqrt(1 / (3 * S*S*S*S - 2 * S*S + 1)) * pow(1 / (S*S * M_PI), 1.0 / 4.0) * (2 * (x - X) * (x - X) - 1) * exp(-(x - X) * (x - X) / (2 * S*S));
}

//初期化用関数の定義
void init(vC &f){
    for (int i = 0; i < N; i++){
        f[i] = firstExcited(i2x(i), X);
    }
}

//ポテンシャル関数の定義
double V(double x){
    return (1.0 / 2.0) * x*x - (b / 3.0) * x*x*x - 1.0 / (6 * b * b);
    //return (1.0 / 2.0) * x*x;
}

//光学ポテンシャルの定義
double V_opt(double x){
    return x > X_OPT ? x*x : 0.0;
}

//Simpson積分
double simpson(vC &f){
    double S_even, S_odd;
    int n = N / 2;

    S_even = S_odd = 0.0;

    for (int i = 1; i < (n / 2) - 1; i++){
        S_even += norm(f[2 * i]);
    }

    for (int i = 1; i < n / 2; i++){
        S_odd += norm(f[2 * i - 1]);
    }

    return DELTA_X * (norm(f[0]) + 2 * S_even + 4 * S_odd + norm(f[n - 1])) / 3.0;
}

//dtだけ時間発展させる関数
void timeEvolution(vC &f, fftw_plan plan_for, fftw_plan plan_back){
    //ポテンシャル部分の計算
    for (int j = 0; j < N; j++){
        f[j] *= polar(1.0 / N, -V(i2x(j)) * dt); // 1/Nは正規化因子
    }

    //光学ポテンシャル部分の計算
    for (int j = 0; j < N; j++){
        f[j] *= exp(-V_opt(i2x(j)) * dt);
    }

    //f(x, t) -> g(k, t)
    fftw_execute(plan_for);

    //時間発展の計算
    for (int j = 0; j < N; j++){
        f[j] *= polar(1.0, -i2k(j) * i2k(j) * dt / 2.0);
    }

    //f(k, t+dt) -> f(x, t+dt)
    fftw_execute(plan_back);
}