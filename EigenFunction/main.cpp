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
    printf("--------------------------------\n\n");

    ofstream ofs;

    //-----gnuplot�p�p�����[�^��������-----
    ofs.open("params.txt");
    ofs << "T = " << T_END << endl;
    ofs << "N = " << N << endl;
    ofs.close();
    //-------------------------------------

    cout << setprecision(3);

    //----------����-----------
    vd real;
    getRealPart(real);

    //----------����-----------
    vd imag(real.size());
    getImagPart(imag, real);

    vvC phi(real.size(), vC(N));

    //�ŗL�֐��̒��o
    for (int i = 0; i < real.size(); i++){
        getEigenfunction(phi[i], real[i], imag[i]);
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


    auto end = system_clock::now();
    auto dur = end - start;
    auto sec = duration_cast<seconds>(dur).count();
    auto min = duration_cast<minutes>(dur).count();

    cout << endl;
    cout << "execution time : " << sec << "sec " << "(" << min << "min)" << endl;

    return 0;
}