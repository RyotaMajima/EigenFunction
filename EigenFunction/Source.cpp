/*
得られた複素固有値から固有関数を表示する
*/

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "libfftw3-3.lib")
#include<iostream>
#include<iomanip>
#include<fstream>
#include<cstdio>
#include<cmath>
#include<vector>
#include<utility>
#include<string>
#include<algorithm>
#include<functional>
#include<complex>
#include<chrono>
#include<fftw3.h>
using namespace std;
using namespace std::chrono;
using Complex = complex<double>;
using vC = vector<Complex>;
using vvC = vector<vC>;
using vvvC = vector<vvC>;
inline fftw_complex* fftwcast(Complex* f){ return reinterpret_cast<fftw_complex*>(f); }

const double X_BEGIN = -5.0, X_END = 25.0; //系の両端
const double L = X_END - X_BEGIN; //空間幅
const int N = 512; //空間分割数
const double DELTA_X = L / N;

const double T_END = 300; //終了時刻
const int TN = T_END * 100; //時間分割数
const double dt = T_END / TN; //時間刻み幅

const double S = 0.9; //波束の幅
const double X = -0.5; //初期波束の原点からのずれ
const double X_OPT = 4.0; //光学ポテンシャルをかける位置
const double b = 1.0 / 3.0; //3次ポテンシャルの係数

double i2x(int i){
    return X_BEGIN + i * DELTA_X;
}
double i2k(int i){
    return 2 * M_PI * ((i < N / 2) ? i : i - N) / L;
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

    S_even = S_odd = 0.0;

    for (int i = 1; i < (N / 2) - 1; i++){
        S_even += norm(f[2 * i]);
    }

    for (int i = 1; i < N / 2; i++){
        S_odd += norm(f[2 * i - 1]);
    }

    return DELTA_X * (norm(f[0]) + 2 * S_even + 4 * S_odd + norm(f[N - 1])) / 3.0;
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

//固有状態の抽出
void getEigenfunction(vC &phi, vC &f, fftw_plan plan_for, fftw_plan plan_back, double energy_real, double energy_imag){
    for (int i = 0; i <= TN; i++){
        //積分計算
        for (int j = 0; j < N; j++){
            phi[j] += f[j] * polar(dt, energy_real * (i * dt)) * exp(-energy_imag * (i * dt));
        }

        timeEvolution(f, plan_for, plan_back);
    }

    for (auto &val : phi){
        val *= exp(-fabs(energy_imag) * T_END) / T_END;
    }
}

int main(){
    auto start = system_clock::now();
    vC f(N);

    //順方向Fourier変換
    fftw_plan plan_for = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_FORWARD, FFTW_MEASURE);

    //逆方向
    fftw_plan plan_back = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_BACKWARD, FFTW_MEASURE);

    printf("---- calculation parameters ----\n");
    printf("system size : \t [%.1f, %.1f]\n", X_BEGIN, X_END);
    printf("partition number : N = %d\n", N);
    printf("calculation time : T = %.1f\n", T_END);
    printf("time step : dt = %.2f\n", dt);
    printf("--------------------------------\n");


    vvC phi(2, vC(N));
    vector<double> real = { -1.02, -0.156 }, imag = { -5.233e-5, -3.692e-3 };

    //固有関数の抽出
    for (int i = 0; i < 2; i++){
        init(f);
        getEigenfunction(phi[i], f, plan_for, plan_back, real[i], imag[i]);
    }

    vector<double> tmp(2);

    for (int i = 0; i < 2; i++){
        tmp[i] = simpson(phi[i]);
    }

    //比較のため調和振動子の解を出力
    vector<vector<double>> ho(2, vector<double>(N));
    for (int i = 0; i < N; i++){
        double x = i2x(i);
        ho[0][i] = groundState(x, 0.0);
        ho[1][i] = firstExcited(x, 0.0);
    }

    ofstream ofs;
    ofs.open("./output/phi.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    ofs << scientific;
    for (int i = 0; i < N; i++){
        ofs << i2x(i) << "\t" << V(i2x(i)) <<"\t";
        for (int j = 0; j < 2; j++){
            ofs << norm(ho[j][i]) << "\t";
            ofs << norm(phi[j][i]) / tmp[j] << "\t";
        }
        ofs << endl;
    }

    ofs.close();

    //パラメータと結果のファイル書き込み(gnuplot用)
    FILE *fp = fopen("params.txt", "w");
    if (fp == NULL){
        cerr << "file open error!" << endl;
        exit(1);
    }

    fprintf(fp, "T = %.1f\n", T_END);
    fprintf(fp, "N = %d\n", N);

    for (int i = 0; i < 2; i++){
        fprintf(fp, "ER%d = %.3f\n", i, real[i]);
        fprintf(fp, "EI%d = %.3e\n", i, imag[i]);
    }

    fclose(fp);

    fftw_destroy_plan(plan_for);
    fftw_destroy_plan(plan_back);

    auto end = system_clock::now();
    auto dur = end - start;
    auto sec = duration_cast<seconds>(dur).count();
    auto min = duration_cast<minutes>(dur).count();

    cout << endl;
    cout << "execution time : " << sec << "sec " << "(" << min << "min)" << endl;

    return 0;
}