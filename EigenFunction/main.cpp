/*
1. �����̂݃p�����[�^�Ƃ��ČŗL�l�̎����𓾂�
2. ����ꂽ�������狕���̃v���b�g�𓾂�
3. �f�[�^��gnuplot�ɓn���ăt�B�b�e�B���O���邱�Ƃŋ����𓾂�
4. �ŗL�֐���\��.
*/

#include "define.h"
#include "solver.h"
#include "common.h"

int main(){
    auto start = system_clock::now();

    printf("---- calculation parameters ----\n");
    printf("system size : \t [%.1f, %.1f]\n", X_BEGIN, X_END);
    printf("partition number : N = %d\n", N);
    printf("calculation time : T = %.1f\n", T_END);
    printf("time step : dt = %.2f\n", dt);
    printf("--------------------------------\n");

    ofstream ofs;

    //gnuplot�p�p�����[�^��������
    ofs.open("params.txt");
    ofs << "T = " << T_END << endl;
    ofs << "N = " << N << endl;
    ofs.close();

    vector<double> res_real(EN_real); //���ʊi�[�p�z��(����)
    energyReal(res_real); //�G�l���M�[�ŗL�l�̎���

    ofs.open("./output/energy_peak_real.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    ofs << scientific;
    for (int i = 0; i < EN_real; i++){
        ofs << i2E(E_BEGIN_real, i, dE_real) << "\t";
        ofs << res_real[i] << endl;
    }

    ofs.close();

    vector<pair<double, int>> peak; //�s�[�N�ʒu�i�[�p�z��
    getPeaks(peak, res_real); //�ŗL�l�̃s�[�N�̒T��(����)

    //gnuplot�p�ǉ���������
    ofs.open("params.txt", ios_base::app);
    ofs << "peakNum = " << peak.size() << endl;
    ofs << fixed;
    for (int i = 0; i < peak.size(); i++){
        ofs << "ER" << i << " = " << i2E(E_BEGIN_real, peak[i].second, dE_real) << endl;
        ofs << "ER" << i << "_val = " << peak[i].first << endl;
    }
    ofs.close();

    vector<vector<double>> res_imag(EN_imag, vector<double>(peak.size())); //���ʊi�[�p�z��(����)
    energyImag(res_imag, peak); //�G�l���M�[�ŗL�l�̋���

    ofs.open("./output/energy_imag.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    ofs << scientific;
    for (int i = 0; i < EN_imag; i++){
        ofs << i2E(E_BEGIN_imag, i, dE_imag) << "\t";
        for (int j = 0; j < peak.size(); j++){
            ofs << res_imag[i][j] << "\t";
        }
        ofs << endl;
    }

    /*
    vvC phi(2, vC(N));
    vector<double> real = { -1.02, -0.156 }, imag = { -5.233e-5, -3.692e-3 };

    //�ŗL�֐��̒��o
    for (int i = 0; i < 2; i++){
        init(f);
        getEigenfunction(phi[i], f, plan_for, plan_back, real[i], imag[i]);
    }

    vector<double> tmp(2);

    for (int i = 0; i < 2; i++){
        tmp[i] = simpson(phi[i]);
    }

    //��r�̂��ߒ��a�U���q�̉����o��
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

    //�p�����[�^�ƌ��ʂ̃t�@�C����������(gnuplot�p)
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

    auto end = system_clock::now();
    auto dur = end - start;
    auto sec = duration_cast<seconds>(dur).count();
    auto min = duration_cast<minutes>(dur).count();

    cout << endl;
    cout << "execution time : " << sec << "sec " << "(" << min << "min)" << endl;

    return 0;
}