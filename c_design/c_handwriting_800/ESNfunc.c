#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define NET_SIZE 800
#define IN_SIZE 3
#define OUT_SIZE 2
#define SPARSE_BOUND 105

float lut[256] = {
	0, 0.0099996667, 0.0199973338, 0.0299910032, 0.0399786803, 0.0499583750, 0.0599281035, 0.0698858903, 0.0798297691, 
	0.0897577847, 0.0996679946, 0.1095584702, 0.1194272985, 0.1292725836, 0.1390924479, 0.1488850336, 0.1586485043, 
	0.1683810459, 0.1780808681, 0.1877462059, 0.1973753202, 0.2069664997, 0.2165180615, 0.2260283523, 0.2354957495, 
	0.2449186624, 0.2542955326, 0.2636248355, 0.2729050806, 0.2821348127, 0.2913126125, 0.3004370971, 0.3095069212, 
	0.3185207769, 0.3274773948, 0.3363755443, 0.3452140341, 0.3539917125, 0.3627074676, 0.3713602279, 0.3799489623, 
	0.3884726802, 0.3969304320, 0.4053213087, 0.4136444422, 0.4218990053, 0.4300842114, 0.4381993148, 0.4462436102, 
	0.4542164327, 0.4621171573, 0.4699451989, 0.4777000122, 0.4853810906, 0.4929879667, 0.5005202112, 0.5079774329, 
	0.5153592780, 0.5226654297, 0.5298956075, 0.5370495670, 0.5441270989, 0.5511280285, 0.5580522156, 0.5648995528, 
	0.5716699661, 0.5783634130, 0.5849798829, 0.5915193954, 0.5979820005, 0.6043677771, 0.6106768328, 0.6169093029, 
	0.6230653496, 0.6291451614, 0.6351489524, 0.6410769612, 0.6469294504, 0.6527067060, 0.6584090360, 0.6640367703, 
	0.6695902596, 0.6750698748, 0.6804760061, 0.6858090622, 0.6910694698, 0.6962576727, 0.7013741309, 0.7064193204, 
	0.7113937318, 0.7162978702, 0.7211322541, 0.7258974148, 0.7305938961, 0.7352222529, 0.7397830513, 0.7442768674, 
	0.7487042870, 0.7530659049, 0.7573623242, 0.7615941560, 0.7657620182, 0.7698665359, 0.7739083399, 0.7778880666, 
	0.7818063576, 0.7856638590, 0.7894612210, 0.7931990971, 0.7968781442, 0.8004990218, 0.8040623914, 0.8075689166, 
	0.8110192621, 0.8144140938, 0.8177540780, 0.8210398813, 0.8242721703, 0.8274516110, 0.8305788685, 0.8336546070, 
	0.8366794891, 0.8396541757, 0.8425793257, 0.8454555957, 0.8482836400, 0.8510641097, 0.8537976532, 0.8564849155, 
	0.8591265382, 0.8617231593, 0.8642754128, 0.8667839288, 0.8692493331, 0.8716722472, 0.8740532879, 0.8763930675, 
	0.8786921934, 0.8809512680, 0.8831708889, 0.8853516482, 0.8874941329, 0.8895989244, 0.8916665990, 0.8936977272, 
	0.8956928738, 0.8976525982, 0.8995774536, 0.9014679878, 0.9033247426, 0.9051482536, 0.9069390510, 0.9086976584, 
	0.9104245939, 0.9121203692, 0.9137854901, 0.9154204563, 0.9170257614, 0.9186018929, 0.9201493321, 0.9216685544, 
	0.9231600290, 0.9246242190, 0.9260615814, 0.9274725673, 0.9288576215, 0.9302171829, 0.9315516846, 0.9328615534, 
	0.9341472104, 0.9354090706, 0.9366475432, 0.9378630317, 0.9390559335, 0.9402266404, 0.9413755385, 0.9425030081, 
	0.9436094241, 0.9446951554, 0.9457605656, 0.9468060128, 0.9478318497, 0.9488384234, 0.9498260757, 0.9507951432, 
	0.9517459572, 0.9526788437, 0.9535941237, 0.9544921131, 0.9553731226, 0.9562374581, 0.9570854206, 0.9579173060, 
	0.9587334055, 0.9595340058, 0.9603193885, 0.9610898309, 0.9618456054, 0.9625869801, 0.9633142186, 0.9640275801, 
	0.9647273193, 0.9654136869, 0.9660869290, 0.9667472878, 0.9673950013, 0.9680303033, 0.9686534239, 0.9692645889, 
	0.9698640204, 0.9704519366, 0.9710285520, 0.9715940773, 0.9721487194, 0.9726926818, 0.9732261643, 0.9737493633, 
	0.9742624716, 0.9747656786, 0.9752591705, 0.9757431300, 0.9762177367, 0.9766831669, 0.9771395937, 0.9775871874, 
	0.9780261147, 0.9784565399, 0.9788786239, 0.9792925249, 0.9796983982, 0.9800963963, 0.9804866688, 0.9808693627, 
	0.9812446223, 0.9816125893, 0.9819734027, 0.9823271991, 0.9826741124, 0.9830142743, 0.9833478138, 0.9836748577, 
	0.9839955304, 0.9843099539, 0.9846182482, 0.9849205309, 0.9852169173, 0.9855075208, 0.9857924526, 0.9860718216, 
	0.9863457352, 0.9866142982, 0.9868776137, 0.9871357831, 0.9873889054, 0.9876370782, 0.9878803970};

int main() {
	
    FILE *ifp, *ofp, *tfp;
    struct timeval t1, t2, tr, tacc1, tacc2;

    int i, j, t;
    double inWMd[IN_SIZE][NET_SIZE];
    //double ofbWMd[2][500];
    static double intWMd[NET_SIZE][NET_SIZE];
    double outWMd[OUT_SIZE][NET_SIZE];

    float inWM[IN_SIZE][NET_SIZE];
    //float ofbWM[2][500];
    static float intWM[NET_SIZE][NET_SIZE];
    float outWM[OUT_SIZE][NET_SIZE];

    int initialRunlength = 0;
    int plotRunlength = 50100; // 3200*6*6;
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

    unsigned char xState_entry;
    //unsigned char netOut0_entry;
    //unsigned char netOut1_entry;

    float tc_factor = 0.10;

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

    static double intWMdT[NET_SIZE][NET_SIZE];
    static float intWMT[NET_SIZE][NET_SIZE];
    for (i = 0; i < NET_SIZE; ++i)
        for (j = 0; j < NET_SIZE; ++j) {
            intWMdT[i][j] = intWMd[j][i];
            intWMT[i][j] = intWM[j][i];
        }

    // Read in outWM from "outWM.bin" file
    if (!(ifp = fopen("outWM.bin", "rb"))) {
        printf("File outWM.bin cannot be opened for read.\n");
        return -1;
    }
    for (i = 0; i < OUT_SIZE; ++i)
        fread(outWMd[i], 8, NET_SIZE, ifp);
    fclose(ifp);
    for (i = 0; i < OUT_SIZE; ++i)
        for (j = 0; j < NET_SIZE; ++j)
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

    float intWMS[NET_SIZE][NET_SIZE] = {};
    unsigned short intWMS_j[NET_SIZE][NET_SIZE] = {};
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
        xState[i] = 0;  //rand()/(RAND_MAX + 1.0)*2-1;
        rState[i] = 0;
    }

    gettimeofday(&t1, NULL);

    for (t = 0; t < samplelength; ++t) { 
        if (t % 50000 == 0) {
            gettimeofday(&t2, NULL);
            timersub(&t1, &t2, &tr);

            printf("Execute time: %.5f sec\n", -tr.tv_sec-(double)tr.tv_usec/1000000.0);
            printf("Iteration %d: t = %d\n", (t/10000), t);

            gettimeofday(&t1, NULL);
        }

        for (i = 0; i < IN_SIZE; ++i)
            in[i] = 0;

        if (t == 0)
            in[0] = 1;

        if (t >= 1000 && t < 1050)
            in[2] = 2;

        for (i = 0; i < IN_SIZE; ++i)
            uState[i] = in[i];
        
        for (i = 0; i < NET_SIZE; ++i) {
            xAccState[i] = 0;  
            for (j = 0; j < SPARSE_BOUND; ++j) {
                xAccState[i] += intWMS[i][j] * rState[intWMS_j[i][j]];
            }
        }

        for (i = 0; i < NET_SIZE; ++i) {
            for (j = 0; j < IN_SIZE; ++j)
                xAccState[i] += inWM[j][i] * uState[j];

            xState[i] += (xAccState[i] - xState[i]) * tc_factor;

            //if (xState[i] > max_tanh_x)
            //    max_tanh_x = xState[i];
            //if (xState[i] < min_tanh_x)
            //    min_tanh_x = xState[i];

            if (xState[i] > 0) {
                if (xState[i] * 100 > 255) {
                    xState_entry = 255;
                    //rState[i] = 1;
                }
                else {
                    xState_entry = (unsigned char) (xState[i] * 100);
                }
                rState[i] = lut[xState_entry];
            }
            else {
                if (-xState[i] * 100 > 255) {
                    xState_entry = 255;
                    //rState[i] = -1;
                }
                else {
                    xState_entry = (unsigned char) (-xState[i] * 100);
                }
                rState[i] = -lut[xState_entry];
            }

            //rState[i] = tanhf(xState[i]);
        }

        netOut[0] = 0;
        netOut[1] = 0;

        for (i = 0; i < NET_SIZE; ++i) {
            for (j = 0; j < OUT_SIZE; ++j)
                netOut[j] += outWM[j][i] * rState[i];
        }

        fwrite(netOut, 4, OUT_SIZE, ofp);
        //fprintf(tfp, "%f, %f\n", netOut[0], -netOut[1]);
    }


    //float ti;
    //for (ti = -1; ti < 1; ti = ti + 0.01)
    //    fprintf(tfp, "%f, %f\n", ti, tanh(ti));

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
    /*for (t = 0; t < NET_SIZE; ++t) {
        fprintf(tfp, "\t{");
        ini = 0;
        for (j = 0; j < NET_SIZE; ++j) {
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
    /*for (t = 0; t < NET_SIZE; ++t) {
        fprintf(tfp, "\t{");
        ini = 0;
        for (j = 0; j < NET_SIZE; ++j) {
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
    /*for (t = 0; t < IN_SIZE; ++t) {
        for (i = 0; i < NET_SIZE/10; ++i) {
            fprintf(tfp, "\t");
            for (j = 0; j < 10; ++j) {
                fprintf(tfp, "%f,", inWM[t][id_reorder[i*10+j]]);
            }
            fprintf(tfp, "\n");
        }
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
    /*for (t = 0; t < NET_SIZE/5; ++t) {
        fprintf(tfp, "\t");
        for (j = 0; j < 5; ++j) {
            fprintf(tfp, "%f,", outWM[0][id_reorder[t*5+j]]);
            fprintf(tfp, "%f,", outWM[1][id_reorder[t*5+j]]);
        }
        fprintf(tfp, "\n");
    }*/
    //fprintf(tfp, "%f,%f\n", outWM[500][0], outWM[500][1]);

    //printf("Divided Groups: %d, %d, %d, %d, %d\n", cnt[159], cnt[319], cnt[479], cnt[639], cnt[799]); 

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

/*
    for (i = 0; i < 32; ++i) {
        fprintf(tfp, "\t");
        for (j = 0; j < 8; ++j) {
            fprintf(tfp, "%.10f, ", tanh((i*8+j+1)*0.01));
        }
        fprintf(tfp, "\n");
    }
*/
    //printf("%f\n", rand()/(RAND_MAX + 1.0)*2-1);
   
    printf("Processing complete.\n");

    return 0;
}
