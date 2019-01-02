void Solver::ILS(){
    clock_t start_time = clock();

    constructor();
    bestSol = sol;

    printf("--->Improve...\n");
    int flagSwap = 0;
    int flagReclocate = 0;
    swap(flagSwap);
    relocate(flagReclocate);

    int nbNoImprove = 0;
    while(nbNoImprove < 7000){
        if((float(clock() - start_time) / CLOCKS_PER_SEC) > 59){
            break;
        }

        perubation();
        while(true){
            flagSwap = 0;
            flagReclocate = 0;
            swap(flagSwap);
            relocate(flagReclocate);
            if(flagReclocate == 1){
                continue;
            }
            if(bestSol.objective > sol.objective){
                bestSol = sol;
                nbNoImprove = 0;
            }else{
                int cooling = rand() % 100;
                if(cooling < 90){
                    sol = bestSol;
                }
                nbNoImprove++;
            }
            break;
        }
    }
    printf("\tPotential objective: %d\n", bestSol.objective);
    printf("----END ILS----\n");
}

void Solver::MS_ILS(){
    ILS();
    lastSol = bestSol;

    int nbNoImprove = 0;
    while(nbNoImprove < 10){
        ILS();
        if(lastSol.objective > bestSol.objective){
            lastSol = bestSol;
            nbNoImprove = 0;
        }else{
            nbNoImprove++;
        }
    }
    printf("========= END MS-ILS =========\n");
    
}

void Solver::constructor(){
    int * orderNode = new int[nbNode];
    for(int i = 1; i < nbNode; i++){
        orderNode[i] = i;
    }
    shuffleArray(orderNode, nbNode);

    for(int i = 0; i < max_nb_route; i++){
        sol.route_load[i] = 0;
        sol.routes[i][0] = 0;
        sol.routes[i][1] = 0;
        sol.nbNodeInRoute[i] = 2;
    }

    int inRoute[nbNode];
    for(int i = 1; i < nbNode; i++){
        inRoute[i] = 0;
    }
    sol.objective = 0;
    int nbServeNode = 1;

    int incCost;
    int minIncCost;
    int saveNode;
    int saveRoute;
    int savePos;
    
    while(nbServeNode < nbNode){
        minIncCost = 9999999;
        for(int sequence = 1; sequence < nbNode; sequence++){
            int i = orderNode[sequence];
            if(!inRoute[i]){
                for(int route_id = 0; route_id < max_nb_route; route_id++){
                    if(sol.route_load[route_id] + requests[i].demand <= capacity){
                        for(int pos = 1; pos < sol.nbNodeInRoute[route_id]; pos++){
                            incCost = distanceNode[sol.routes[route_id][pos-1]][i] + distanceNode[i][sol.routes[route_id][pos]] - distanceNode[sol.routes[route_id][pos-1]][sol.routes[route_id][pos]];
                            if(minIncCost > incCost){
                                minIncCost = incCost;
                                saveNode = i;
                                saveRoute = route_id;
                                savePos = pos;
                            }
                        }
                    }
                }
            }
        }
        if(minIncCost != 9999999){
        for(int i = sol.nbNodeInRoute[saveRoute]; i > savePos; i--){
            sol.routes[saveRoute][i] = sol.routes[saveRoute][i - 1];
        }

        sol.objective += minIncCost;
        
        sol.routes[saveRoute][savePos] = saveNode;
        sol.route_load[saveRoute] += requests[saveNode].demand;
        sol.nbNodeInRoute[saveRoute]++;
        inRoute[saveNode] = 1;
        nbServeNode++;
        }
    }
    int count_request = 0;
    sol.nb_vehicle = 0;
     for(int route_id = 0; route_id < max_nb_route; route_id++){
        if(sol.nbNodeInRoute[route_id] == 2){
            continue;
        }
        sol.nb_vehicle++;
        for(int pos = 0; pos < sol.nbNodeInRoute[route_id]; pos++){
            if(sol.routes[route_id][pos] != 0){
                count_request++;
            }
        }
    }
    cout << "+) Number vehicle: " << sol.nb_vehicle << endl;
    cout << "+) Request: " << count_request << endl;
    cout << "+) Objecitve: " << sol.objective << endl;

    delete [] orderNode;
}

void Solver::swap(int & flagSwap){
    int saveK1;
    int saveK2;
    int savePos1;
    int savePos2;
    int saveNode1;
    int saveNode2;
    int saving = 1;
    int saveK;

    //Intra
    saving = 1;
    while(saving > 0){
        saving = 0;
        for(int k = 0; k < sol.nb_vehicle; k++){
            for(int pos1 = 1; pos1 < sol.nbNodeInRoute[k] - 2; pos1++){
                for(int pos2 = pos1 + 1; pos2 < sol.nbNodeInRoute[k] - 1; pos2++){
                    int node1 = sol.routes[k][pos1];
                    int node2 = sol.routes[k][pos2];
                    int before1 = sol.routes[k][pos1 - 1];
                    int after2 = sol.routes[k][pos2 + 1];

                    int changeCost = 0;
                   
                    if(pos1 + 1 == pos2){
                        changeCost = distanceNode[before1][node1] + distanceNode[node2][after2] - distanceNode[before1][node2] - distanceNode[node1][after2];
                        
                    }else{
                        int after1 = sol.routes[k][pos1 + 1];
                        int before2 = sol.routes[k][pos2 - 1];
                        
                        changeCost = distanceNode[before1][node1] + distanceNode[node1][after1] + distanceNode[before2][node2] + distanceNode[node2][after2]
                            - distanceNode[before1][node2] - distanceNode[node2][after1] - distanceNode[before2][node1] - distanceNode[node1][after2];
                    }

                    if(changeCost > saving){
                        saving = changeCost;
                        saveK = k;
                        saveNode1 = node1;
                        saveNode2 = node2;
                        savePos1 = pos1;
                        savePos2 = pos2;
                    }
                }
            }
        }

        if(saving > 0){
            flagSwap = 1;
            sol.routes[saveK][savePos1] = saveNode2;
            sol.routes[saveK][savePos2] = saveNode1;

            sol.objective -= saving;
        }
    }

    //Inter
    saving = 1;
    while(saving > 0){
        saving = 0;
        for(int k1 = 0; k1 < sol.nb_vehicle; k1++){
            for(int k2 = k1 + 1; k2 < sol.nb_vehicle; k2++){
                for(int pos1 = 1; pos1 < sol.nbNodeInRoute[k1] - 1; pos1++){
                    for(int pos2 = 1; pos2 < sol.nbNodeInRoute[k2] - 1; pos2++){
                        int node1 = sol.routes[k1][pos1];
                        int node2 = sol.routes[k2][pos2];
                        int before1 = sol.routes[k1][pos1 - 1];
                        int after1 = sol.routes[k1][pos1 + 1];
                        int before2 = sol.routes[k2][pos2 - 1];
                        int after2 = sol.routes[k2][pos2 + 1];

                        int changeCost = 0;

                        if(sol.route_load[k1] - requests[node1].demand + requests[node2].demand <= capacity
                        && sol.route_load[k2] - requests[node2].demand + requests[node1].demand <= capacity){
                            changeCost = distanceNode[before1][node1] + distanceNode[node1][after1] + distanceNode[before2][node2] + distanceNode[node2][after2]
                                - distanceNode[before1][node2] - distanceNode[node2][after1] - distanceNode[before2][node1] - distanceNode[node1][after2];
                            
                            if(changeCost > saving){
                                saving = changeCost;
                                saveK1 = k1;
                                saveK2 = k2;
                                saveNode1 = node1;
                                saveNode2 = node2;
                                savePos1 = pos1;
                                savePos2 = pos2;
                            }
                        }
                    }
                }
            }
        }

        if(saving > 0){
            flagSwap = 1;
            sol.routes[saveK1][savePos1] = saveNode2;
            sol.routes[saveK2][savePos2] = saveNode1;

            sol.route_load[saveK1] += requests[saveNode2].demand - requests[saveNode1].demand;
            sol.route_load[saveK2] += requests[saveNode1].demand - requests[saveNode2].demand;

            sol.objective -= saving;
        }
    }
}

/**
 * Relocate
 * @idea: insert node1 before node2
 **/
void Solver::relocate(int & flagReclocate){
    int saveK1;
    int saveK2;
    int saveK;
    int saveNode1;
    int saveNode2;
    int savePos1;
    int savePos2;
    int saving;
    int changeCost;

    //Intra
    saving = 1;
    while(saving > 0){
        saving = 0;
        for(int k = 0; k < sol.nb_vehicle; k++){
            for(int pos1 = 1; pos1 < sol.nbNodeInRoute[k] - 2; pos1++){
                for(int pos2 = 1; pos2 < sol.nbNodeInRoute[k] - 1; pos2++){
                    int node1 = sol.routes[k][pos1];
                    int node2 = sol.routes[k][pos2];

                    if(pos1 < pos2 - 2 || pos1 > pos2 + 1){
                        int before1 = sol.routes[k][pos1 - 1];
                        int after1 = sol.routes[k][pos1 + 1];
                        int before2 = sol.routes[k][pos2 - 1];
                        
                        int changeCost = 0;
                        changeCost = distanceNode[before1][node1] + distanceNode[node1][after1] + distanceNode[before2][node2]
                            - distanceNode[before1][after1] - distanceNode[before2][node1] - distanceNode[node1][node2];
                        
                        if(saving < changeCost){
                            saving = changeCost;
                            saveK = k;
                            saveNode1 = node1;
                            saveNode2 = node2;
                            savePos1 = pos1;
                            savePos2 = pos2;
                        }
                    }
                }
            }
        }
        if(saving > 0){
            sol.objective -= saving;

            if(savePos1 < savePos2){
                for(int l = savePos1; l < savePos2 - 1; l++){
                    sol.routes[saveK][l] = sol.routes[saveK][l+1];
                }
                sol.routes[saveK][savePos2 - 1] = saveNode1;

            }else{
                for(int l = savePos1; l > savePos2; l--){
                    sol.routes[saveK][l] = sol.routes[saveK][l - 1];
                }
                sol.routes[saveK][savePos2] = saveNode1;
            }
        }
    }


    //Inter
    saving = 1;
    while(saving > 0){
        saving = 0;
        for(int k1 = 0; k1 < sol.nb_vehicle; k1++){
            for(int k2 = k1 + 1; k2 < sol.nb_vehicle; k2++){
                for(int pos1 = 1; pos1 < sol.nbNodeInRoute[k1] - 2; pos1++){
                    for(int pos2 = 1; pos2 < sol.nbNodeInRoute[k2] - 1; pos2++){
                        int node1 = sol.routes[k1][pos1];
                        int node2 = sol.routes[k2][pos2];

                        if(sol.route_load[k2] + requests[node1].demand <= capacity){
                            int before1 = sol.routes[k1][pos1 - 1];
                            int after1 = sol.routes[k1][pos1 + 1];
                            int before2 = sol.routes[k2][pos2 - 1];

                            changeCost = distanceNode[before1][node1] + distanceNode[node1][after1] + distanceNode[before2][node2]
                                - distanceNode[before1][after1] - distanceNode[before2][node1] - distanceNode[node1][node2];
                            
                            if(changeCost > saving){
                                saving = changeCost;
                                saveK1 = k1;
                                saveK2 = k2;
                                saveNode1 = node1;
                                saveNode2 = node2;
                                savePos1 = pos1;
                                savePos2 = pos2;
                            }
                        }
                    }
                }
            }
        }
        if(saving > 0){
            flagReclocate = 1;
            for(int l = sol.nbNodeInRoute[saveK2]; l > savePos2; l--){
                sol.routes[saveK2][l] = sol.routes[saveK2][l - 1];
            }
            sol.routes[saveK2][savePos2] = saveNode1;
            sol.nbNodeInRoute[saveK2]++;

            for(int l = savePos1; l < sol.nbNodeInRoute[saveK1] - 1; l++){
                sol.routes[saveK1][l] = sol.routes[saveK1][l + 1];
            }
            sol.nbNodeInRoute[saveK1]--;

            sol.route_load[saveK1] -= requests[saveNode1].demand;
            sol.route_load[saveK2] += requests[saveNode1].demand;

            sol.objective -= saving;

        }
    }
}

void Solver::threeMove(int &flagThree){

}

void Solver::perubation(){
    int k1;
    int k2;
    int pos1;
    int pos2;
    int node1;
    int node2;
    int selectType;

    for(int k = 0; k < 1; k++){
        selectType = rand() % 2;
        if(selectType == 0){
            /**
             * Random swap pertubation
             **/
            int nbSwap = rand() % int(0.03 * nbNode) + 2;
            nbSwap = 4;
            for(int i = 0; i < nbSwap; i++){
            //for(int i = 0; i < int(0.1 * nbNode); i++){
                k1 = rand() % sol.nb_vehicle;
                do{
                    k2 = rand() % sol.nb_vehicle;
                }while(k1 == k2);
                //Fix in here: rand() % < 0
                if(sol.nbNodeInRoute[k1] < 3 || sol.nbNodeInRoute[k2] < 3){
                    continue;
                }
                pos1 = rand() % (sol.nbNodeInRoute[k1] - 2) + 1;
                pos2 = rand() % (sol.nbNodeInRoute[k2] - 2) + 1;
                node1 = sol.routes[k1][pos1];
                node2 = sol.routes[k2][pos2];

                if(sol.route_load[k1] + requests[node2].demand - requests[node1].demand <= capacity
                && sol.route_load[k2] + requests[node1].demand - requests[node2].demand <= capacity){
                    int before1 = sol.routes[k1][pos1 - 1];
                    int after1 = sol.routes[k1][pos1 + 1];
                    int before2 = sol.routes[k2][pos2 - 1];
                    int after2 = sol.routes[k2][pos2 + 1];
                    
                    sol.objective -= distanceNode[before1][node1] + distanceNode[node1][after1] + distanceNode[before2][node2] + distanceNode[node2][after2]
                        - distanceNode[before1][node2] - distanceNode[node2][after1] - distanceNode[before2][node1] - distanceNode[node1][after2];

                    sol.routes[k1][pos1] = node2;
                    sol.routes[k2][pos2] = node1;
                    
                    sol.route_load[k1] += requests[node2].demand - requests[node1].demand;
                    sol.route_load[k2] += requests[node1].demand - requests[node2].demand;
                }
            }
        }else if(selectType == 1){
            /**
             * Random relocation pertubation
             * @idea: relocate node1 to before node2
             **/
            int nbRelocate = rand() % (int(0.06 * nbNode)) + 2;
            nbRelocate = 6;
            for(int i = 0; i < nbRelocate; i++){
            //for(int i = 0; i < int(0.15 * nbNode); i++){
                //cout << "Obj before: " << sol.objective << endl;
                k1 = rand() % sol.nb_vehicle;
                do{
                    k2 = rand() % sol.nb_vehicle;
                }while(k1 == k2);
                
                if(sol.nbNodeInRoute[k1] < 3 || sol.nbNodeInRoute[k2] < 3){
                    continue;
                }
                pos1 = rand() % (sol.nbNodeInRoute[k1] - 2) + 1;
                pos2 = rand() % (sol.nbNodeInRoute[k2] - 1) + 1;
                node1 = sol.routes[k1][pos1];
                node2 = sol.routes[k2][pos2];

                if(sol.route_load[k2] + requests[node1].demand <= capacity){
                    int before1 = sol.routes[k1][pos1 - 1];
                    int after1 = sol.routes[k1][pos1 + 1];
                    int before2 = sol.routes[k2][pos2 - 1];

                    for(int l = sol.nbNodeInRoute[k2]; l > pos2; l--){
                        sol.routes[k2][l] = sol.routes[k2][l - 1];
                    }
                    sol.nbNodeInRoute[k2]++;
                    sol.routes[k2][pos2] = node1;

                    for(int l = pos1; l < sol.nbNodeInRoute[k1] - 1; l++){
                        sol.routes[k1][l] = sol.routes[k1][l + 1];
                    }
                    sol.nbNodeInRoute[k1]--;

                    sol.objective -= distanceNode[before1][node1] + distanceNode[node1][after1] + distanceNode[before2][node2]
                        - distanceNode[before1][after1] - distanceNode[before2][node1] - distanceNode[node1][node2];
            
                    sol.route_load[k2] += requests[node1].demand;
                    sol.route_load[k1] -= requests[node1].demand;
                }
            }
        }
    }

}

