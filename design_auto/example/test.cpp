//#include "json.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include "json/json.h"

using namespace std;

int main()
{
    Json::Value people;
 
    ifstream people_file("people.json", ifstream::binary);

    people_file >> people;

    cout << people["Anna"]["age"] << "\n";

    cout << people["Ben"]["age"] << "\n";

    cout << "Processing Finished\n";
}

//std::ifstream people_file("people.json", std::ifstream::binary);
//people_file >> people;

//cout << people;

//cout << people["Anna"];


