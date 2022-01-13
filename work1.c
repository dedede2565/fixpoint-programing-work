/*
    work1.c
    このプログラムは、ログが保存されているファイルlog.txtを読み込んで
    応答がなかったとき故障していると判断し、応答が再開されるまでの
    時間を計算するものである。
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define error 200
int main(int argc, char *argv[]) {
    FILE *fp;
    int y, month, d, h, min, s;
    int k;
    int i = 1;
    int minspan, sspan;
    int c;
    char x;
    char address[16];//アドレスを格納する文字列
    char check[error][16];//アドレスが一致しているか確認するときに使うポインタ
    int minmemory[error], smemory[error];//エラーの日時を格納するポインタ

    fp = fopen("log.txt", "r");

    while(fscanf(fp, "%4d%2d%2d%2d%2d%2d,%[^,],%s", &y, &month, &d, &h, &min, &s, address, &x) != EOF) {
        for(k=0; k<i; k++) {
            c = strcmp(address, check[k]);
            if(c == 0 && x != 45) {//45は-の文字コード
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
            }
        }
        
        if(c != 0 && x == 45) {//2回連続でエラーが生じた際にデータを更新しないようにしている。
            strcpy(check[i], address);
            minmemory[i] = min;
            smemory[i] = s;
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
