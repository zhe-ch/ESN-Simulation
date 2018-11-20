#include "ap_cint.h"
#include "ap_interfaces.h"

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

float arctan[256] = {
        0,0.2246881796,0.4493694487,0.674036898,0.8986836207,1.1233027141,1.3478872802,1.5724304277,1.7969252727,
        2.0213649404,2.2457425659,2.470051296,2.6942842899,2.9184347208,3.1424957771,3.3664606634,3.5903226021,
        3.8140748343,4.037710621,4.2612232445,4.4846060095,4.7078522444,4.930955302,5.1539085612,5.3767054281,
        5.5993393365,5.82180375,6.0440921622,6.2661980985,6.4881151165,6.7098368078,6.9313567983,7.15266875,
        7.3737663613,7.5946433686,7.8152935468,8.0357107105,8.2558887152,8.4758214577,8.6955028774,8.9149269571,
        9.134087724,9.3529792501,9.5715956538,9.7899311,10.0079798014,10.2257360192,10.4431940635,10.6603482945,
        10.8771931232,11.0937230116,11.309932474,11.5258160775,11.7413684425,11.9565842431,12.1714582086,12.3859851229,
        12.6001598261,12.8139772143,13.0274322405,13.2405199152,13.4532353065,13.6655735407,13.8775298032,14.0890993381,
        14.3002774492,14.5110595002,14.721440915,14.9314171781,15.1409838351,15.3501364924,15.5588708183,15.7671825425,
        15.9750674568,16.1825214152,16.389540334,16.5961201922,16.8022570311,17.0079469554,17.2131861323,17.4179707922,
        17.6222972287,17.8261617984,18.0295609213,18.2324910807,18.4349488229,18.6369307577,18.838433558,19.0394539598,
        19.2399887625,19.4400348282,19.6395890821,19.8386485122,20.0372101692,20.2352711664,20.4328286794,20.6298799461,
        20.8264222665,21.0224530022,21.2179695768,21.4129694749,21.6074502425,21.8014094864,21.994844874,22.1877541333,
        22.380135052,22.5719854777,22.7633033175,22.9540865376,23.1443331629,23.3340412767,23.5232090205,23.7118345933,
        23.8999162515,24.0874523086,24.2744411344,24.460881155,24.6467708519,24.8321087624,25.0168934781,25.2011236455,
        25.3847979648,25.5679151897,25.7504741273,25.932473637,26.1139126303,26.2947900706,26.4751049723,26.6548564006,
        26.8340434708,27.0126653479,27.1907212463,27.3682104289,27.5451322068,27.7214859388,27.8972710309,28.0724869359,
        28.2471331523,28.4212092245,28.5947147421,28.7676493388,28.9400126928,29.1118045255,29.2830246013,29.4536727269,
        29.6237487512,29.7932525641,29.9621840964,30.1305433194,30.2983302437,30.4655449195,30.6321874353,30.7982579181,
        30.9637565321,31.1286834787,31.2930389959,31.4568233576,31.6200368729,31.7826798863,31.9447527762,32.1062559551,
        32.2671898688,32.4275549958,32.5873518469,32.7465809647,32.905242923,33.0633383263,33.2208678093,33.3778320364,
        33.5342317012,33.690067526,33.8453402611,34.0000506846,34.1541996018,34.3077878447,34.4608162714,34.6132857656,
        34.7651972366,34.916551618,35.0673498679,35.2175929682,35.367281924,35.5164177634,35.6650015369,35.8130343166,
        35.9605171966,36.1074512918,36.2538377374,36.3996776894,36.5449723228,36.6897228325,36.8339304318,36.9775963527,
        37.120721845,37.2633081764,37.4053566314,37.5468685116,37.6878451347,37.8282878346,37.9681979607,38.1075768775,
        38.2464259645,38.3847466154,38.5225402381,38.6598082541,38.7965520983,38.9327732185,39.0684730751,39.2036531406,
        39.3383148997,39.4724598483,39.6060894938,39.7392053542,39.8718089581,40.0039018446,40.1354855622,40.2665616694,
        40.3971317337,40.5271973316,40.6567600483,40.7858214773,40.91438322,41.0424468859,41.1700140916,41.297086461,
        41.423665625,41.549753221,41.6753508929,41.8004602904,41.9250830694,42.049220891,42.1728754218,42.2960483335,
        42.4187413026,42.5409560099,42.6626941409,42.7839573849,42.9047474354,43.0250659891,43.1449147465,43.2642954111,
        43.3832096893,43.5016592905,43.6196459266,43.7371713115,43.8542371618,43.9708451957,44.0869971332,44.2026946959,
        44.3179396068,44.4327335901,44.547078371,44.6609756755,44.7744272304,44.8874347627,45};

/*DEFINE_INWM*/

/*DEFINE_OFBWM*/

/*DEFINE_OUTWM*/

/*DEFINE_INTWM*/

/*DEFINE_INTWM_J*/

void lfp_phase(
    volatile int24 sampleinput,
    volatile int max_val,
    volatile int min_val,
    volatile uint8 initial_en,
    volatile uint8 *out_ttl
    )
{
//#pragma HLS INTERFACE ap_ctrl_none port = return

    int i, j;
    int norm_sample;
    float sampleinput_f;

#pragma HLS array_partition variable=inWM cyclic factor=10
#pragma HLS array_partition variable=ofbWM dim=2 cyclic factor=10
#pragma HLS array_partition variable=intWM_j dim=1 cyclic factor=10
#pragma HLS array_partition variable=intWM dim=1 cyclic factor=10
#pragma HLS array_partition variable=outWM dim=1 cyclic factor=10

    float totalstate_500;
    float totalstate_501;
    float totalstate_502;
    float netOut[2];
    float netOutTemp[10][2];
#pragma HLS array_partition variable=netOutTemp complete dim=0
    float abs_x;
    float abs_y;
    float abs_angle;
    float out_angle;

    float totalstate[500];
    float internalState[500];
#pragma HLS array_partition variable=totalstate complete
#pragma HLS array_partition variable=internalState cyclic factor=10

    uint8 internalState_entry;
    uint8 netOut0_entry;
    uint8 netOut1_entry;
    uint8 arctan_entry;

    int24 sampleinput_buf[256];
    uint8 cur_pnt;
    int32 acc_sampleinput;

    if (initial_en == 1) {
        for (i = 0; i < 500; ++i) {
#pragma HLS pipeline
#pragma HLS unroll factor=500
            totalstate[i] = 0;
        }
        totalstate_500 = 0;
        totalstate_501 = 0;
        totalstate_502 = 0;
        for (i = 0; i < 256; ++i) {
#pragma HLS pipeline
            sampleinput_buf[i] = 0;
        }
        cur_pnt = 0;
    }
    else {
        sampleinput_buf[cur_pnt] = sampleinput;
        acc_sampleinput = 0;
        for (i = 0; i < 256; ++i) {
#pragma HLS pipeline
            acc_sampleinput += sampleinput_buf[i];
        }
        cur_pnt++;

        norm_sample = sampleinput - (acc_sampleinput / 256);
        sampleinput_f = ((float)(norm_sample - min_val)) / ((float)(max_val - min_val));

        totalstate_500 = sampleinput_f;

        for (i = 0; i < 500; ++i) {
#pragma HLS pipeline
#pragma HLS unroll factor=10
            internalState[i] = 0;
        }

        for (j = 0; j < /*LOOP_ITERATION_1*/; ++j) {
            for (i = 0; i < 100; ++i) {
#pragma HLS pipeline
#pragma HLS unroll factor=10
                internalState[i] += intWM[i][j] * totalstate[intWM_j[i][j]];
            }
        }

        for (j = 0; j < /*LOOP_ITERATION_2*/; ++j) {
            for (i = 100; i < 200; ++i) {
#pragma HLS pipeline
#pragma HLS unroll factor=10
                internalState[i] += intWM[i][j] * totalstate[intWM_j[i][j]];
            }
        }

        for (j = 0; j < /*LOOP_ITERATION_3*/; ++j) {
            for (i = 200; i < 300; ++i) {
#pragma HLS pipeline
#pragma HLS unroll factor=10
                internalState[i] += intWM[i][j] * totalstate[intWM_j[i][j]];
            }
        }

        for (j = 0; j < /*LOOP_ITERATION_4*/; ++j) {
            for (i = 300; i < 400; ++i) {
#pragma HLS pipeline
#pragma HLS unroll factor=10
                internalState[i] += intWM[i][j] * totalstate[intWM_j[i][j]];
            }
        }

        for (j = 0; j < /*LOOP_ITERATION_5*/; ++j) {
            for (i = 400; i < 500; ++i) {
#pragma HLS pipeline
#pragma HLS unroll factor=10
                internalState[i] += intWM[i][j] * totalstate[intWM_j[i][j]];
            }
        }

        for (i = 0; i < 500; ++i) {
#pragma HLS pipeline
#pragma HLS unroll factor=10
            internalState[i] += inWM[i] * totalstate_500;
            internalState[i] += ofbWM[0][i] * totalstate_501;
            internalState[i] += ofbWM[1][i] * totalstate_502;

            if (internalState[i] > 0) {
                internalState_entry = (uint8) (internalState[i] * 640);
                internalState[i] = lut[internalState_entry];
            }
            else {
                internalState_entry = (uint8) (-internalState[i] * 640);
                internalState[i] = -lut[internalState_entry];
            }
            totalstate[i] = internalState[i];
        }

        netOut[0] = 0;
        netOut[1] = 0;

        for (i = 0; i < 10; ++i) {
#pragma HLS pipeline
            netOutTemp[i][0] = 0;
            netOutTemp[i][1] = 0;
        }

        for (i = 0; i < 500; i = i+10) {
#pragma HLS pipeline
            netOutTemp[0][0] += outWM[i+0][0] * internalState[i+0];
            netOutTemp[1][0] += outWM[i+1][0] * internalState[i+1];
            netOutTemp[2][0] += outWM[i+2][0] * internalState[i+2];
            netOutTemp[3][0] += outWM[i+3][0] * internalState[i+3];
            netOutTemp[4][0] += outWM[i+4][0] * internalState[i+4];
            netOutTemp[5][0] += outWM[i+5][0] * internalState[i+5];
            netOutTemp[6][0] += outWM[i+6][0] * internalState[i+6];
            netOutTemp[7][0] += outWM[i+7][0] * internalState[i+7];
            netOutTemp[8][0] += outWM[i+8][0] * internalState[i+8];
            netOutTemp[9][0] += outWM[i+9][0] * internalState[i+9];

            netOutTemp[0][1] += outWM[i+0][1] * internalState[i+0];
            netOutTemp[1][1] += outWM[i+1][1] * internalState[i+1];
            netOutTemp[2][1] += outWM[i+2][1] * internalState[i+2];
            netOutTemp[3][1] += outWM[i+3][1] * internalState[i+3];
            netOutTemp[4][1] += outWM[i+4][1] * internalState[i+4];
            netOutTemp[5][1] += outWM[i+5][1] * internalState[i+5];
            netOutTemp[6][1] += outWM[i+6][1] * internalState[i+6];
            netOutTemp[7][1] += outWM[i+7][1] * internalState[i+7];
            netOutTemp[8][1] += outWM[i+8][1] * internalState[i+8];
            netOutTemp[9][1] += outWM[i+9][1] * internalState[i+9];
        }

        for (i = 0; i < 10; ++i) {
#pragma HLS pipeline
            netOut[0] += netOutTemp[i][0];
            netOut[1] += netOutTemp[i][1];
        }

        netOut[0] += outWM_500[0] * sampleinput_f;
        netOut[1] += outWM_500[1] * sampleinput_f;

        if (netOut[0] > 0) {
            netOut0_entry = (uint8) (netOut[0] * 640);
            netOut[0] = lut[netOut0_entry];
        }
        else {
            netOut0_entry = (uint8) (-netOut[0] * 640);
            netOut[0] = -lut[netOut0_entry];
        }
        if (netOut[1] > 0) {
            netOut1_entry = (uint8) (netOut[1] * 640);
            netOut[1] = lut[netOut1_entry];
        }
        else {
            netOut1_entry = (uint8) (-netOut[1] * 640);
            netOut[1] = -lut[netOut1_entry];
        }

        totalstate_501 = netOut[0];
        totalstate_502 = netOut[1];

        if (netOut[0] >= 0) {
            abs_x = netOut[0];
        }
        else {
            abs_x = -netOut[0];
        }
        if (netOut[1] >= 0) {
            abs_y = netOut[1];
        }
        else {
            abs_y = -netOut[1];
        }

        // Arctangent Function
        if (abs_x >= abs_y) {
            arctan_entry = (uint8) (abs_y * 255 / abs_x);
            abs_angle = arctan[arctan_entry];
        }
        else {
            arctan_entry = (uint8) (abs_x * 255 / abs_y);
            abs_angle = 90 - arctan[arctan_entry];
        }

        if ((netOut[0] >= 0) && (netOut[1] >= 0)) {
            out_angle = abs_angle;
        }
        else if ((netOut[0] <= 0) && (netOut[1] >= 0)) {
            out_angle = 180 - abs_angle;
        }
        else if ((netOut[0] <= 0) && (netOut[1] <= 0)) {
            out_angle = 180 + abs_angle;
        }
        else {
            out_angle = 360 - abs_angle;
        }

        *out_ttl = out_angle / 45;
    }
    return;
}
