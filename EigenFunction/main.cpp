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
    printf("time step : dt = %.3f\n", dt);
    printf("--------------------------------\n\n");

    ofstream ofs;

    //-----gnuplot�p�p�����[�^��������-----
    ofs.open("params.txt");
    ofs << "T = " << T_END << endl;
    ofs << "N = " << N << endl;
    ofs << "b = " << b << endl;
    ofs.close();
    //-------------------------------------

    cout << setprecision(3);

    //----------����-----------
    vd real;
    getRealPart(real);

    //----------����-----------
    vd imag(real.size());
    getImagPart(imag, real);

    ////-------�ŗL�֐��̒��o-------
    //vvC phi(real.size(), vC(N));
    //getEigenfunction(phi, real, imag);

    ////-------���q�U���q�̉�-------
    //getHarmonic();

    //decayRatio(phi, real);

    auto end = system_clock::now();
    auto dur = end - start;
    auto sec = duration_cast<seconds>(dur).count();
    auto min = duration_cast<minutes>(dur).count();

    cout << endl;
    cout << "execution time : " << sec << "sec " << "(" << min << "min)" << endl;

    return 0;
}