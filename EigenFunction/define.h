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

const double X_BEGIN = -5.0, X_END = 25.0; //�n�̗��[
const double L = X_END - X_BEGIN; //��ԕ�
const int N = 512; //��ԕ�����
const double DELTA_X = L / N;

const double T_END = 300; //�I������
const int TN = T_END * 100; //���ԕ�����
const double dt = T_END / TN; //���ԍ��ݕ�

const double S = 0.9; //�g���̕�
const double X = -0.5; //�����g���̌��_����̂���
const double X_OPT = 4.0; //���w�|�e���V������������ʒu
const double b = 1.0 / 3.0; //3���|�e���V�����̌W��
