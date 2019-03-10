#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

float lut[256] = {
		0,0.0078123411,0.0156237286,0.0234332094,0.0312398314,0.0390426439,0.0468406979,0.0546330468,0.0624187467,
		0.0701968573,0.0779664414,0.0857265663,0.093476304,0.1012147311,0.1089409299,0.1166539887,0.1243530018,
		0.1320370704,0.1397053028,0.1473568149,0.1549907304,0.1626061813,0.1702023084,0.1777782615,0.1853331999,
		0.1928662925,0.2003767185,0.2078636675,0.2153263397,0.2227639466,0.230175711,0.2375608675,0.2449186624,
		0.2522483545,0.2595492148,0.2668205274,0.274061589,0.2812717097,0.288450213,0.2955964359,0.3027097293,
		0.3097894581,0.3168350011,0.3238457517,0.3308211175,0.3377605207,0.3446633984,0.351529202,0.3583573984,
		0.3651474688,0.37189891,0.3786112336,0.3852839663,0.3919166501,0.3985088421,0.4050601148,0.4115700557,
		0.4180382676,0.4244643685,0.4308479916,0.4371887851,0.4434864126,0.4497405524,0.4559508978,0.4621171573,
		0.4682390538,0.4743163253,0.4803487241,0.4863360172,0.4922779861,0.4981744262,0.5040251475,0.5098299737,
		0.5155887425,0.5213013054,0.5269675271,0.5325872862,0.5381604741,0.5436869957,0.5491667683,0.5545997223,
		0.5599858005,0.5653249579,0.5706171618,0.5758623913,0.5810606373,0.5862119024,0.5913162001,0.5963735555,
		0.6013840042,0.6063475927,0.611264378,0.6161344271,0.6209578173,0.6257346357,0.6304649787,0.6351489524,
		0.6397866718,0.644378261,0.6489238528,0.6534235881,0.6578776166,0.6622860957,0.6666491906,0.6709670742,
		0.6752399267,0.6794679353,0.6836512945,0.6877902051,0.6918848746,0.6959355168,0.6999423513,0.7039056039,
		0.7078255059,0.7117022939,0.71553621,0.719327501,0.7230764189,0.7267832199,0.7304481652,0.7340715196,
		0.7376535525,0.7411945368,0.7446947494,0.7481544703,0.7515739833,0.754953575,0.758293535,0.761594156,
		0.7648557329,0.7680785635,0.7712629476,0.7744091874,0.777517587,0.7805884522,0.7836220909,0.7866188121,
		0.7895789266,0.7925027463,0.7953905842,0.7982427545,0.8010595722,0.8038413531,0.8065884134,0.8093010702,
		0.8119796408,0.8146244427,0.8172357939,0.8198140121,0.8223594152,0.824872321,0.8273530468,0.82980191,
		0.8322192272,0.8346053146,0.8369604881,0.8392850624,0.8415793518,0.8438436697,0.8460783286,0.84828364,
		0.8504599142,0.8526074606,0.8547265873,0.8568176011,0.8588808076,0.860916511,0.8629250139,0.8649066177,
		0.8668616221,0.8687903251,0.8706930233,0.8725700115,0.8744215828,0.8762480286,0.8780496383,0.8798266997,
		0.8815794985,0.8833083188,0.8850134424,0.8866951494,0.8883537177,0.8899894234,0.8916025404,0.8931933404,
		0.8947620933,0.8963090667,0.8978345261,0.8993387348,0.900821954,0.9022844428,0.9037264578,0.9051482536,
		0.9065500827,0.9079321952,0.9092948389,0.9106382595,0.9119627003,0.9132684025,0.9145556049,0.9158245442,
		0.9170754546,0.9183085682,0.9195241148,0.9207223218,0.9219034145,0.923067616,0.9242151467,0.9253462253,
		0.9264610678,0.9275598883,0.9286428982,0.9297103072,0.9307623223,0.9317991486,0.9328209887,0.9338280432,
		0.9348205105,0.9357985866,0.9367624655,0.9377123389,0.9386483966,0.9395708259,0.9404798121,0.9413755385,
		0.9422581861,0.9431279339,0.9439849588,0.9448294355,0.9456615369,0.9464814336,0.9472892943,0.9480852856,
		0.9488695722,0.9496423166,0.9504036796,0.9511538199,0.9518928942,0.9526210574,0.9533384624,0.9540452602,
		0.9547415999,0.9554276289,0.9561034926,0.9567693345,0.9574252964,0.9580715184,0.9587081385,0.9593352933,
		0.9599531174,0.9605617438,0.9611613036,0.9617519265,0.9623337402,0.962906871,0.9634714433};


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

int main() {
	
    FILE *ifp, *ofp, *tfp, *dfp;
    struct timeval t1, t2, tr;

    int i, j, t;
    double inWMd[500];
    double ofbWMd[2][500];
    double intWMd[500][500];
    double outWMd[501][2];

    //int inWM[500];
    //int ofbWM[2][500];
    //int intWM[500][500];
    //int outWM[501][2];
    float inWM[500];
    float ofbWM[2][500];
    float intWM[500][500];
    float outWM[501][2];

    int initialRunlength = 640;
    int plotRunlength = 3200*6*6;
    int samplelength = initialRunlength + plotRunlength;
    int offs = 100000;

    double sampleinputd[samplelength];
    double sampleoutd[samplelength][2];

    //int sampleinput[samplelength];
    float sampleinput[samplelength];
    float sampleout[samplelength][2];
 
	float inputscaling = 0.125;
    int inputshift = 0;
    float teacherscaling[2] = {0.25, 0.25};
    int teachershift[2] = {0, 0};

    float in;
    int teach[2];

    //int totalstate[503];
    //int internalState[500];
    //int netOut[2];
    float totalstate[503];
    float internalState[500];
    float netOut[2];

    unsigned char internalState_entry;
    unsigned char netOut0_entry;
    unsigned char netOut1_entry;
    unsigned char arctan_entry;

	unsigned char cur_pnt;
    int sampleinput_buf[256];
    int acc_sampleinput;
    //float sampleinput_buf[256];
    //float acc_sampleinput;

    int diff_sampleinput;
    //float diff_sampleinput;
    float norm_sampleinput;
    int input;
    int average_val = 3915;
    int range_val = 19660;
    //float average_val = 0.119476318;  //3915/32768.0;
    //float range_val = 0.599975586;  //19660/32768.0;

    int max_val = 0;
    int min_val = 0;

    // Read in inWM from "inWM.bin" file
    if (!(ifp = fopen("inWM.bin", "rb"))) {
        printf("File inWM.bin cannot be opened for read.\n");
        return -1;
    }
    fread(inWMd, 8, 500, ifp);
    fclose(ifp);
    for (i = 0; i < 500; ++i)
    	//inWM[i] = (int)(inWMd[i] * 32768);
        inWM[i] = (float) inWMd[i];

    // Read in ofbWM from "ofbWM.bin" file
    if (!(ifp = fopen("ofbWM.bin", "rb"))) {
        printf("File ofbWM.bin cannot be opened for read.\n");
        return -1;
    }
    fread(ofbWMd[0], 8, 500, ifp);
    fread(ofbWMd[1], 8, 500, ifp);
    fclose(ifp);
    for (i = 0; i < 500; ++i) {
    	//ofbWM[0][i] = (int)(ofbWMd[0][i] * 32768);
        //ofbWM[1][i] = (int)(ofbWMd[1][i] * 32768);
        ofbWM[0][i] = (float) ofbWMd[0][i];
        ofbWM[1][i] = (float) ofbWMd[1][i];
    }

    // Read in intWM from "intWM.bin" file
    if (!(ifp = fopen("intWM.bin", "rb"))) {
        printf("File intWM.bin cannot be opened for read.\n");
        return -1;
    }
    for (i = 0; i < 500; ++i)
        fread(intWMd[i], 8, 500, ifp);
    fclose(ifp);
    for (i = 0; i < 500; ++i)
        for (j = 0; j < 500; ++j)
        	//intWM[i][j] = (int)(intWMd[i][j] * 32768);
            intWM[i][j] = (float) intWMd[i][j];

    // Read in outWM from "outWM.bin" file
    if (!(ifp = fopen("outWM.bin", "rb"))) {
        printf("File outWM.bin cannot be opened for read.\n");
        return -1;
    }
    for (i = 0; i < 501; ++i)
        fread(outWMd[i], 8, 2, ifp);
    fclose(ifp);
    for (i = 0; i < 501; ++i) {
    	//outWM[i][0] = (int)(outWMd[i][0] * 32768);
        //outWM[i][1] = (int)(outWMd[i][1] * 32768);
        outWM[i][0] = (float) outWMd[i][0];
        outWM[i][1] = (float) outWMd[i][1];
    }

    // Read in sample input from "sampleinput.bin" file
    if (!(ifp = fopen("sampleinput.bin", "rb"))) {
        printf("File sampleinput.bin cannot be opened for read.\n");
        return -1;
    }
    fseek(ifp, (offs*8), SEEK_SET);
    fread(sampleinputd, 8, samplelength, ifp);
    fclose(ifp);
    for (i = 0; i < samplelength; ++i)
    	//sampleinput[i] = (int)(sampleinputd[i] * 32768);
    	sampleinput[i] = (float) sampleinputd[i];

    // Read in sample out from "sampleout.bin" file
    if (!(ifp = fopen("sampleout.bin", "rb"))) {
        printf("File sampleout.bin cannot be opened for read.\n");
        return -1;
    }
    fseek(ifp, (offs*8*2), SEEK_SET);
    for (i = 0; i < samplelength; ++i)
        fread(sampleoutd[i], 8, 2, ifp);
    fclose(ifp);
    for (i = 0; i < samplelength; ++i) {
        //sampleout[i][0] = (int)(sampleoutd[i][0] * 32768);
        //sampleout[i][1] = (int)(sampleoutd[i][1] * 32768);
        sampleout[i][0] = (float) sampleoutd[i][0];
        sampleout[i][1] = (float) sampleoutd[i][1];
    }

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

    // Open "record.txt" file for data record
    if (!(dfp = fopen("record.txt", "w"))) {
    	printf("File record.txt cannot be opened for write\n");
    	return -1;
    }

    for (i = 0; i < 503; ++i)
        totalstate[i] = 0;

    //for (i = 0; i < 256; ++i)
    //	sampleinput_buf[i] = 0;

    gettimeofday(&t1, NULL);
    for (t = 0; t < samplelength; ++t) {
        if (t % 10000 == 0) {
            gettimeofday(&t2, NULL);
            timersub(&t1, &t2, &tr);
            printf("Execute time: %.2f sec\n", -tr.tv_sec-(double)tr.tv_usec/1000000.0);
            printf("Iteration %d: t = %d\n", (t/10000), t);
            gettimeofday(&t1, NULL);
        }

        //cur_pnt++;
        //sampleinput_buf[cur_pnt] = sampleinput[t];
        //acc_sampleinput = 0;
        //for (i = 0; i < 256; ++i) {
        //	acc_sampleinput += sampleinput_buf[i];
        //}

        //diff_sampleinput = sampleinput[t] - (acc_sampleinput / 256);
        //norm_sampleinput = ((float)(diff_sampleinput - average_val))/((float)range_val);
        //input = (int)(norm_sampleinput * 32768);

        in = inputscaling * sampleinput[t] + inputshift;
        teach[0] = teacherscaling[0] * sampleout[t][0] + teachershift[0];
        teach[1] = teacherscaling[1] * sampleout[t][1] + teachershift[1];

        totalstate[500] = input;

        for (i = 0; i < 500; ++i) {
            internalState[i] = 0;
            for (j = 0; j < 500; ++j)
                internalState[i] += (intWM[j][i] * totalstate[j]);
            internalState[i] += (inWM[i] * totalstate[500]);
            internalState[i] += (ofbWM[0][i] * totalstate[501]);
            internalState[i] += (ofbWM[1][i] * totalstate[502]);
            internalState[i] = tanhf(internalState[i]);

            /*
            if (internalState[i] > 0) {
            	if ((internalState[i] >> 8) >= 255)
            		internalState_entry = 255;
            	else 
            		internalState_entry = (unsigned char)(internalState[i] >> 8);
            	internalState[i] = (int)(lut[internalState_entry] * 32768);
            }
            else {
            	if (((-internalState[i]) >> 8) >= 255)
            		internalState_entry = 255;
            	else 
            		internalState_entry = (unsigned char)((-internalState[i]) >> 8);
            	internalState[i] = -(int)(lut[internalState_entry] * 32768);
            }
            */
            
            //internalState[i] = tanh(internalState[i]);

            //if (t == 100) {
            //	printf("internalState[i] = %f\n", internalState[i]);
            //}
        }

        netOut[0] = 0;
        netOut[1] = 0;
        for (i = 0; i < 500; ++i) {
            netOut[0] += (outWM[i][0] * internalState[i]);
            netOut[1] += (outWM[i][1] * internalState[i]);
            totalstate[i] = internalState[i];
            //netOut[0] += (outWM[i][0] * internalState[i]);
            //netOut[1] += (outWM[i][1] * internalState[i]);
            //totalstate[i] = internalState[i];
		}

		//if (t <= 50) {
            //printf("netOut[0], netOut[1] = %d, %d\n", netOut[0], netOut[1]);
        //}

		//printf("netOut[0], netOut[1] = %d, %d\n", netOut[0], netOut[1]);

        netOut[0] += (outWM[500][0] * in);
        netOut[1] += (outWM[500][1] * in);
        netOut[0] = tanhf(netOut[0]);
        netOut[1] = tanhf(netOut[1]);

        //netOut[0] += (outWM[500][0] * norm_sampleinput);
        //netOut[1] += (outWM[500][1] * norm_sampleinput);

        /*
	    if (netOut[0] > 0) {
	    	if ((netOut[0] >> 8) >= 255)
	    		netOut0_entry = 255;
	    	else
	    		netOut0_entry = (unsigned char)(netOut[0] >> 8);
	    	netOut[0] = (int)(lut[netOut0_entry] * 32768);
	    }
	    else {
	    	if (((-netOut[0]) >> 8) >= 255)
	    		netOut0_entry = 255;
	    	else
	    		netOut0_entry = (unsigned char)((-netOut[0]) >> 8);
	    	netOut[0] = -(int)(lut[netOut0_entry] * 32768);
	    }

	    if (netOut[1] > 0) {
	    	if ((netOut[1] >> 8) >= 255)
	    		netOut1_entry = 255;
	    	else
	    		netOut1_entry = (unsigned char)(netOut[1] >> 8);
	    	netOut[1] = (int)(lut[netOut1_entry] * 32768);
	    }
	    else {
	    	if (((-netOut[1]) >> 8) >= 255)
	    		netOut1_entry = 255;
	    	else
	    		netOut1_entry = (unsigned char)((-netOut[1]) >> 8);
	    	netOut[1] = -(int)(lut[netOut1_entry] * 32768);
	    }
	    */

        //netOut[0] = tanh(netOut[0]);
        //netOut[1] = tanh(netOut[1]);

        //printf("netOut[0], netOut[1] = %d, %d\n", netOut[0], netOut[1]);

        fprintf(dfp, "%f\t%f\n", netOut[0], netOut[1]);

        /*
        if (netOut[0] > 0) {
        	if ((netOut[0] >> 6) >= 255)
            	netOut0_entry = 255;
            else 
            	netOut0_entry = (unsigned char)(netOut[0] >> 6);
        	netOut[0] = (short)(lut[netOut0_entry] * 32768);
        }
        else {
        	if ((-netOut[0] >> 6) >= 255)
        		netOut0_entry = 255;
        	else
        		netOut0_entry = (unsigned char)(-netOut[0] >> 6);
        	netOut[0] = -(short)(lut[netOut0_entry] * 32768);
        }


        if (netOut[1] > 0) {
        	if ((netOut[1] >> 6) >= 255)
            	netOut1_entry = 255;
            else 
            	netOut1_entry = (unsigned char)(netOut[1] >> 6);
        	netOut[1] = (short) (lut[netOut1_entry] * 32768);
        }
        else {
        	if ((-netOut[1] >> 6) >= 255)
        		netOut1_entry = 255;
        	else
        		netOut1_entry = (unsigned char)(-netOut[1] >> 6);
        	netOut[1] = -(short)(lut[netOut1_entry] * 32768);
        }
        //printf("netOut[0], netOut[1] = %d, %d\n", netOut[0], netOut[1]);
		*/


        //fwrite(&input, 4, 1, ofp);
        fwrite(netOut, 4, 2, ofp);
        
        if (t < initialRunlength) {
            totalstate[501] = teach[0];
            totalstate[502] = teach[1];
        }
        else {
            totalstate[501] = netOut[0];
            totalstate[502] = netOut[1];
        }
    }

    //printf("max_val = %d, min_val = %d.\n", max_val, min_val);

    /*
    printf("%f\n", sampleout[0][0]);
    printf("%f\n", sampleout[1][0]);
    printf("%f\n", sampleout[2][0]);
    printf("%f\n", sampleout[0][1]);
    printf("%f\n", sampleout[1][1]);
    printf("%f\n", sampleout[2][1]);
    */

    fclose(ofp);
    fclose(tfp);

    printf("Processing complete.\n");

    return 0;
}
