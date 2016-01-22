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
        cout << "-------peak search-------" << endl << endl;
        //cout << "threshold value : " << E_th << endl << endl;
        cout << "peak pos" << "\t" << "peak value" << endl;
        for (auto pair : peak){
            cout << i2E(E_BEGIN_real, pair.first, dE_real) << "\t\t";
            cout << pair.second << endl;
        }
        cout << endl;
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

    //実部の表示
    cout << "--------real part--------" << endl << endl;
    for (int i = 0; i < real.size(); i++){
        cout << "ER" << i << " = " << real[i] << endl;
    }
    cout << endl;
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
                    B[j][k][l] += f[l] * polar(dt, real[k] * (i * dt)) * exp(GetValueLog(E_BEGIN_imag, E_END_imag, EN_imag, j) * (i * dt));
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
                B[i][j][k] *= exp(-GetValueLog(E_BEGIN_imag, E_END_imag, EN_imag, i) * T_END) / T_END;
            }
        }
    }

    vvd res(EN_imag, vd(real.size()));

    for (int i = 0; i < EN_imag; i++){
        for (int j = 0; j < real.size(); j++){
            res[i][j] = simpson(B[i][j]) * exp(GetValueLog(E_BEGIN_imag, E_END_imag, EN_imag, i) * T_END);
        }
    }

    ofstream ofs("./output/energy_imag.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    ofs << scientific;
    for (int i = 0; i < EN_imag; i++){
        ofs << GetValueLog(E_BEGIN_imag, E_END_imag, EN_imag, i) << "\t";
        for (int j = 0; j < real.size(); j++){
            ofs << res[i][j] << "\t";
        }
        ofs << endl;
    }

    ofs.close();

    //---------gnuplotによるフィッティング-------------
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
    //--------------------------------------------------

    //フィッティング誤差を％に直す
    for (int i = 0; i < imag.size(); i++){
        err[i] = (err[i] / imag[i]) * 100;
    }

    //---------------gnuplot用追加書き込み----------------
    ofs.open("params.txt", ios_base::app);
    for (int i = 0; i < imag.size(); i++){
        ofs << scientific;
        ofs << "EI" << i << " = " << imag[i] << endl;
        ofs << fixed;
        ofs << "EI_err" << i << " = " << err[i] << endl;
    }
    ofs.close();
    //----------------------------------------------------

    //虚部の表示
    cout << "--------imag part--------" << endl << endl;
    for (int i = 0; i < imag.size(); i++){
        cout << scientific;
        cout << "EI" << i << " = " << imag[i];
        cout << fixed;
        cout << " (" << "+/- " << err[i];
        cout << " %" << ")" << endl;
    }

    cout << endl;
}

//固有状態の抽出
void getEigenfunction(vvC &phi, vd &real, vd &imag){
    vC f(N);

    fftw_plan plan_for = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_FORWARD, FFTW_MEASURE);
    fftw_plan plan_back = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_BACKWARD, FFTW_MEASURE);

    for (int i = 0; i < real.size(); i++){
        init(f); //初期化

        for (int j = 0; j <= TN; j++){
            //積分計算
            for (int k = 0; k < N; k++){
                phi[i][k] += f[k] * polar(dt, real[i] * (j * dt)) * exp(-imag[i] * (j * dt));
            }

            timeEvolution(f, plan_for, plan_back);
        }

        for (auto &val : phi[i]){
            //val *= exp(-fabs(imag[i]) * T_END) / T_END;
            val /= T_END;
        }

        //再規格化
        double tmp = simpson(phi[i]);

        for (int j = 0; j < N; j++){
            phi[i][j] = phi[i][j] / sqrt(tmp);
        }

        //cout << simpson(phi[i]) << endl;

        string filename = "./output/phi" + to_string(i) + ".txt";

        ofstream ofs(filename);
        if (!ofs){
            cerr << "file open error!" << endl;
            exit(1);
        }

        ofs << scientific;
        for (int j = 0; j < N; j++){
            ofs << i2x(j) << "\t";
            ofs << V(i2x(j)) << "\t";
            ofs << norm(phi[i][j]) << endl;
        }
    }

    fftw_destroy_plan(plan_for);
    fftw_destroy_plan(plan_back);
}

void getHarmonic(){
    vvd ho(2, vd(N));
    
    for (int i = 0; i < N; i++){
        ho[0][i] = groundState(i2x(i), 0.0);
        ho[1][i] = firstExcited(i2x(i), 0.0);
    }

    ofstream ofs("./output/ho.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    ofs << scientific;
    for (int i = 0; i < N; i++){
        ofs << i2x(i) << "\t";
        ofs << norm(ho[0][i]) << "\t";
        ofs << norm(ho[1][i]) << endl;
    }
}

void decayRatio(vvC &phi, vd &real){
    vC f(N);
    ofstream ofs("./output/decay.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    fftw_plan plan_for = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_FORWARD, FFTW_MEASURE);
    fftw_plan plan_back = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_BACKWARD, FFTW_MEASURE);

    for (int i = 0; i < real.size(); i++){
        for (int j = 0; j < N; j++){
            f[j] = phi[i][j];
        }
        int n = x2i((1.0 / b) + 3.0);

        ofs << scientific;
        for (int j = 0; j <= TN; j++){
            ofs << j * dt << "\t" << simpson(f, n) << endl;
            timeEvolution(f, plan_for, plan_back);
        }
        ofs << endl << endl;
    }

    fftw_destroy_plan(plan_for);
    fftw_destroy_plan(plan_back);
    ofs.close();

    //---------gnuplotによるフィッティング-------------
    FILE *gp = _popen("gnuplot.exe", "w");

    fprintf(gp, "load 'fit_decay.plt'\n");
    fflush(gp);
    _pclose(gp);

    ifstream ifs;

    //フィッティング結果の取得
    ifs.open("./output/fit_result_decay.txt");
    if (!ifs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    vd err(real.size()), lambda(real.size()); //フィッティングの誤差

    for (int i = 0; i < real.size(); i++){
        ifs >> lambda[i] >> err[i];
    }

    ifs.close();
    //--------------------------------------------------

    //フィッティング誤差を％に直す
    for (int i = 0; i < real.size(); i++){
        err[i] = (err[i] / lambda[i]) * 100;
    }

    //---------------gnuplot用追加書き込み----------------
    ofs.open("params.txt", ios_base::app);
    for (int i = 0; i < real.size(); i++){
        ofs << scientific;
        ofs << "lambda" << i << " = " << lambda[i] << endl;
        ofs << fixed;
        ofs << "lambda_err" << i << " = " << err[i] << endl;
    }
    ofs.close();
    //----------------------------------------------------

    //decay rateの表示
    cout << "--------decay rate--------" << endl << endl;
    for (int i = 0; i < real.size(); i++){
        cout << scientific;
        cout << "lambda" << i << " = " << lambda[i];
        cout << fixed;
        cout << " (" << "+/- " << err[i];
        cout << " %" << ")" << endl;
    }

    cout << endl;
}

//WKB被積分関数
double f(double x, double E){
    if (E > V(x)){
        cout << "x: " << x << "\t" << "E > V(x)" << endl;
    }
    return sqrt(2 * (V(x) - E));
}

//転回点   
void calcTurningPoints(vector<double> &x, double E){
    //求解
    gsl_poly_solve_cubic(-3.0 / (2 * b), 0.0, (3.0 / b)*(1.0 / (6.0*b*b) + E), &x[0], &x[1], &x[2]);
}

double calcEta(vector<double> &x, double E){
    double h = (x[2] - x[1]) / N;

    double S_even, S_odd;
    S_even = S_odd = 0.0;

    for (int i = 1; i < (N / 2) - 1; i++){
        S_even += f(i2x(2 * i, x[1], h), E);
    }

    for (int i = 1; i < N / 2; i++){
        S_odd += f(i2x(2 * i - 1, x[1], h), E);
    }
    return h * (f(i2x(0, x[1], h) + 0.001, E) + 2 * S_even + 4 * S_odd + f(i2x(N - 1, x[1], h), E)) / 3.0;
}

double calcT(double eta){
    return exp(-2 * eta) / pow(1 + pow(exp(-eta) / 2.0, 2), 2);
}

void WKB(){
    ofstream ofs("./output/WKB.txt");

    for (int i = 1; i < EN_real; i++){
        vector<double> x(3);
        double E = i2E(E_BEGIN_real, i, dE_real);
        calcTurningPoints(x, E);
        double eta = calcEta(x, E);
        double T = calcT(eta);

        ofs << fixed;
        ofs << E << "\t";
        ofs << scientific;
        ofs << T / (2.0 * 2.0 * M_PI) << endl;
    }

    ofs.close();
}