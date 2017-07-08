// フローのライブラリプログラム FlowLibrary.h
int head[e2maxsize+1], rescap[e2maxsize+1];
int edgefirst[vmaxsize+1], edgenext[e2maxsize+1];
int m, n, s, t;
void flow_network_input(void){
   int e, efwd, ebk;
    printf("入力するデータは有向ネットワークのデータです\n");
   printf("有向ネットワークの点数nと辺数mを入力してください\n");
   scanf("%d %d", &n, &m);
   printf("m本の各辺の始点と終点および容量を入力してください\n");
   for (e = 1; e <= m; e++) {// 入口を終点とする辺eは存在しないと仮定
      efwd = e * 2; ebk = efwd-1;// efwdはeに対応しebkは逆向きの仮想的辺に対応
      scanf("%d %d %d", &head[ebk], &head[efwd], &rescap[efwd]);
      rescap[ebk]=0;
   }
   printf("入口sと出口tを入力してください\n");
   scanf("%d %d", &s, &t);
   printf("有向ネットワークの点数n=%d，辺数 m=%d \n",n,m);
}
void flow_incidence_list_construct(void){// 接続辺リストを構成する関数
   int e, efwd, ebk, v, v1, v2;
   for (v = 1; v <= n; v++) edgefirst[v] = 0; // 空のリストに初期設定
   for (e = m; e >= 1; e--) {// 辺eを逆順に見ていきながらリストの先頭に挿入
      efwd = e * 2; ebk = efwd-1; // efwdはeに対応しebkは逆向きの仮想的辺に対応
      v1 = head[ebk];  edgenext[efwd] = edgefirst[v1]; edgefirst[v1] = efwd;
      v2 = head[efwd]; edgenext[ebk] = edgefirst[v2];  edgefirst[v2] = ebk;
   }
}
void flow_incidence_list_output(void){// 接続辺リストを出力する関数
int a, k, v;
    printf("\n各点を始点とする辺のリスト\n");
    printf("点v: vを始点とする辺（その終点）の並び\n");
    for (v = 1; v <= n; v++) {// 各点vを始点とする辺のリストの出力
        printf("%3d:  ", v);
        a = edgefirst[v];
        k = 0;
        while (a != 0) {
            printf("%3d (%3d)  ", a, head[a]);
            k++; if (k % 10 == 0) printf("\n    ");
            a = edgenext[a];
        }
        printf("\n");
    }
}
void max_flow_output(void){
   int e, cap, value;
   printf("\n入口%dから出口%dへの最大フローf\n", s, t);
   value=0;
   for (e = 1; e <= m; e++) {
      cap = rescap[2*e-1]+rescap[2*e];
      printf("辺%2d  始点%2d  終点%2d  容量%2d    f(%2d)=%2d \n", 
               e, head[2*e-1], head[2*e], cap, e, rescap[2*e-1]);
      if (head[2*e-1] == s) value=value+rescap[2*e-1];
   }
   printf("最大フローfの流量 = %2d\n", value);
}
void min_cut_output(int path[]){
   int e, cap, capsum, v;
   printf("\n最小%d-%dカットX\n", s, t);
   printf("X={");
   for (v = 1; v <= n; v++) {
      if (path[v] != unvisited) printf("%2d ",v);
   }
   printf("}\n");
   capsum=0;
   for (e = 1; e <= m; e++) {
      if (path[head[2*e-1]] != unvisited && path[head[2*e]] == unvisited) {
          cap= rescap[2*e-1]+rescap[2*e];
          printf("辺%2d  始点%2d  終点%2d  容量%2d    f(%2d)=%2d \n", 
               e, head[2*e-1], head[2*e], cap, e, rescap[2*e-1]);
       capsum=capsum+cap;
      }
   }
   printf("最小%d-%dカットXの容量 = %2d\n",s,t,capsum);
}
