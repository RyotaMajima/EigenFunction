#pragma once
#include "define.h"
#include "common.h"

void getPeaks(vector<pair<int, double>> &peak, vector<double> &res){
    //微分値が正から負に変わったところの値とインデックス
    for (int i = 1; i < EN_real; i++){
        if (res[i - 1] < res[i] && res[i] > res[i + 1]){
            peak.push_back(make_pair(i, res[i]));
        }
    }

    //ピーク値の大きい順にソート
    sort(peak.begin(), peak.end(), [](const pair<int, double> &i, const pair<int, double> &j){ return i.second > j.second; });
    if (peak.empty()){
        cout << "no peak" << endl;
        exit(1);
    }
    else{
        double E_th = peak[0].second / 10; //しきい値
        //しきい値以下の要素を削除
        peak.erase(remove_if(peak.begin(), peak.end(), [E_th](pair<int, double> pair) {return pair.second < E_th; }), peak.end());

        //実部の小さい順にソート
        sort(peak.begin(), peak.end(), [](const pair<int, double> &i, const pair<int, double> &j){ return i.first < j.first; });

        //得られたピーク値を表示
        cout << "threshold value : " << E_th << endl << endl;
        cout << "Re" << "\t" << "peak value" << endl;
        for (auto pair : peak){
            printf("%.3lf\t%.3lf\n", i2E(E_BEGIN_real, pair.first, dE_real), pair.second);
        }
    }
}

void getRealPart(vector<double> &real){
    vC f(N);

    fftw_plan plan_for = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_FORWARD, FFTW_MEASURE);
    fftw_plan plan_back = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_BACKWARD, FFTW_MEASURE);

    init(f); //初期化
    vvC A(EN_real, vC(N));

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

    fftw_destroy_plan(plan_for);
    fftw_destroy_plan(plan_back);

    for (int i = 0; i < EN_real; i++){
        for (int j = 0; j < N; j++){
            A[i][j] /= T_END;
        }
    }

    vd res(EN_real);
    for (int i = 0; i < EN_real; i++){
        res[i] = simpson(A[i]);
    }

    //---------------ファイル書き込み----------------
    ofstream ofs("./output/energy_peak_real.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    ofs << scientific;
    for (int i = 0; i < EN_real; i++){
        ofs << i2E(E_BEGIN_real, i, dE_real) << "\t";
        ofs << res[i] << endl;
    }
    ofs.close();
    //-------------------------------------------------

    vector<pair<int, double>> peak;
    getPeaks(peak, res);

    //---------------gnuplot用追加書き込み----------------
    ofs.open("params.txt", ios_base::app);
    ofs << "peakNum = " << peak.size() << endl;
    ofs << fixed;
    for (int i = 0; i < peak.size(); i++){
        ofs << "ER" << i << " = ";
        ofs << i2E(E_BEGIN_real, peak[i].first, dE_real) << endl;
        ofs << "ER" << i << "_val = " << peak[i].second << endl;
    }
    ofs.close();
    //----------------------------------------------------

    //実部の取得
    for (int i = 0; i < peak.size(); i++){
        real.push_back(i2E(E_BEGIN_real, peak[i].first, dE_real));
    }
}

void getImagPart(vector<double> &imag, vector<double> &real){
    vC f(N);
    vvvC B(EN_imag, vvC(real.size(), vC(N)));

    fftw_plan plan_for = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_FORWARD, FFTW_MEASURE);
    fftw_plan plan_back = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_BACKWARD, FFTW_MEASURE);

    init(f); //初期化

    for (int i = 0; i <= TN; i++){
        //虚部のみで振る
        for (int j = 0; j < EN_imag; j++){
            for (int k = 0; k < real.size(); k++){
                for (int l = 0; l < N; l++){
                    B[j][k][l] += f[l] * polar(dt, real[k] * (i * dt)) * exp(i2E(E_BEGIN_imag, j, dE_imag) * (i * dt));
                }
            }
        }

        //時間発展
        timeEvolution(f, plan_for, plan_back);
    }

    fftw_destroy_plan(plan_for);
    fftw_destroy_plan(plan_back);

    for (int i = 0; i < EN_imag; i++){
        for (int j = 0; j < real.size(); j++){
            for (int k = 0; k < N; k++){
                B[i][j][k] *= exp(-i2E(E_BEGIN_imag, i, dE_imag) * T_END) / T_END;
            }
        }
    }

    vvd res(EN_imag, vd(real.size()));

    for (int i = 0; i < EN_imag; i++){
        for (int j = 0; j < real.size(); j++){
            res[i][j] = simpson(B[i][j]);
        }
    }

    ofstream ofs("./output/energy_imag.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    ofs << scientific;
    for (int i = 0; i < EN_imag; i++){
        ofs << i2E(E_BEGIN_imag, i, dE_imag) << "\t";
        for (int j = 0; j < real.size(); j++){
            ofs << res[i][j] << "\t";
        }
        ofs << endl;
    }

    //--------gnuplotによるフィッティング-------------------
    FILE *gp = _popen("gnuplot.exe", "w");

    fprintf(gp, "load 'fit.plt'\n");
    fflush(gp);
    _pclose(gp);

    ifstream ifs;

    //フィッティング結果の取得
    ifs.open("./output/fit_result.txt");
    if (!ifs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    vd err(real.size()); //フィッティングの誤差

    for (int i = 0; i < real.size(); i++){
        ifs >> imag[i] >> err[i];
    }
    //------------------------------------------------------

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