//#include "json.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include "json/json.h"

using namespace std;

template <typename T> string to_string(const T& t)
{
    ostringstream os;
    os << t;
    return os.str();
}

int main()
{
    Json::Value descrip;
 
    ifstream desc_file("circuit.json", ifstream::binary);

    desc_file >> descrip;

    string template_file = "./template/template_lfp_phase.c";
    string export_file = "./export/autogen_lfp_phase.c";

    string inWM_file = descrip["Parameter"]["inWM"].asString();
    string ofbWM_file = descrip["Parameter"]["ofbWM"].asString();
    string outWM_file = descrip["Parameter"]["outWM"].asString();
    string intWM_file = descrip["Parameter"]["intWM"].asString();

    cout << inWM_file << '\n';

    //string inWM_file = "./source/inWM.bin";
    //string ofbWM_file = "./source/ofbWM.bin";
    //string intWM_file = "./source/intWM.bin";
    //string outWM_file = "./source/outWM.bin";

    FILE *ifp;

    int i, j, k;
    double inWM_double[500];
    double ofbWM_double[2][500];
    double intWM_double[500][500];
    double outWM_double[501][2];

    float inWM[500];
    float ofbWM[2][500];
    float intWM[500][500];
    float outWM[501][2];

    int tmp;
    int cnt[500];
    int id_reorder[500];
    int id_convert[500];

    // Read in inWM from "./source/inWM.bin" file
    if (!(ifp = fopen(inWM_file.c_str(), "rb"))) {
        cout << "File inWM.bin cannot be opened for read.\n";
        return -1;
    }
    fread(inWM_double, 8, 500, ifp); 
    fclose(ifp);
    for (i = 0; i < 500; ++i)
        inWM[i] = (float) inWM_double[i];

    // Read in ofbWm from "./source/ofbWM.bin" file
    if (!(ifp = fopen(ofbWM_file.c_str(), "rb"))) {
        cout << "File ofbWM.bin cannot be opened for read.\n";
        return -1;
    }
    fread(ofbWM_double[0], 8, 500, ifp);
    fread(ofbWM_double[1], 8, 500, ifp);
    fclose(ifp);
    for (i = 0; i < 500; ++i) {
        ofbWM[0][i] = (float) ofbWM_double[0][i];
        ofbWM[1][i] = (float) ofbWM_double[1][i];
    }

    // Read in intWM from "./source/intWM.bin" file
    if (!(ifp = fopen(intWM_file.c_str(), "rb"))) {
        cout << "File intWM.bin cannot be opened for read.\n";
        return -1;
    }
    for (i = 0; i < 500; ++i)
        fread(intWM_double[i], 8, 500, ifp);
    fclose(ifp);
    for (i = 0; i < 500; ++i)
        for (j = 0; j < 500; ++j)
            intWM[i][j] = (float) intWM_double[i][j];

    // Read in outWM from "./source/outWM.bin" file
    if (!(ifp = fopen(outWM_file.c_str(), "rb"))) {
        cout << "File outWM.bin cannot be opened for read.\n";
        return -1;
    }
    for (i = 0; i < 501; ++i)
        fread(outWM_double[i], 8, 2, ifp);
    fclose(ifp);
    for (i = 0; i < 501; ++i) {
        outWM[i][0] = (float) outWM_double[i][0];
        outWM[i][1] = (float) outWM_double[i][1];
    }

    // Count non-zero weight values.
    for (i = 0; i < 500; ++i) {
        cnt[i] = 0;
        id_reorder[i] = i;
        for (j = 0; j < 500; ++j) {
            if (intWM[i][j] != 0) {
                cnt[i]++;
            }
        }
    }

    // Reorder non-zero weight values.
    for (i = 0; i < 500; ++i) {
        k = i;
        for (j = i; j < 500; ++j) {
            if (cnt[j] < cnt[k]) {
                k = j;
            }
        }
        tmp = cnt[i];
        cnt[i] = cnt[k];
        cnt[k] = tmp;
        tmp = id_reorder[i];
        id_reorder[i] = id_reorder[k];
        id_reorder[k] = tmp;
    }

    // Convert row exchange mapping.
    for (i = 0; i < 500; ++i) {
        id_convert[id_reorder[i]] = i;
    }

    // Construct inWM definition according to exchanged order.
    string str_inWM = "float inWM[500] = {\n";
    for (i = 0; i < 50; ++i) {
        str_inWM += '\t';
        for (j = 0; j < 10; ++j) {
            str_inWM += to_string(inWM[id_reorder[i*10+j]]) + ',';
        };
        str_inWM += '\n';
    }
    str_inWM.replace(str_inWM.end()-2, str_inWM.end(), "};");
    
    // Construct ofbWM definition according to exchanged order.
    string str_ofbWM = "float ofbWM[2][500] = {\n";
    for (k = 0; k < 2; ++k) {
        for (i = 0; i < 50; ++i) {
            str_ofbWM += '\t';
            for (j = 0; j < 10; ++j) {
                str_ofbWM += to_string(ofbWM[k][id_reorder[i*10+j]]) + ',';
            }
            str_ofbWM += '\n';
        }
    }
    str_ofbWM.replace(str_ofbWM.end()-2, str_ofbWM.end(), "};");

    // Construct outWM definition according to exchanged order.
    string str_outWM = "float outWM[500][2] = {\n";
    for (i = 0; i < 100; ++i) {
        str_outWM += '\t';
        for (j = 0; j < 5; ++j) {
            str_outWM += to_string(outWM[id_reorder[i*5+j]][0]) + ',';
            str_outWM += to_string(outWM[id_reorder[i*5+j]][1]) + ',';
        }
        str_outWM += '\n';
    }
    str_outWM.replace(str_outWM.end()-2, str_outWM.end(), "};\n\n");
    str_outWM += "float outWM_500[2] = {" + to_string(outWM[500][0]) + ',' + to_string(outWM[500][1]) + "};";

    // Construct intWM non-zero weight values according to exchanged order.
    string str_intWM = "float intWM[500][120] = {\n";
    for (i = 0; i < 500; ++i) {
        str_intWM += "\t{";
        for (j = 0; j < 500; ++j) {
            if (intWM[id_reorder[i]][j] != 0) {
                str_intWM += to_string(intWM[id_reorder[i]][j]) + ',';
            }
        }
        str_intWM.replace(str_intWM.end()-1, str_intWM.end(), "},\n");
    }
    str_intWM.replace(str_intWM.end()-2, str_intWM.end(), "};");

    // Construct intWM non-zero weight column index according to exchanged order.
    string str_intWM_j = "uint10 intWM_j[500][120] = {\n";
    for (i = 0; i < 500; ++i) {
        str_intWM_j += "\t{";
        for (j = 0; j < 500; ++j) {
            if (intWM[id_reorder[i]][j] != 0) {
                str_intWM_j += to_string(id_convert[j]) + ',';
            }
        }
        str_intWM_j.replace(str_intWM_j.end()-1, str_intWM_j.end(), "},\n");
    }
    str_intWM_j.replace(str_intWM_j.end()-2, str_intWM_j.end(), "};");

    cout << "Five Group: \n";
    cout << cnt[99] << '\n';
    cout << cnt[199] << '\n';
    cout << cnt[299] << '\n';
    cout << cnt[399] << '\n';
    cout << cnt[499] << '\n';

    cout << str_intWM_j << '\n';

    string line;

    ifstream infile(template_file.c_str());
    if (!infile.is_open()) {
        cout << "Unable to open template file.\n";
        return -1;
    }
    
    ofstream outfile(export_file.c_str());
    if (!outfile.is_open()) {
        cout << "Unable to open export file.\n";
        return -1;
    }

    string str;
    size_t pos;

    while (getline(infile, line)) {

        str = "/*DEFINE_INWM*/";
        pos = line.find(str);
        if (pos != string::npos) {
            line.replace(pos, str.length(), str_inWM);
        }
        str = "/*DEFINE_OFBWM*/";
        pos = line.find(str);
        if (pos != string::npos) {
            line.replace(pos, str.length(), str_ofbWM);
        }
        str = "/*DEFINE_OUTWM*/";
        pos = line.find(str);
        if (pos != string::npos) {
            line.replace(pos, str.length(), str_outWM);
        }
        str = "/*DEFINE_INTWM*/";
        pos = line.find(str);
        if (pos != string::npos) {
            line.replace(pos, str.length(), str_intWM);
        }
        str = "/*DEFINE_INTWM_J*/";
        pos = line.find(str);
        if (pos != string::npos) {
            line.replace(pos, str.length(), str_intWM_j);
        }
 
        str = "/*LOOP_ITERATION_1*/";
        pos = line.find(str);
        if (pos != string::npos) {
            line.replace(pos, str.length(), to_string(cnt[99]));
        }
        str = "/*LOOP_ITERATION_2*/";
        pos = line.find(str);
        if (pos != string::npos) {
            line.replace(pos, str.length(), to_string(cnt[199]));
        }
        str = "/*LOOP_ITERATION_3*/";
        pos = line.find(str);
        if (pos != string::npos) {
            line.replace(pos, str.length(), to_string(cnt[299]));
        }
        str = "/*LOOP_ITERATION_4*/";
        pos = line.find(str);
        if (pos != string::npos) {
            line.replace(pos, str.length(), to_string(cnt[399]));
        }
        str = "/*LOOP_ITERATION_5*/";
        pos = line.find(str);
        if (pos != string::npos) {
            line.replace(pos, str.length(), to_string(cnt[499]));
        }

        outfile << line << '\n';
    }

    //cout << people["Anna"]["age"] << "\n";
    //cout << people["Ben"]["age"] << "\n";

    infile.close();
    outfile.close();

    cout << "Processing Finished.\n";

    return 0;
}

//std::ifstream people_file("people.json", std::ifstream::binary);
//people_file >> people;

//cout << people;

//cout << people["Anna"];


