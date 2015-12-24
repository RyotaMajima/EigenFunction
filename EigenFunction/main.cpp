/*
1. 実部のみパラメータとして固有値の実部を得る
2. 得られた実部から虚部のプロットを得る
3. データをgnuplotに渡してフィッティングすることで虚部を得る
4. 固有関数を表示.
*/

#include "define.h"
#include "solver.h"
#include "common.h"

int main(){
    auto start = system_clock::now();

    vC f(N); //時間発展用波動関数
    vvC A(EN_real, vC(N));

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

    ofstream ofs;

    //gnuplot用パラメータ書き込み
    ofs.open("params.txt");
    ofs << "T = " << T_END << endl;
    ofs << "N = " << N << endl;
    ofs.close();    

    init(f); //初期化
    
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

    ofs.open("./output/energy_peak_real.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    vector<double> res_real(EN_real); //結果格納用配列(実部)
    ofs << scientific;
    for (int i = 0; i < EN_real; i++){
        res_real[i] = simpson(A[i]);
        ofs << i2E(E_BEGIN_real, i, dE_real) << "\t";
        ofs << res_real[i] << endl;
    }

    ofs.close();

    vector<pair<double, int>> peak; //ピーク位置格納用配列

    getPeaks(peak, res_real); //固有値のピークの探索(実部)

    int peakNum = peak.size(); //得られたピーク数

    //gnuplot用追加書き込み
    ofs.open("params.txt", ios_base::app);
    ofs << "peakNum = " << peakNum << endl;
    ofs << fixed;
    for (int i = 0; i < peakNum; i++){
        ofs << "ER" << i << " = " << i2E(E_BEGIN_real, peak[i].second, dE_real) << endl;
        ofs << "ER" << i << "_val = " << peak[i].first << endl;
    }
    ofs.close();

    init(f); //再初期化

    vvvC B(EN_imag, vvC(peakNum, vC(N)));

    for (int i = 0; i <= TN; i++){
        //虚部のみで振る
        for (int j = 0; j < EN_imag; j++){
            for (int k = 0; k < peakNum; k++){
                for (int l = 0; l < N; l++){
                    B[j][k][l] += f[l] * polar(dt, i2E(E_BEGIN_real, peak[k].second, dE_real) * (i * dt)) * exp(i2E(E_BEGIN_imag, j, dE_imag) * (i * dt));
                }
            }
        }

        //時間発展
        timeEvolution(f, plan_for, plan_back);
    }

    for (int i = 0; i < EN_imag; i++){
        for (int j = 0; j < peakNum; j++){
            for (int k = 0; k < N; k++){
                B[i][j][k] *= exp(-i2E(E_BEGIN_imag, i, dE_imag) * T_END) / T_END;
            }
        }
    }

    vector<vector<double>> res_imag(EN_imag, vector<double>(peakNum));
    for (int i = 0; i < EN_imag; i++){
        for (int j = 0; j < peakNum; j++){
            res_imag[i][j] = simpson(B[i][j]);
        }
    }

    ofs.open("./output/energy_imag.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    ofs << scientific;
    for (int i = 0; i < EN_imag; i++){
        ofs << i2E(E_BEGIN_imag, i, dE_imag) << "\t";
        for (int j = 0; j < peakNum; j++){
            ofs << res_imag[i][j] << "\t";
        }
        ofs << endl;
    }

    /*
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

    */

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