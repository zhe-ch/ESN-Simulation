#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NET_SIZE 256

int main() {

    int i, j, t;
    FILE *ifp, *ofp;
    static double intWM[NET_SIZE][NET_SIZE];
    int cnt_hyperedge;
    int cnt_inner_conn[NET_SIZE];
    int cnt_inter_conn[NET_SIZE];
    int cnt_gain[NET_SIZE];
    int p, q, p_gain, q_gain;
    int tmpWM[NET_SIZE];
    struct timeval t1, t2, tr, tacc1, tacc2;

    // Read in intWM from "intWM.bin" file
    if (!(ifp = fopen("intWM.bin", "rb"))) {
        printf("File intWM.bin cannot be opened for read.\n");
        return -1;
    }
    for (i = 0; i < NET_SIZE; ++i)
        fread(intWM[i], 8, NET_SIZE, ifp);
    fclose(ifp);

    // Open "output.txt" file for recording
    if (!(ofp = fopen("output.txt", "w"))) {
        printf("File output.txt cannot be opened for write.\n");
        return -1;
    }

    gettimeofday(&t1, NULL);

    for (t = 0; t < 300; ++t) {

        for (i = 0; i < NET_SIZE; ++i) {
            cnt_inner_conn[i] = 0;
            cnt_inter_conn[i] = 0;
            cnt_gain[i] = 0;
        }

        for (i = 0; i < (NET_SIZE>>1); ++i) {
            for (j = 0; j < (NET_SIZE>>1); ++j)
                if ((intWM[i][j] != 0) && (i != j))
                    cnt_inner_conn[i]++;
            for (j = (NET_SIZE>>1); j < NET_SIZE; ++j)
                if (intWM[i][j] != 0)
                    cnt_inter_conn[i]++;
        }
        for (i = (NET_SIZE>>1); i < NET_SIZE; ++i) {
            for (j = 0; j < (NET_SIZE>>1); ++j)
                if (intWM[i][j] != 0)
                    cnt_inter_conn[i]++;
            for (j = (NET_SIZE>>1); j < NET_SIZE; ++j)
                if ((intWM[i][j] != 0) && (i != j))
                    cnt_inner_conn[i]++;
        }

        for (i = 0; i < NET_SIZE; ++i)
            cnt_gain[i] = cnt_inter_conn[i] - cnt_inner_conn[i];

        p = 0;
        p_gain = cnt_gain[p];
        for (i = 0; i < (NET_SIZE>>1); ++i) {
            if (cnt_gain[i] > p_gain) {
                p = i;
                p_gain = cnt_gain[p];
            }
        }

        q = 0;
        q_gain = cnt_gain[q];
        for (i = (NET_SIZE>>1); i < NET_SIZE; ++i) {
            if (cnt_gain[i] > q_gain) {
                q = i;
                q_gain = cnt_gain[q];
            }
        }
        
        if (p_gain + q_gain > 0) {
            for (i = 0; i < NET_SIZE; ++i) {
                tmpWM[i] = intWM[p][i];
                intWM[p][i] = intWM[q][i];
                intWM[q][i] = tmpWM[i];
            }
            for (i = 0; i < NET_SIZE; ++i) {
                tmpWM[i] = intWM[i][p];
                intWM[p][i] = intWM[q][i];
                intWM[q][i] = tmpWM[i];
            }
        }
        else {
            break;
        }
    }

    gettimeofday(&t2, NULL);
    timersub(&t1, &t2, &tr);

    printf("Execute time: %.5f sec\n", -tr.tv_sec-(double)tr.tv_usec/1000000.0);
    
    cnt_hyperedge = 0;
    for (i = 0; i < NET_SIZE; ++i)
        cnt_hyperedge += cnt_inter_conn[i];
    printf("%d: %d\n", (t+1), cnt_hyperedge);

    printf("Processing complete.\n");

    fclose(ofp);

    return 0;
}
