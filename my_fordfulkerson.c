// Ford-Fulkersonの最大フローアルゴリズムのプログラム fordfulkerson.c
#include <stdio.h>
#include <limits.h>
#define vmaxsize          100
#define emaxsize          400
#define e2maxsize         800
#define unvisited          -1
#define true                1
#define false               0
#include"FlowLibrary.h"
int t_found; // 増加パスが発見されるとtrueになる
int counter; // 発見された増加パスの個数
void dfs(int v, int path[]){// vからの深さ優先探索
    int afwd, abk, w;
    afwd = edgefirst[v];
    for(afwd = edgefirst[v]; afwd != 0; afwd = edgenext[afwd]){
        if(rescap[afwd] == 0) continue;
        w = head[afwd];
        if(path[w] == unvisited){
            abk = (afwd % 2 == 0) ? afwd - 1 : afwd + 1;
            path[w] = abk;
            if(w != t){
                dfs(w, path);
            }else{
                t_found = true;
                return;
            }
        }
    }
}
void augmentation(int path[]){
    int afwd, abk, v, w, delta, cap;
    t_found = false;
    for(v = 1; v <= n; v++) path[v] = unvisited;
    dfs(s, path);
    if(t_found == true){
        counter++;
        v = t;
        delta = INT_MAX;
        for(v = t; v != s; v = head[abk]){
            abk = path[v];
            afwd = (abk % 2 == 0) ? abk - 1 : abk + 1;
            if (delta > rescap[afwd]) delta = rescap[afwd];
        }

        printf("\n");
        printf("Δ（P%d)= %d\n", counter, delta);

        for(v = t; v != s; v = head[abk]){
            abk = path[v];
            afwd = (abk % 2 == 0) ? abk - 1 : abk + 1;
            cap = rescap[afwd] + rescap[abk];
            rescap[afwd] = rescap[afwd] - delta;
            rescap[abk] = rescap[abk] + delta;
            if(afwd % 2 == 0){
                printf("辺%2d  始点%2d  終点%2d  容量%2d   f(%2d):%2d --> %2d\n", afwd/2, head[abk], head[afwd], cap, afwd/2, rescap[abk] - delta, rescap[abk]);
            }else{
                printf("辺%2d  始点%2d  終点%2d  容量%2d   f(%2d):%2d --> %2d\n", abk/2, head[afwd], head[abk], cap, abk/2, rescap[afwd] + delta, rescap[afwd]);
            }
        }
    }
}
int main(void){
    int path[vmaxsize+1];
    flow_network_input(); 
    // ネットワークの辺数m，点数n，入口s，出口t，各辺の始点，終点，容量が決定される
    flow_incidence_list_construct(); // 各点を始点とする接続辺リストが構成される
    counter = 0; // 発見された増加パスの個数は0個に初期設定

    do{
        augmentation(path);
    }while(t_found);

    max_flow_output();
    min_cut_output(path);
    return 0;
}
