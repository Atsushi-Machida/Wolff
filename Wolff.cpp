#include <bits/stdc++.h>
#include <random>
#include <vector>
#include <math.h>
// #include <class>
using namespace std;

random_device rnd;

double exp(double x);
double Compute_Probability(double J, double ONEOKBT) {
    return 1.0 - exp(-2.0 * J * ONEOKBT);
}

void Cluster_Flip(int IMAX, int JMAX, vector<vector<int>>& S, vector<vector<vector<vector<int>>>> NEIGHBOR, double p) {
    int a = rnd()%IMAX;
    int b = rnd()%JMAX;
    vector<int> coord = {a, b}; //IMAX, JMAX 未定義
    vector<vector<int>> pocket;
    vector<vector<int>> cluster;

    pocket = {coord};
    cluster = {coord};

    while (pocket.size() != 0)
    {
        int randomNumber = rnd()%pocket.size();
        vector<int> l = pocket.at(randomNumber);

        for (int m = 0; m < 4; m++) //この4は最近接格子数
        {
            
            // NEIGHBOR、S、未定義
            if (S.at(NEIGHBOR.at(l.at(0)).at(l.at(1)).at(m).at(0)).at(NEIGHBOR.at(l.at(0)).at(l.at(1)).at(m).at(1)) == S.at(l.at(0)).at(l.at(1)) )
            {
                printf("calc\n");
                vector<int> mm = {NEIGHBOR.at(l.at(0)).at(l.at(1)).at(m).at(0), NEIGHBOR.at(l.at(0)).at(l.at(1)).at(m).at(1)};
                auto f = find(cluster.begin(), cluster.end(), mm);
                if (f != cluster.end())
                {
                    if ((double)rand()/RAND_MAX < p)
                    {
                        pocket.push_back(NEIGHBOR.at(l.at(0)).at(l.at(1)).at(m));
                        cluster.push_back(NEIGHBOR.at(l.at(0)).at(l.at(1)).at(m));
                    }
                    
                }
                printf("calc end\n");
                
            }
            
        }
        pocket.erase(pocket.begin() + randomNumber);
        
    }

    for (int i = 0; i < cluster.size(); i++)
    {
        printf("coordinate %d %d\n", cluster.at(i).at(0), cluster.at(i).at(1));
        S.at(cluster.at(i).at(0)).at(cluster.at(i).at(1)) *= -1;
    }
    
    
}

int Magnetization(int IMAX, int JMAX, vector<vector<int>> S) {
    int a = 0;
    for (int i = 0; i < IMAX; i++)
    {
        for (int j = 0; j < JMAX; j++)
        {
            printf("i %d j %d\n", i, j);
            a += S.at(i).at(j);
        }
        
    }
    return (double)a / (IMAX) * (JMAX);
   
}

int main()
{
// public:
    int IMAX = 50;
    int JMAX = 50;
    double J = 1.0;
    vector<vector<int>> S(IMAX, vector<int>(JMAX)); //これであってる？
    // for文でSを埋める
    for (int i = 0; i < IMAX; i++){
        for (int j = 0; j < JMAX; j++)
        {
            S.at(i).at(j) = (rnd()%2) * (-2) + 1;
        }
        
    }

    double ONEOKBT = 1.0;
    int NUM_ITER = 30;

    vector<vector<vector<vector<int>>>> NEIGHBOR(IMAX, vector<vector<vector<int>>>(JMAX, vector<vector<int>>(4, vector<int>(2))));
    // for文で要素NEIGHBORを埋める
    for (int i = 0; i < IMAX; i++){
        for (int j = 0; j < JMAX; j++){
            NEIGHBOR.at(i).at(j).at(0).at(0) = (i+1)%IMAX;
            NEIGHBOR.at(i).at(j).at(0).at(1) = j;
            NEIGHBOR.at(i).at(j).at(1).at(0) = (i-1)%IMAX;
            NEIGHBOR.at(i).at(j).at(1).at(1) = j;
            NEIGHBOR.at(i).at(j).at(2).at(0) = i;
            NEIGHBOR.at(i).at(j).at(2).at(1) = (j+1)%JMAX;
            NEIGHBOR.at(i).at(j).at(3).at(0) = i;
            NEIGHBOR.at(i).at(j).at(3).at(0) = (j-1)%JMAX;
        }
    }


    double p = Compute_Probability(J, ONEOKBT);

    vector<double> Magnet_Iter(NUM_ITER); //各イテレーションごとの磁化を保存
    

    for (int n = 0; n < NUM_ITER; n++)
    {
        Magnet_Iter.at(n) = Magnetization(IMAX, JMAX, S);
        // printf("[");
        //     for (int i = 0; i < IMAX; i++)
        //     {
        //         printf("[");
        //         for (int j = 0; j < JMAX; j++)
        //         {
        //             printf("%d ", S.at(i).at(j));
        //         }
        //         printf("]\n");
        //     }
        // printf("]\n\n");

        Cluster_Flip(IMAX, JMAX, S, NEIGHBOR, p);
        printf("%f\n\n", Magnet_Iter.at(n));

    }
    

}
 
