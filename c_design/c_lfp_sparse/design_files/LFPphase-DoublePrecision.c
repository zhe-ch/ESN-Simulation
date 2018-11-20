#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

int main() {
	
    FILE *ifp, *ofp;
    struct timeval t1, t2, tr;

    int i, j, t;
    double inWM[500];
    double ofbWM[2][500];
    double intWM[500][500];
    double outWM[501][2];

    int initialRunlength = 640;
    int plotRunlength = 3200*6*6;
    int samplelength = initialRunlength + plotRunlength;
    int offs = 100000;

    double sampleinput[samplelength];
    double sampleout[samplelength][2];
   
    double inputscaling = 0.125;
    int inputshift = 0;
    double teacherscaling[2] = {0.25, 0.25};
    int teachershift[2] = {0, 0};
 
    double in;
    double teach[2];

    double totalstate[503];
    double internalState[500];
    double netOut[2];

    // Read in inWM from "inWM.bin" file
    if (!(ifp = fopen("inWM.bin", "rb"))) {
        printf("File inWM.bin cannot be opened for read.\n");
        return -1;
    }
    fread(inWM, 8, 500, ifp);
    fclose(ifp);

    // Read in ofbWM from "ofbWM.bin" file
    if (!(ifp = fopen("ofbWM.bin", "rb"))) {
        printf("File ofbWM.bin cannot be opened for read.\n");
        return -1;
    }
    fread(ofbWM[0], 8, 500, ifp);
    fread(ofbWM[1], 8, 500, ifp);
    fclose(ifp);

    // Read in intWM from "intWM.bin" file
    if (!(ifp = fopen("intWM.bin", "rb"))) {
        printf("File intWM.bin cannot be opened for read.\n");
        return -1;
    }
    for (i = 0; i < 500; ++i)
        fread(intWM[i], 8, 500, ifp);
    fclose(ifp);

    // Read in outWM from "outWM.bin" file
    if (!(ifp = fopen("outWM.bin", "rb"))) {
        printf("File outWM.bin cannot be opened for read.\n");
        return -1;
    }
    for (i = 0; i < 501; ++i)
        fread(outWM[i], 8, 2, ifp);
    fclose(ifp);

    // Read in sample input from "sampleinput.bin" file
    if (!(ifp = fopen("sampleinput.bin", "rb"))) {
        printf("File sampleinput.bin cannot be opened for read.\n");
        return -1;
    }
    fseek(ifp, (offs*8), SEEK_SET);
    fread(sampleinput, 8, samplelength, ifp);
    fclose(ifp);

    // Read in sample out from "sampleout.bin" file
    if (!(ifp = fopen("sampleout.bin", "rb"))) {
        printf("File sampleout.bin cannot be opened for read.\n");
        return -1;
    }
    fseek(ifp, (offs*8*2), SEEK_SET);
    for (i = 0; i < samplelength; ++i)
        fread(sampleout[i], 8, 2, ifp);
    fclose(ifp);

    // Open "sampleresult.bin" file for sample result write back
    if (!(ofp = fopen("sampleresult.bin", "wb"))) {
        printf("File sampleresult.bin cannot be opened for write.\n");
        return -1;
    }

    for (i = 0; i < 503; ++i)
        totalstate[i] = 0;

    gettimeofday(&t1, NULL);
    for (t = 0; t < (initialRunlength + plotRunlength); ++t) {
        if (t % 10000 == 0) {
            gettimeofday(&t2, NULL);
            timersub(&t1, &t2, &tr);
            printf("Execute time: %.2f sec\n", -tr.tv_sec-(double)tr.tv_usec/1000000.0);
            
            printf("Iteration %d: t = %d\n", (t/10000), t);
        
            gettimeofday(&t1, NULL);
        }

        in = inputscaling * sampleinput[t] + inputshift;
        teach[0] = teacherscaling[0] * sampleout[t][0] + teachershift[0];
        teach[1] = teacherscaling[1] * sampleout[t][1] + teachershift[1];

        totalstate[500] = in;

        for (i = 0; i < 500; ++i) {
            internalState[i] = 0;
            for (j = 0; j < 500; ++j)
                internalState[i] += intWM[j][i] * totalstate[j];
            internalState[i] += inWM[i] * totalstate[500];
            internalState[i] += ofbWM[0][i] * totalstate[501];
            internalState[i] += ofbWM[1][i] * totalstate[502];
            internalState[i] = tanh(internalState[i]);
        }

        netOut[0] = 0;
        netOut[1] = 0;
        for (i = 0; i < 500; ++i) {
            netOut[0] += outWM[i][0] * internalState[i];
            netOut[1] += outWM[i][1] * internalState[i];
            totalstate[i] = internalState[i];
        }
        netOut[0] += outWM[500][0] * in;
        netOut[1] += outWM[500][1] * in;
        netOut[0] = tanh(netOut[0]);
        netOut[1] = tanh(netOut[1]);

        fwrite(&in, 8, 1, ofp);
        fwrite(netOut, 8, 2, ofp);
        
        if (t <= initialRunlength) {
            totalstate[501] = teach[0];
            totalstate[502] = teach[1];
        }
        else {
            totalstate[501] = netOut[0];
            totalstate[502] = netOut[1];
        }
    }

    /*
    printf("%f\n", sampleout[0][0]);
    printf("%f\n", sampleout[1][0]);
    printf("%f\n", sampleout[2][0]);
    printf("%f\n", sampleout[0][1]);
    printf("%f\n", sampleout[1][1]);
    printf("%f\n", sampleout[2][1]);
    */

    fclose(ofp);

    printf("Processing complete.\n");

    return 0;
}
