#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>

using namespace std;

#include "request.h"
#include "solver.h"

vector <string> getListName(string );

int main(){
    //srand(time(NULL));

    vector <string> listName = getListName("Instances/list-name.txt");
    string path;


    //for(int test = listName.size() - 1; test < listName.size(); test++){
    for(int test = 0; test < 1; test++){
        Solver solve;
        //path = "Instances/" + listName[test];
        //path = "Instances/X\\X-n1001-k43.vrp";
        path = "Instances/X\\X-n139-k10.vrp";
        
        //cout << "Instances" << test + 1 << ": " << path << endl;
        solve.path = path;
        //solve.runILS();
        solve.runMSILS();
        
    }
    
    cout << "End program" << endl;
    return 0;
}

vector <string> getListName(string path){
    vector <string> listName;
    string name;
    int number_vrp;

    ifstream file;
    file.open(path);

    file >> number_vrp;
    for(int i = 0; i < number_vrp; i++){
        file >> name;
        listName.push_back(name);
    }
    file.close();
    return listName;
}