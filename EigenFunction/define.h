#pragma once

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

const double T_END = 100; //終了時刻
const int TN = T_END * 100; //時間分割数
const double dt = T_END / TN; //時間刻み幅

const double S = 0.9; //波束の幅
const double X = -0.5; //初期波束の原点からのずれ
const double X_OPT = 4.0; //光学ポテンシャルをかける位置
const double b = 1.0 / 3.0; //3次ポテンシャルの係数

const double E_BEGIN_real = -1.5, E_END_real = 0.5;
const int EN_real = 200;
const double dE_real = (E_END_real - E_BEGIN_real) / EN_real;

const double E_BEGIN_imag = 0.0, E_END_imag = 0.03;
const int EN_imag = 200;
const double dE_imag = (E_END_imag - E_BEGIN_imag) / EN_imag;