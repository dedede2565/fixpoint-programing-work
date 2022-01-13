/*
    work2.c
    このプログラムはN回連続で応答がなかったときにのみ故障した
    と判断する。復旧までの時間を計出力するプログラム。
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define error 200
#define N 3//応答がなかった回数を指定している。
int main(int argc, char *argv[]) {
    FILE *fp;
    int y, month, d, h, min, s;//それぞれ年, 月, 日付, 時, 分, 秒を格納する変数
    int k;
    int i = 1;
    int minspan, sspan;//出力用の変数
    int count[error];//応答がなかった回数をカウントする変数
    int c;
    char x;
    char address[16];//アドレスを格納する文字列
    char check[error][16];//アドレスが一致しているか確認するときに使うポインタ
    int minmemory[error], smemory[error];//エラーの日時を格納するポインタ

    fp = fopen("log.txt", "r");

    while(fscanf(fp, "%4d%2d%2d%2d%2d%2d,%[^,],%s", &y, &month, &d, &h, &min, &s, address, &x) != EOF) {
        for(k=0; k<i; k++) {
            c = strcmp(address, check[k]);
            if(c == 0 && x != 45 && count[k] >= N) {
                minspan = min - minmemory[k];
                sspan = s - smemory[k];
                if(sspan >= 0){
                    printf("故障しているサーバは%sであり、故障期間は%d分%d秒である。\n", address, minspan, sspan);
                } else {
                    minspan -= 1;
                    sspan += 60;
                    printf("故障しているサーバは%sであり、故障期間は%d分%d秒である。\n", address, minspan, sspan);
                }
                strcpy(check[k], "0");
                c = -1;
            }
            if(c == 0 && x == 45 && count[k] < N) {
                count[k]++;
            }
            if(c == 0 && x != 45 && count[k] < N) {//復帰をした時は回数をリセットする。
                count[k] = 0;
                strcpy(check[k], "0");
                c = -1;
            }
        }
        //分と秒のデータ保存
        if(c!=0 && x == 45) {//2回連続でエラーが生じた際にデータを更新しないようにしている。
            strcpy(check[i], address);
            minmemory[i] = min;
            smemory[i] = s;
            count[i] = 1;
            i++;
            i = i % error;
        }
    }
    fclose(fp);
    return 0;
}

/*
    コンパイルはgccを用いて行った。
*/
