/*
1. 実部のみパラメータとして固有値の実部を得る
2. 得られた実部から虚部のプロットを得る
3. データをgnuplotに渡してフィッティングすることで虚部を得る
4. 固有関数を表示する
5. WKB近似のトンネル確率から寿命の逆数を得る
5. ノルムの時間発展をフィッティングして寿命の逆数を得る
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

    //-----gnuplot用パラメータ書き込み-----
    ofs.open("params.txt");
    ofs << "T = " << T_END << endl;
    ofs << "TN = " << TN << endl;
    ofs << "N = " << N << endl;
    ofs << "b = " << b << endl;
    ofs.close();
    //-------------------------------------

    cout << setprecision(3);

    //----------実部-----------
    vd real;
    getRealPart(real);

    //----------虚部-----------
    vd imag(real.size());
    getImagPart(imag, real);

    //波束の時間発展から求めた複素固有値の書き込み
    ofs.open("./output/eigenValueFit3.txt");
    if (!ofs){
        cerr << "file open error!" << endl;
        exit(1);
    }

    ofs << scientific;
    for (int i = 0; i < real.size(); i++){
        ofs << real[i] << "\t" << imag[i] << endl;
    }

    ofs.close();

    //-------固有関数の抽出-------
    vvC phi(real.size(), vC(N));
    getEigenfunction(phi, real, imag);

    //-------調子振動子の解-------
    getHarmonic();

    //----------WKB近似-----------
    WKB();

    //-------減衰率からの虚部の推定-------
    decayRatio(phi, real);

    auto end = system_clock::now();
    auto dur = end - start;
    auto sec = duration_cast<seconds>(dur).count();
    auto min = duration_cast<minutes>(dur).count();

    cout << endl;
    cout << "execution time : " << sec << "sec " << "(" << min << "min)" << endl;

    return 0;
}