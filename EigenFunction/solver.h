#pragma once
#include "define.h"
#include "common.h"

void energyReal(vector<double> &res){
    vC f(N);
    vvC A(EN_real, vC(N));

    fftw_plan plan_for = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_FORWARD, FFTW_MEASURE);
    fftw_plan plan_back = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_BACKWARD, FFTW_MEASURE);

    init(f);

    for (int i = 0; i <= TN; i++){
        //実部のみで振る
        for (int j = 0; j < EN_real; j++){
            for (int k = 0; k < N; k++){
                A[j][k] += f[k] * polar(dt, i2E(E_BEGIN_real, j, dE_real) * (i * dt));
            }
        }

        //時間発展
        timeEvolution(f, plan_for, plan_back);
    }

    for (int i = 0; i < EN_real; i++){
        for (int j = 0; j < N; j++){
            A[i][j] /= T_END;
        }
    }

    for (int i = 0; i < EN_real; i++){
        res[i] = simpson(A[i]);
    }

    fftw_destroy_plan(plan_for);
    fftw_destroy_plan(plan_back);
}

void energyImag(vector<vector<double>> &res, vector<pair<double, int>> &peak){
    vC f(N);
    vvvC B(EN_imag, vvC(peak.size(), vC(N)));

    fftw_plan plan_for = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_FORWARD, FFTW_MEASURE);
    fftw_plan plan_back = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_BACKWARD, FFTW_MEASURE);

    init(f);

    for (int i = 0; i <= TN; i++){
        //虚部のみで振る
        for (int j = 0; j < EN_imag; j++){
            for (int k = 0; k < peak.size(); k++){
                for (int l = 0; l < N; l++){
                    B[j][k][l] += f[l] * polar(dt, i2E(E_BEGIN_real, peak[k].second, dE_real) * (i * dt)) * exp(i2E(E_BEGIN_imag, j, dE_imag) * (i * dt));
                }
            }
        }

        //時間発展
        timeEvolution(f, plan_for, plan_back);
    }

    for (int i = 0; i < EN_imag; i++){
        for (int j = 0; j < peak.size(); j++){
            for (int k = 0; k < N; k++){
                B[i][j][k] *= exp(-i2E(E_BEGIN_imag, i, dE_imag) * T_END) / T_END;
            }
        }
    }

    for (int i = 0; i < EN_imag; i++){
        for (int j = 0; j < peak.size(); j++){
            res[i][j] = simpson(B[i][j]);
        }
    }

    fftw_destroy_plan(plan_for);
    fftw_destroy_plan(plan_back);
}

void getPeaks(vector<pair<double, int>> &peak, vector<double> &res){
    //微分値が正から負に変わったところの値とインデックス
    for (int i = 1; i < EN_real; i++){
        if (res[i - 1] < res[i] && res[i] > res[i + 1]){
            peak.push_back(make_pair(res[i], i));
        }
    }

    //ピーク値の大きい順にソート
    sort(peak.begin(), peak.end(), [](const pair<double, int> &i, const pair<double, int> &j){ return i.first > j.first; });
    if (peak.empty()){
        cout << "no peak" << endl;
        exit(1);
    }
    else{
        double E_th = peak[0].first / 10; //しきい値
        //しきい値以下の要素を削除
        peak.erase(remove_if(peak.begin(), peak.end(), [E_th](pair<double, int> pair) {return pair.first < E_th; }), peak.end());

        //実部の小さい順にソート
        sort(peak.begin(), peak.end(), [](const pair<double, int> &i, const pair<double, int> &j){ return i.second < j.second; });

        //得られたピーク値を表示
        cout << "---- real ver. ----" << endl << endl;

        cout << "threshold value : " << E_th << endl << endl;
        cout << "Re" << "\t" << "peak value" << endl;
        for (auto pair : peak){
            printf("%.3lf\t%.3lf\n", i2E(E_BEGIN_real, pair.second, dE_real), pair.first);
        }
    }
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