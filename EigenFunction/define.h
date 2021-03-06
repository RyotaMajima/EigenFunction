#pragma once

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "libfftw3-3.lib")
#pragma comment(lib, "gsl.lib")
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
#include<gsl/gsl_poly.h>
using namespace std;
using namespace std::chrono;
using Complex = complex<double>;
using vC = vector<Complex>;
using vvC = vector<vC>;
using vvvC = vector<vvC>;
using vd = vector<double>;
using vvd = vector<vd>;
inline fftw_complex* fftwcast(Complex* f){ return reinterpret_cast<fftw_complex*>(f); }

const double X_BEGIN = -5.0, X_END = 25.0; //系の両端
const double L = X_END - X_BEGIN; //空間幅
const int N = 1024; //空間分割数
const double DELTA_X = L / N;

const double T_END = 100; //終了時刻
const int TN = T_END * 100; //時間分割数
const double dt = T_END / TN; //時間刻み幅

const double S = 0.9; //波束の幅
const double X = -0.5; //初期波束の原点からのずれ
const double X_OPT = X_BEGIN + 3.0 * L / 4.0; //光学ポテンシャルをかける位置

const double d = 3.0; //bの分母
const double b = 1.0 / 3.0; //3次ポテンシャルの係数

//実部の範囲(WKB近似含む)
const double E_BEGIN_real = -1.0 / (6.0 * b*b), E_END_real = 0.0;
const int EN_real = 500;
const double dE_real = (E_END_real - E_BEGIN_real) / EN_real;

//虚部の範囲
const double E_BEGIN_imag = 1.0e-4;
const double E_END_imag = 5.0e-2;
const int EN_imag = 500;
const double dE_imag =(E_END_imag - E_BEGIN_imag) / EN_imag;

/*
b = 1/4 :
    (6.0e-8 ~ 1.0e-4, T = 100)
    5.0e-9 ~ 1.0e-4, T = 200
b = 1/3 :
    1.0e-4 ~ 5.0e-2, N = 1024
b = 1/2 :
    1.0e-3 ~ 5.0e-2
*/