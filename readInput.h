void Solver::readInput(){
    ifstream file;
    file.open(path);

    string line;
    int number;

    for(int i = 1; i <= 3; i++){
        getline(file, line);
    }

    file >> line >> line >> nbNode;
    file >> line >> line >> line;
    file >> line >> line >> capacity;
    file >> line;

    requests = new Request[nbNode];

    for(int i = 0; i < nbNode; i++){
        requests[i].id = i;
        file >> number >> requests[i].coor_x >> requests[i].coor_y; 
    }

    file >> line;
    for(int i = 0; i < nbNode; i++){
        file >> number >> requests[i].demand;
    }

    file.close();

    for(int i = 0; i < nbNode; i++){
        for(int j = 0; j < nbNode; j++){
            distanceNode[i][j] = int(sqrt(pow(requests[i].coor_x - requests[j].coor_x , 2) + pow(requests[i].coor_y - requests[j].coor_y, 2)));
        }
    }
}