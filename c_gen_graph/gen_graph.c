#include <stdio.h>
#include <stdlib.h>

#define NET_SIZE 1024

int main() {

    int i, j;
    FILE *ifp, *ofp;
    static double intWM[NET_SIZE][NET_SIZE];
    static int graph[NET_SIZE*NET_SIZE+1][2];
    int cnt_edge = 0;

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

    for (i = 0; i < NET_SIZE; ++i) {
        for (j = 0; j < NET_SIZE; ++j) {
            if (intWM[i][j] != 0) {
                cnt_edge++;
                graph[cnt_edge][0] = i + 1;
                graph[cnt_edge][1] = j + 1;
            }
        }
    }

    graph[0][0] = cnt_edge;
    graph[0][1] = NET_SIZE;

    for (i = 0; i < (cnt_edge+1); ++i)
        fprintf(ofp, "%d %d\n", graph[i][0], graph[i][1]);

    printf("Processing complete.\n");

    fclose(ofp);

    return 0;
}
