

const int max_node = 1100;
const int max_nb_route = 100;
const int max_nb_node = 200;

#include "solution.h"
#include "extension.h"

struct Solver{
    string path;

    /**
     * Parameter
     **/
    int capacity;
    int distanceNode[max_node][max_node];
    int nbNode;
    Request * requests;

    Solution sol; //Current solution
    Solution bestSol; //best solution for ILS
    Solution lastSol;   //Best solution for MS-ILS

    void readInput();
    void constructor();
    void swap(int &);
    void relocate(int &);
    void threeMove(int &);
    void decreaseVehicle(int &);
    void perubation();
    void ILS();
    void MS_ILS();
    


    void runILS(){
        clock_t start_time;
        clock_t end_time;
        float long_time;

        start_time = clock();

        readInput();

        ILS();

        bestSol.displayRoute();

        end_time = clock();
        long_time = float(end_time - start_time) / CLOCKS_PER_SEC;

        ofstream file;
        file.open("result.csv", ios::app);
        printf("Last Objective %d - %fs\n", bestSol.objective, long_time);
        file << path << "," << bestSol.objective << "," << long_time << endl;
        file.close();
    }

    void runMSILS(){
        clock_t start_time;
        clock_t end_time;
        float long_time;

        start_time = clock();

        readInput();

        MS_ILS();

        lastSol.displayRoute();

        end_time = clock();
        long_time = float(end_time - start_time) / CLOCKS_PER_SEC;

        ofstream file;
        file.open("result.csv", ios::app);
        cout << "Last Objective: " << lastSol.objective << endl;
        file << path << "," << lastSol.objective << "," << long_time << endl;
        file.close();
    }
};



#include "readInput.h"
#include "ILS.h"
