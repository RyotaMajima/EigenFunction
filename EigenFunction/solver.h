#pragma once
#include "define.h"
#include "common.h"

void getPeaks(vector<pair<double, int>> &peak, vector<double> &res){
    //�����l�������畉�ɕς�����Ƃ���̒l�ƃC���f�b�N�X
    for (int i = 1; i < EN_real; i++){
        if (res[i - 1] < res[i] && res[i] > res[i + 1]){
            peak.push_back(make_pair(res[i], i));
        }
    }

    //�s�[�N�l�̑傫�����Ƀ\�[�g
    sort(peak.begin(), peak.end(), [](const pair<double, int> &i, const pair<double, int> &j){ return i.first > j.first; });
    if (peak.empty()){
        cout << "no peak" << endl;
        exit(1);
    }
    else{
        double E_th = peak[0].first / 10; //�������l
        //�������l�ȉ��̗v�f���폜
        peak.erase(remove_if(peak.begin(), peak.end(), [E_th](pair<double, int> pair) {return pair.first < E_th; }), peak.end());

        //�����̏��������Ƀ\�[�g
        sort(peak.begin(), peak.end(), [](const pair<double, int> &i, const pair<double, int> &j){ return i.second < j.second; });

        //����ꂽ�s�[�N�l��\��
        cout << "---- real ver. ----" << endl << endl;

        cout << "threshold value : " << E_th << endl << endl;
        cout << "Re" << "\t" << "peak value" << endl;
        for (auto pair : peak){
            printf("%.3lf\t%.3lf\n", i2E(E_BEGIN_real, pair.second, dE_real), pair.first);
        }
    }
}

//�ŗL��Ԃ̒��o
void getEigenfunction(vC &phi, vC &f, fftw_plan plan_for, fftw_plan plan_back, double energy_real, double energy_imag){
    for (int i = 0; i <= TN; i++){
        //�ϕ��v�Z
        for (int j = 0; j < N; j++){
            phi[j] += f[j] * polar(dt, energy_real * (i * dt)) * exp(-energy_imag * (i * dt));
        }

        timeEvolution(f, plan_for, plan_back);
    }

    for (auto &val : phi){
        val *= exp(-fabs(energy_imag) * T_END) / T_END;
    }
}