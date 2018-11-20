#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define NET_SIZE 800
#define IN_SIZE 3
#define OUT_SIZE 2
#define SPARSE_BOUND 105

float lut[256] = {
    0,0.0015624987,0.0031249898,0.0046874657,0.0062499186,0.0078123411,0.0093747254,0.0109370639,0.012499349,
	0.0140615731,0.0156237286,0.0171858077,0.018747803,0.0203097068,0.0218715115,0.0234332094,0.024994793,
	0.0265562546,0.0281175866,0.0296787814,0.0312398314,0.0328007291,0.0343614668,0.0359220368,0.0374824318,
	0.0390426439,0.0406026657,0.0421624896,0.0437221079,0.0452815132,0.0468406979,0.0483996543,0.049958375,
	0.0515168523,0.0530750787,0.0546330468,0.0561907488,0.0577481774,0.0593053249,0.0608621839,0.0624187467,
	0.063975006,0.0655309542,0.0670865838,0.0686418873,0.0701968573,0.0717514861,0.0733057664,0.0748596907,
	0.0764132515,0.0779664414,0.0795192529,0.0810716785,0.082623711,0.0841753427,0.0857265663,0.0872773745,
	0.0888277597,0.0903777146,0.0919272318,0.093476304,0.0950249237,0.0965730836,0.0981207764,0.0996679946,
	0.1012147311,0.1027609784,0.1043067292,0.1058519763,0.1073967123,0.1089409299,0.1104846219,0.1120277811,
	0.1135704001,0.1151124717,0.1166539887,0.1181949438,0.1197353299,0.1212751397,0.122814366,0.1243530018,
	0.1258910397,0.1274284727,0.1289652935,0.1305014951,0.1320370704,0.1335720122,0.1351063134,0.136639967,
	0.1381729658,0.1397053028,0.141236971,0.1427679634,0.1442982728,0.1458278923,0.1473568149,0.1488850336,
	0.1504125415,0.1519393315,0.1534653968,0.1549907304,0.1565153253,0.1580391748,0.1595622719,0.1610846096,
	0.1626061813,0.16412698,0.1656469988,0.167166231,0.1686846698,0.1702023084,0.17171914,0.1732351578,
	0.1747503552,0.1762647253,0.1777782615,0.1792909571,0.1808028054,0.1823137997,0.1838239334,0.1853331999,
	0.1868415925,0.1883491046,0.1898557297,0.1913614612,0.1928662925,0.1943702171,0.1958732285,0.1973753202,
	0.1988764857,0.2003767185,0.2018760122,0.2033743604,0.2048717565,0.2063681944,0.2078636675,0.2093581694,
	0.210851694,0.2123442347,0.2138357854,0.2153263397,0.2168158913,0.2183044341,0.2197919617,0.2212784679,
	0.2227639466,0.2242483915,0.2257317966,0.2272141556,0.2286954625,0.230175711,0.2316548952,0.233133009,
	0.2346100464,0.2360860012,0.2375608675,0.2390346393,0.2405073106,0.2419788755,0.2434493281,0.2449186624,
	0.2463868725,0.2478539527,0.2493198969,0.2507846994,0.2522483545,0.2537108562,0.2551721988,0.2566323766,
	0.2580913838,0.2595492148,0.2610058638,0.2624613252,0.2639155934,0.2653686626,0.2668205274,0.268271182,
	0.269720621,0.2711688389,0.27261583,0.274061589,0.2755061102,0.2769493883,0.2783914179,0.2798321935,
	0.2812717097,0.2827099611,0.2841469425,0.2855826485,0.2870170737,0.288450213,0.289882061,0.2913126125,
	0.2927418622,0.2941698051,0.2955964359,0.2970217495,0.2984457407,0.2998684045,0.3012897357,0.3027097293,
	0.3041283803,0.3055456837,0.3069616344,0.3083762275,0.3097894581,0.3112013212,0.3126118119,0.3140209253,
	0.3154286567,0.3168350011,0.3182399538,0.3196435099,0.3210456648,0.3224464136,0.3238457517,0.3252436743,
	0.3266401769,0.3280352547,0.3294289031,0.3308211175,0.3322118934,0.3336012262,0.3349891113,0.3363755443,
	0.3377605207,0.3391440361,0.3405260859,0.3419066658,0.3432857714,0.3446633984,0.3460395423,0.3474141989,
	0.3487873639,0.350159033,0.351529202,0.3528978667,0.3542650228,0.3556306662,0.3569947928,0.3583573984,
	0.3597184788,0.3610780302,0.3624360483,0.3637925292,0.3651474688,0.3665008633,0.3678527085,0.3692030006,
	0.3705517358,0.37189891,0.3732445195,0.3745885604,0.3759310289,0.3772719212,0.3786112336};

int main() {
	
    FILE *ifp, *ofp, *tfp;
    struct timeval t1, t2, tr, tacc1, tacc2;

    int i, j, t;
    double inWMd[IN_SIZE][NET_SIZE];
    //double ofbWMd[2][500];
    double intWMd[NET_SIZE][NET_SIZE];
    double outWMd[NET_SIZE][OUT_SIZE];

    float inWM[IN_SIZE][NET_SIZE];
    //float ofbWM[2][500];
    float intWM[NET_SIZE][NET_SIZE];
    float outWM[NET_SIZE][OUT_SIZE];

    int initialRunlength = 0;
    int plotRunlength = 3000; // 3200*6*6;
    int samplelength = initialRunlength + plotRunlength;
    //int offs = 100000;

    //double sampleinputd[samplelength];
    //double sampleoutd[samplelength][2];

    //float sampleinput[samplelength];
    //float sampleout[samplelength][2];
   
    //float inputscaling = 0.125;
    //int inputshift = 0;
    //float teacherscaling[2] = {0.25, 0.25};
    //int teachershift[2] = {0, 0};
 
    float in[IN_SIZE];
    //float teach[2];

    //float totalstate[803];
    float uState[IN_SIZE];
    float xState[NET_SIZE];
    float rState[NET_SIZE];
    float xAccState[NET_SIZE];
    float netOut[OUT_SIZE];

    //float max_tanh_x = 0;
    //float min_tanh_x = 0;

    //unsigned char internalState_entry;
    //unsigned char netOut0_entry;
    //unsigned char netOut1_entry;

    float tc_factor = 0.1;

    // Read in inWM from "inWM.bin" file
    if (!(ifp = fopen("inWM.bin", "rb"))) {
        printf("File inWM.bin cannot be opened for read.\n");
        return -1;
    }
    for (i = 0; i < IN_SIZE; ++i)
        fread(inWMd[i], 8, NET_SIZE, ifp);
    fclose(ifp);
    for (i = 0; i < IN_SIZE; ++i)
        for (j = 0; j < NET_SIZE; ++j)
            inWM[i][j] = (float) inWMd[i][j];

    // Read in ofbWM from "ofbWM.bin" file
    /*
    if (!(ifp = fopen("ofbWM.bin", "rb"))) {
        printf("File ofbWM.bin cannot be opened for read.\n");
        return -1;
    }
    fread(ofbWMd[0], 8, 500, ifp);
    fread(ofbWMd[1], 8, 500, ifp);
    fclose(ifp);
    for (i = 0; i < 500; ++i) {
        ofbWM[0][i] = (float) ofbWMd[0][i];
        ofbWM[1][i] = (float) ofbWMd[1][i];
    }*/

    // Read in intWM from "intWM.bin" file
    if (!(ifp = fopen("intWM.bin", "rb"))) {
        printf("File intWM.bin cannot be opened for read.\n");
        return -1;
    }
    for (i = 0; i < NET_SIZE; ++i)
        fread(intWMd[i], 8, NET_SIZE, ifp);
    fclose(ifp);
    for (i = 0; i < NET_SIZE; ++i)
        for (j = 0; j < NET_SIZE; ++j)    
            intWM[i][j] = (float) intWMd[i][j];

    // Read in outWM from "outWM.bin" file
    if (!(ifp = fopen("outWM.bin", "rb"))) {
        printf("File outWM.bin cannot be opened for read.\n");
        return -1;
    }
    for (i = 0; i < NET_SIZE; ++i)
        fread(outWMd[i], 8, OUT_SIZE, ifp);
    fclose(ifp);
    for (i = 0; i < NET_SIZE; ++i)
        for (j = 0; j < OUT_SIZE; ++j)
            outWM[i][j] = (float) outWMd[i][j];

    // Read in sample input from "sampleinput.bin" file
    /*
    if (!(ifp = fopen("sampleinput.bin", "rb"))) {
        printf("File sampleinput.bin cannot be opened for read.\n");
        return -1;
    }
    fseek(ifp, (offs*8), SEEK_SET);
    fread(sampleinputd, 8, samplelength, ifp);
    fclose(ifp);
    for (i = 0; i < samplelength; ++i)
        sampleinput[i] = (float) sampleinputd[i];*/

    // Read in sample out from "sampleout.bin" file
    /*
    if (!(ifp = fopen("sampleout.bin", "rb"))) {
        printf("File sampleout.bin cannot be opened for read.\n");
        return -1;
    }
    fseek(ifp, (offs*8*2), SEEK_SET);
    for (i = 0; i < samplelength; ++i)
        fread(sampleoutd[i], 8, 2, ifp);
    fclose(ifp);
    for (i = 0; i < samplelength; ++i) {
        sampleout[i][0] = (float) sampleoutd[i][0];
        sampleout[i][1] = (float) sampleoutd[i][1];
    }*/

    // Open "sampleresult.bin" file for sample result write back
    if (!(ofp = fopen("sampleresult.bin", "wb"))) {
        printf("File sampleresult.bin cannot be opened for write.\n");
        return -1;
    }

    // Open "weight.txt" file for weight record
    if (!(tfp = fopen("weight.txt", "w"))) {
        printf("File weight.txt cannot be opened for write\n");
        return -1;
    }

    static float intWMS[NET_SIZE][NET_SIZE] = {};
    static unsigned short intWMS_j[NET_SIZE][NET_SIZE] = {};

    for (i = 0; i < NET_SIZE; ++i) {
        t = 0;
        for (j = 0; j < NET_SIZE; ++j) {
            if (intWM[i][j] != 0) {
                intWMS[i][t] = intWM[i][j];
                intWMS_j[i][t] = j;
                t++;
            }
        }
    }

    for (i = 0; i < IN_SIZE; ++i)
        uState[i] = 0;
    for (i = 0; i < NET_SIZE; ++i) {
        xState[i] = 0;
        rState[i] = 0;
    }

    for (t = 0; t < samplelength; ++t) { 
        for (i = 0; i < IN_SIZE; ++i)
            in[i] = 0;

        if (t == 0)
            in[0] = 1;

        if (t >= 500 && t < 505)
            in[0] = 1;

        for (i = 0; i < IN_SIZE; ++i)
            uState[i] = in[i];
        
        for (i = 0; i < NET_SIZE; ++i) {
            xAccState[i] = 0;  
            for (j = 0; j < NET_SIZE; ++j) {
                xAccState[i] += intWM[i][j] * rState[j];
            }
        }

        for (i = 0; i < NET_SIZE; ++i) {
            for (j = 0; j < IN_SIZE; ++j)
                xAccState[i] += inWM[j][i] * uState[j];

            xState[i] += (xAccState[i] - xState[i]) * tc_factor;
            rState[i] = tanhf(xState[i]);
        }

        netOut[0] = 0;
        netOut[1] = 0;

        for (i = 0; i < NET_SIZE; ++i) {
            for (j = 0; j < OUT_SIZE; ++j)
                netOut[j] += outWM[i][j] * rState[i];
        }
        fwrite(netOut, 4, OUT_SIZE, ofp);
        fprintf(tfp, "%f, %f\n", netOut[0], netOut[1]);
    }

    int ini;
    int tmp;
    int cnt[NET_SIZE];
    int id_reorder[NET_SIZE];
    int id_convert[NET_SIZE];

    // Count non-zero weight values.
    for (t = 0; t < NET_SIZE; ++t) {
        cnt[t] = 0;
        id_reorder[t] = t;
        for (j = 0; j < NET_SIZE; ++j) {
            if (intWM[t][j] != 0) {
                cnt[t]++;
            }
        }
    }

    // Reorder non-zero weight values.
    for (t = 0; t < NET_SIZE; ++t) {
        tmp = NET_SIZE;
        i = t;
        for (j = t; j < NET_SIZE; ++j) {
            if (cnt[j] < cnt[i])
                i = j;
        }
        tmp = cnt[t];
        cnt[t] = cnt[i];
        cnt[i] = tmp;
        tmp = id_reorder[t];
        id_reorder[t] = id_reorder[i];
        id_reorder[i] = tmp;
    }

    // Convert order exchange information.
    for (t = 0; t < NET_SIZE; ++t) {
        id_convert[id_reorder[t]] = t;
    }

    // Store non-zero weight values in order.
    /*for (t = 0; t < 800; ++t) {
        fprintf(tfp, "{");
        ini = 0;
        for (j = 0; j < 800; ++j) {
            if (intWM[id_reorder[t]][j] != 0) {
                if (ini == 0) {
                    fprintf(tfp, "%f", intWM[id_reorder[t]][j]);
                    ini = 1;
                }
                else 
                    fprintf(tfp, ",%f", intWM[id_reorder[t]][j]);
            }
        }
        fprintf(tfp, "},\n");
    }*/

    // Store non-zero weight column index in order.
    /*for (t = 0; t < 500; ++t) {
        fprintf(tfp, "{");
        ini = 0;
        for (j = 0; j < 500; ++j) {
            if (intWM[id_reorder[t]][j] != 0) {
                if (ini == 0) {
                    fprintf(tfp, "%d", id_convert[j]);
                    ini = 1;
                }
                else 
                    fprintf(tfp, ",%d", id_convert[j]);
            }
        }
        fprintf(tfp, "},\n");
    }*/

    // Store order information of the row index exchange.
    /*for (t = 0; t < 10; ++t) {
        for (j = 0; j < 50; ++j) {
            fprintf(tfp, "%d,", id_reorder[t*50+j]);
        }
        fprintf(tfp, "\n");
    }*/

    // Store conversion information of the row index exchange.
    /*for (t = 0; t < 10; ++t) {
        for (j = 0; j < 50; ++j) {
            fprintf(tfp, "%d,", id_convert[t*50+j]);
        }
        fprintf(tfp, "\n");
    }*/

    // Store inWM vector according to exchanged order.
    /*for (t = 0; t < 50; ++t) {
        for (j = 0; j < 10; ++j) {
            fprintf(tfp, "%f,", inWM[id_reorder[t*10+j]]);
        }
        fprintf(tfp, "\n");
    }*/

    // Store ofbWM array according to exchanged order.
    /*for (t = 0; t < 2; ++t) {
        for (i = 0; i < 50; ++i) {
            for (j = 0; j < 10; ++j) {
                fprintf(tfp, "%f,", ofbWM[t][id_reorder[i*10+j]]);
            }
            fprintf(tfp, "\n");
        }
    }*/

    // Store outWM array according to exchanged order.
    /*for (t = 0; t < 100; ++t) {
        for (j = 0; j < 5; ++j) {
            fprintf(tfp, "%f,", outWM[id_reorder[t*5+j]][0]);
            fprintf(tfp, "%f,", outWM[id_reorder[t*5+j]][1]);
        }
        fprintf(tfp, "\n");
    }
    fprintf(tfp, "%f,%f\n", outWM[500][0], outWM[500][1]);*/

    //printf("Five Groups: %d, %d, %d, %d, %d\n", cnt[99], cnt[199], cnt[299], cnt[399], cnt[499]); 

    /*
    printf("%f\n", sampleout[0][0]);
    printf("%f\n", sampleout[1][0]);
    printf("%f\n", sampleout[2][0]);
    printf("%f\n", sampleout[0][1]);
    printf("%f\n", sampleout[1][1]);
    printf("%f\n", sampleout[2][1]);
    */

//    fclose(ofp);
//    fclose(tfp);

    //printf("Maximum tanh() value is: %f\n", max_tanh_x);
    //printf("Minimum tanh() value is: %f\n", min_tanh_x);

    printf("Processing complete.\n");

    return 0;
}
