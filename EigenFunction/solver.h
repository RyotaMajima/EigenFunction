#pragma once
#include "define.h"
#include "common.h"

void getPeaks(vector<pair<int, double>> &peak, vector<double> &res){
    //�����l�������畉�ɕς�����Ƃ���̒l�ƃC���f�b�N�X
    for (int i = 1; i < EN_real; i++){
        if (res[i - 1] < res[i] && res[i] > res[i + 1]){
            peak.push_back(make_pair(i, res[i]));
        }
    }

    //�s�[�N�l�̑傫�����Ƀ\�[�g
    sort(peak.begin(), peak.end(), [](const pair<int, double> &i, const pair<int, double> &j){ return i.second > j.second; });
    if (peak.empty()){
        cout << "no peak" << endl;
        exit(1);
    }
    else{
        double E_th = peak[0].second / 10; //�������l
        //�������l�ȉ��̗v�f���폜
        peak.erase(remove_if(peak.begin(), peak.end(), [E_th](pair<int, double> pair) {return pair.second < E_th; }), peak.end());

        //�����̏��������Ƀ\�[�g
        sort(peak.begin(), peak.end(), [](const pair<int, double> &i, const pair<int, double> &j){ return i.first < j.first; });

        //����ꂽ�s�[�N�l��\��
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

    init(f); //������
    vvC A(EN_real, vC(N));

    for (int i = 0; i <= TN; i++){
        //�����݂̂ŐU��
        for (int j = 0; j < EN_real; j++){
            for (int k = 0; k < N; k++){
                A[j][k] += f[k] * polar(dt, i2E(E_BEGIN_real, j, dE_real) * (i * dt));
            }
        }

        //���Ԕ��W
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

    //---------------�t�@�C����������----------------
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

    //---------------gnuplot�p�ǉ���������----------------
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

    //�����̎擾
    for (int i = 0; i < peak.size(); i++){
        real.push_back(i2E(E_BEGIN_real, peak[i].first, dE_real));
    }

    //�����̕\��
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

    init(f); //������

    for (int i = 0; i <= TN; i++){
        //�����݂̂ŐU��
        for (int j = 0; j < EN_imag; j++){
            for (int k = 0; k < real.size(); k++){
                for (int l = 0; l < N; l++){
                    B[j][k][l] += f[l] * polar(dt, real[k] * (i * dt)) * exp(GetValueLog(E_BEGIN_imag, E_END_imag, EN_imag, j) * (i * dt));
                }
            }
        }

        //���Ԕ��W
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

    //---------gnuplot�ɂ��t�B�b�e�B���O-------------
    FILE *gp = _popen("gnuplot.exe", "w");

    fprintf(gp, "load 'fit.plt'\n");
    fflush(gp);
    _pclose(gp);

    ifstream ifs;

    //�t�B�b�e�B���O���ʂ̎擾
    ifs.open("./output/fit_result.txt");
    if (!ifs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    vd err(real.size()); //�t�B�b�e�B���O�̌덷

    for (int i = 0; i < real.size(); i++){
        ifs >> imag[i] >> err[i];
    }
    //--------------------------------------------------

    //�t�B�b�e�B���O�덷�����ɒ���
    for (int i = 0; i < imag.size(); i++){
        err[i] = (err[i] / imag[i]) * 100;
    }

    //---------------gnuplot�p�ǉ���������----------------
    ofs.open("params.txt", ios_base::app);
    for (int i = 0; i < imag.size(); i++){
        ofs << scientific;
        ofs << "EI" << i << " = " << imag[i] << endl;
        ofs << fixed;
        ofs << "EI_err" << i << " = " << err[i] << endl;
    }
    ofs.close();
    //----------------------------------------------------

    //�����̕\��
    cout << "--------imag part--------" << endl << endl;
    for (int i = 0; i < imag.size(); i++){
        cout << scientific;
        cout << "EI" << i << " = " << imag[i];
        cout << fixed;
        cout << " (" << "+/- " << err[i];
        cout << " %" << ")" << endl;
    }

    //ofs.open("./output/error.txt", ios::app);
    //if (!ofs){
    //    cerr << "file open error!" << endl;
    //    exit(1);
    //}

    //ofs << T_END << "\t";
    //for (int i = 0; i < real.size(); i++){
    //    ofs << err[i] << "\t";
    //}
    //ofs << endl;
    //ofs.close();
}

//�ŗL��Ԃ̒��o
void getEigenfunction(vvC &phi, vd &real, vd &imag){
    vC f(N);

    fftw_plan plan_for = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_FORWARD, FFTW_MEASURE);
    fftw_plan plan_back = fftw_plan_dft_1d(N, fftwcast(f.data()), fftwcast(f.data()), FFTW_BACKWARD, FFTW_MEASURE);

    for (int i = 0; i < real.size(); i++){
        init(f); //������

        for (int j = 0; j <= TN; j++){
            //�ϕ��v�Z
            for (int k = 0; k < N; k++){
                phi[i][k] += f[k] * polar(dt, real[i] * (j * dt)) * exp(-imag[i] * (j * dt));
            }

            timeEvolution(f, plan_for, plan_back);
        }

        for (auto &val : phi[i]){
            //val *= exp(-fabs(imag[i]) * T_END) / T_END;
            val /= T_END;
        }

        //�ċK�i��
        double tmp = simpson(phi[i]);

        for (int j = 0; j < N; j++){
            phi[i][j] = norm(phi[i][j]) / tmp;
        }

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
            ofs << phi[i][j].real() << endl;
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
    ofstream ofs("./output/decay.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    fftw_plan plan_for = fftw_plan_dft_1d(N, fftwcast(phi[0].data()), fftwcast(phi[0].data()), FFTW_FORWARD, FFTW_MEASURE);
    fftw_plan plan_back = fftw_plan_dft_1d(N, fftwcast(phi[0].data()), fftwcast(phi[0].data()), FFTW_BACKWARD, FFTW_MEASURE);

    int n = x2i((1.0 / b) + 0.1);

    ofs << scientific;
    for (int i = 0; i <= TN; i++){
        ofs << i * dt << "\t" << simpson(phi[0], n) << endl;
        timeEvolution(phi[0], plan_for, plan_back);
    }

    fftw_destroy_plan(plan_for);
    fftw_destroy_plan(plan_back);

}