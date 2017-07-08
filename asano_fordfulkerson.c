// Ford-Fulkersonの最大フローアルゴリズムのプログラム fordfulkerson.c
#include <stdio.h>
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
   while (afwd != 0 && rescap[afwd] == 0) afwd = edgenext[afwd];
   while (t_found == false && afwd != 0) {// 増加パスはまだ発見されいない限り
      w = head[afwd];
      if (path[w] == unvisited) {// wが未訪問であったらwへ前進
         abk = afwd+1; // abkはafwdの逆向きの辺
         if (afwd % 2 ==0) abk = afwd-1;
         path[w] = abk; // afwdの逆向きの辺abkをpath[w]に記憶する
         if (w != t) dfs(w,path); // wからの深さ優先探索へ進む
         else {// w == t なので増加パスPが見つかった
            t_found=true; // dfs(v,path)の強制終了へ
         }
      }
//      else {// path[w] != unvisited)なので
//            // wからは増加パスでtへは到達できないことがすでにわかっている
//         afwd = edgenext[afwd]; //vを始点とする次の辺に移る
//      }
      if (t_found == false) { // dfs(v,path)が強制終了になっていないときのみ
          afwd = edgenext[afwd]; //次の辺に移る
          while (afwd != 0 && rescap[afwd] == 0) afwd = edgenext[afwd];
      }
   }
}
void augmentation(int path[]){
   int afwd, abk, v, w, delta, cap;
   t_found=false;
   for (v = 1; v <= n; v++) path[v] = unvisited;
   path[s]=0;
   dfs(s,path); //sからの深さ優先探索でtへの増加パスを探す
   if (t_found == true) {// 増加パスPの辺を逆にたどりdelta=Δ(P)を求める
      counter++;
      abk=path[t]; //abkはPの最後の辺の逆向きの辺でtは辺abkの始点
      afwd = abk+1; 
      if (abk % 2 == 0) afwd = abk-1; // afwdはPの最後の辺でtはafwdの終点
      delta = rescap[afwd]; 
      v = head[abk]; // vはafwdの始点
      while (v != s) {// sに到達するまでPを逆にたどる
         abk = path[v]; // abkはPの辺の逆向きの辺
         v = head[abk]; 
         afwd = abk+1; 
         if (abk % 2 == 0) afwd = abk-1; // afwdはPの辺
         if (delta > rescap[afwd]) delta = rescap[afwd];
         // deltaは最終的にはdelta=Δ(P)となる
      }
      printf("\n");
      printf("Δ（P%d)= %d\n", counter,delta);
      v = t;
      while (v != s) {// 増加パスに沿ってdelta=Δ(P)だけフローを送り込む
         abk = path[v]; // abkはPの辺の逆向きの辺
         v = head[abk];
         afwd = abk+1; 
         if (abk % 2 == 0) afwd = abk-1; // afwdはPの辺
         cap = rescap[afwd]+rescap[abk];
         rescap[afwd] = rescap[afwd]-delta;
         rescap[abk] = rescap[abk]+delta;
         if (afwd % 2 ==0) {// afwdはNの本来の辺
             printf("辺%2d  始点%2d  終点%2d  容量%2d   f(%2d):%2d --> %2d\n", 
               afwd/2, head[abk], head[afwd], cap, afwd/2, 
               rescap[abk]-delta, rescap[abk]);
         }
         else {// afwdはNにはなかった仮想的な辺で本来の辺abkの逆向きの辺
             printf("辺%2d  始点%2d  終点%2d  容量%2d   f(%2d):%2d --> %2d\n", 
               abk/2, head[afwd], head[abk], cap, abk/2, 
               rescap[afwd]+delta, rescap[afwd]);
         }
      }
   }
}
int main(void){
int path[vmaxsize+1];
   t_found = true; // 増加パスがある可能性があるのでtrueに設定
   flow_network_input(); 
   // ネットワークの辺数m，点数n，入口s，出口t，各辺の始点，終点，容量が決定される
   flow_incidence_list_construct(); // 各点を始点とする接続辺リストが構成される
   counter=0; // 発見された増加パスの個数は0個に初期設定
   while (t_found) {// 
      augmentation(path); // t_foundをfalseに設定して増加パスを探し，増加パスPが
                          // 発見されたらtrueになり，Pに沿ってフローを流す
   }
   max_flow_output();
   min_cut_output(path);
   return 0;
}
