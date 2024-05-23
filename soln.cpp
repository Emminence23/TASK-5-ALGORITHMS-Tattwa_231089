#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <queue>
#include <vector>
#include <map>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(){
    auto start = high_resolution_clock::now();

    fstream fin; 
    fin.open("flights.csv", ios::in); 

    string temp;
    vector<array<int,5>> Flights;
    fin>>temp;
    int FID,dep,arr,cap,Dt,At;
    char comma;
    fin>>FID>>comma>>dep>>comma>>arr>>comma>>cap>>comma>>Dt>>comma>>At;
    if(FID == 1){Flights.push_back({0,0,0,0,0});}
    Flights.push_back({dep-1,arr-1,cap,Dt,At});

    int N = max(dep,arr);
    
    while(fin>>FID){
        fin>>comma>>dep>>comma>>arr>>comma>>cap>>comma>>Dt>>comma>>At;
        Flights.push_back({dep-1,arr-1,cap,Dt,At});
        N = max(N,max(dep,arr));
    }
    fin.close();
    int F = Flights.size();

    fin.open("canceled.csv", ios::in); 
    fin>>temp;
    vector<int> can;
    while(fin>>FID){
        can.push_back(FID);
    }
    int C = can.size();
    vector<vector<int>> Affected(C);
    fin.close();

    int Affc = 0,AvgLay = 0,TimeDiff = 0;

    fin.open("passengers.csv", ios::in); 
    fin>>temp;
    int PID;
    while(fin>>PID){
        fin>>comma>>FID;
        int flag = 0;
        for(int i = 0; i < C; i++){if(FID == can[i]){Affected[i].push_back(PID) ;Affc++; flag = 1; break;}}
        if(flag == 0){
            Flights[FID][2]--;
        }
    }
    fin.close();
    for(int i = 0; i < C; i++){Flights[can[i]][2] = 0;}

    vector<vector<int>> alts(N);
    vector<vector<int>> fins(N);
    for(int i = 0 ; i < F; i++){
        if(Flights[i][2] > 0){
            int a = Flights[i][0];
            int b = Flights[i][1];
            int flag = 0;
            for(int j = 0 ; j < C; j++){
                if(b == Flights[can[j]][1]){
                    fins[a].push_back(i);
                    flag++;
                }
            }
            if(flag == 0){alts[a].push_back(i);}
        }
    }
    vector<int> collisions(F,0);
    vector<queue<int>> q1(C);
    vector<vector<int>> s1(C);

    for(int i = 0; i < C; i++){
        int x = can[i]; //FID

        int a = Flights[x][0];
        for(int y : fins[a]){
            if(Flights[y][1] == Flights[can[i]][1]){s1[i].push_back(y); collisions[y]++;}
        }
        for(int y : alts[a]){
           q1[i].push(y);
        }
    }
    vector<vector<pair<int,int>>> s2(C);
    vector<vector<array<int,3>>> s3(C);
    for(int i = 0; i < C; i++){
        while(!q1[i].empty()){
            int x = q1[i].front(); //FID
            q1[i].pop();
            int a = Flights[x][1];
            for(int y : fins[a]){
                if(Flights[y][1] == Flights[can[i]][1]){s2[i].push_back(pair<int,int>{x,y}); collisions[x]++; collisions[y]++;}
            }
            for(int y : alts[a]){
                int b = Flights[y][0];
                for(int z : fins[b]){
                    if(Flights[z][1] == Flights[can[i]][1]){s3[i].push_back(array<int,3>{x,y,z}); collisions[x]++; collisions[y]++; collisions[z]++;}
                }
            }
        }
    }

    int count = 0;

    map<int,vector<int>> allot;
    vector<int> resolved(C);
    for(int i = 0; i < C; i++){resolved[i] = 0;}
    vector<int> Afid(C,0);
    for(int i = 0; i < C; i++){
        if(resolved[i] == 1){continue;}
        for(int x : s1[i]){
            //if(collisions[x] == 1){
                int sz = Flights[x][2];
                for(int j = Afid[i]; j < min(Afid[i] + sz,(int)Affected[i].size()); j++){
                    allot[Affected[i][j]].push_back(1);
                    allot[Affected[i][j]].push_back(x);
                    AvgLay += 1;
                    TimeDiff += abs(Flights[can[i]][4] - Flights[x][4]);
                    count++;
                }
                Afid[i] += sz;
                //collisions[x] = 0;
                if(Afid[i] >= Affected[i].size()){resolved[i] = 1; break;}
            
        }
    }

    for(int i = 0; i < C; i++){
        if(resolved[i] == 1){continue;}
        for(auto p : s2[i]){
            //if(collisions[x] == 1){
                int x = p.first, y = p.second;
                int sz = min(Flights[x][2],Flights[y][2]);
                for(int j = Afid[i]; j < min(Afid[i] + sz,(int)Affected[i].size()); j++){
                    allot[Affected[i][j]].push_back(2);
                    allot[Affected[i][j]].push_back(x);
                    allot[Affected[i][j]].push_back(y);
                    AvgLay += 2;
                    TimeDiff += abs(Flights[can[i]][4] - Flights[y][4]);
                    count++;
                }

                Afid[i] += sz;
                if(Afid[i] >= Affected[i].size()){resolved[i] = 1; break;}
            
        }
    }

    for(int i = 0; i < C; i++){
        if(resolved[i] == 1){continue;}
        for(auto p : s3[i]){
            //if(collisions[x] == 1){
                int x = p[0], y = p[1] , z = p[2];
                int sz = min(Flights[x][2],min(Flights[z][2], Flights[y][2]));
                for(int j = Afid[i]; j < min(Afid[i] + sz,(int)Affected[i].size()); j++){
                    allot[Affected[i][j]].push_back(3);
                    allot[Affected[i][j]].push_back(x);
                    allot[Affected[i][j]].push_back(y);
                    allot[Affected[i][j]].push_back(z);
                    AvgLay += 3;
                    TimeDiff += abs(Flights[can[i]][4] - Flights[z][4]);
                    count++;
                }

                Afid[i] += sz;
                if(Afid[i] >= Affected[i].size()){resolved[i] = 1; break;}
            
        }
    }

    fstream fout;
    fout.open("allot.csv", ios::out);
    for(auto p : allot){
        fout << p.first << ",";
        int i = p.second.size();
        for(int x : p.second){fout << x; if(--i != 0) fout<<",";}
        fout << endl;
    }
    fout.close();

    fout.open("stats.csv", ios::out);
    fout << "Affected,Reallocated,AvgLay,TimeDiff,SolTime" <<endl;
    fout << Affc <<","<< count << "," << (AvgLay/count) << "," << (TimeDiff/count) << ",";

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    float SolTime = (float) duration.count();

    float TimeS = SolTime/1000;

    fout << TimeS;
    fout.close();
    return 0;
}