#include <bits/stdc++.h>
#include <math.h>
using namespace std;

// 乱数生成のための準備
random_device rnd;
uniform_real_distribution<double> real_random(0.0, 1.0);

double exp(double x);
double Compute_Probability(double J, double ONEOKBT) {// 確率計算
    return 1.0 - exp(-2.0 * J * ONEOKBT);
}

void Cluster_Flip(int IMAX, int JMAX, vector<vector<int>>& S, vector<vector<vector<vector<int>>>> NEIGHBOR, double p) {// cluster flipを行うための関数
    // スタートする座標(格子点)をランダムに選ぶ
    int a = rnd()%IMAX;
    int b = rnd()%JMAX;
    vector<int> coord = {a, b};

    // pocketとclusterを用意して、スタート地点の座標を格納
    vector<vector<int>> pocket;
    vector<vector<int>> cluster;
    pocket = {coord};
    cluster = {coord};

    while (pocket.size() != 0) {// pocketが空になるまでループ
        // pocketの中から、着目する座標(格子点)をランダムに選ぶ
        // 上のcoordはあくまでスタート地点
        int randomNumber = rnd()%pocket.size();
        vector<int> l = pocket[randomNumber];

        for (int m = 0; m < 4; m++) {// この4は最近接格子数、最近接格子ごとにpocket、clusterに登録するかを判定していく
            // 着目した格子点と最近接格子のスピンが同じ向きかどうかを判定
            if (S[ NEIGHBOR[l[0]][l[1]][m][0] ][ NEIGHBOR[l[0]][l[1]][m][1] ] == S[ l[0] ][ l[1] ]) {
                // 最近接格子がclusterに登録されていないかどうかを判定
                if (find(cluster.begin(), cluster.end(), NEIGHBOR[l[0]][l[1]][m]) == cluster.end()) {// c が1だと、clusterに登録済みということになる!
                    double r = real_random(rnd);
                    if (r < p) {// k_BTの値で決まる確率pでcluster、pocketに登録
                        pocket.push_back(NEIGHBOR[l[0]][l[1]][m]);
                        cluster.push_back(NEIGHBOR[l[0]][l[1]][m]);

                    }
                }
            }
        }

        // cluster、pocketの登録作業が終わったら...
        pocket.erase(pocket.begin() + randomNumber); // 着目していた格子点をpocketから削除
        pocket.shrink_to_fit();
    }

    // pocketが空になったら...
    for (int i = 0; i < cluster.size(); i++) {
        S[cluster[i][0]][cluster[i][1]] *= -1; // clusterに登録されているすべての格子点のスピンをフリップ
    }
}

double Magnetization(int IMAX, int JMAX, vector<vector<int>> S) {// 磁化を計算する関数
    double a = 0.0;
    double N = IMAX * JMAX;
    for (int i = 0; i < IMAX; i++) {
        for (int j = 0; j < JMAX; j++) {
            double b = S.at(i).at(j);
            a += b;
        }
    }
    return a / N;   
}

// この下がメイン

int main(){
    int IMAX = 50;
    int JMAX = 50;
    double J = 1.0;
    vector<vector<int>> S(IMAX, vector<int>(JMAX)); // 格子点
    for (int i = 0; i < IMAX; i++) {
        for (int j = 0; j < JMAX; j++) {
            S[i][j] = (rnd()%2) * (-2) + 1;
        }
    }

    double ONEOKBT = 1.0;
    int NUM_ITER = 1000;

    vector<vector<vector<vector<int>>>> NEIGHBOR(IMAX, vector<vector<vector<int>>>(JMAX, vector<vector<int>>(4, vector<int>(2)))); // このNEIGHBOR(i, j)要素にに格子点(i ,j)の最近接格子の座標を格納
    for (int i = 0; i < IMAX; i++){
        for (int j = 0; j < JMAX; j++){
            NEIGHBOR[i][j][0][0] = (i+1)%IMAX;
            NEIGHBOR[i][j][0][1] = j;
            NEIGHBOR[i][j][1][0] = i != 0 ? (i-1)%IMAX: IMAX-1;
            NEIGHBOR[i][j][1][1] = j;
            NEIGHBOR[i][j][2][0] = i;
            NEIGHBOR[i][j][2][1] = (j+1)%JMAX;
            NEIGHBOR[i][j][3][0] = i;
            NEIGHBOR[i][j][3][1] = j != 0 ? (j-1)%JMAX: JMAX-1;
        }
    }

    double p = Compute_Probability(J, ONEOKBT);
    vector<double> Magnet_Iter(NUM_ITER); //各イテレーションごとの磁化を保存
    
    for (int n = 0; n < NUM_ITER; n++) {// イテレーション回、Cluster_Flipを行う
        Magnet_Iter.at(n) = fabs(Magnetization(IMAX, JMAX, S));
        Cluster_Flip(IMAX, JMAX, S, NEIGHBOR, p);
        printf("Iter %d, Magnetization %lf\n", n, Magnet_Iter.at(n));
    }

    ofstream outputfile("Wolff_kbT1.csv");
    
    for (int i = 0; i < NUM_ITER; i++){
        outputfile << i << "," << Magnet_Iter.at(i) << endl;        
    }

    outputfile.close();
}