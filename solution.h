struct Solution{
    int nb_vehicle = 0;
    int routes[max_nb_route][max_nb_node]; 
    int route_load[max_nb_route];
    int nbNodeInRoute[max_nb_node];
    int objective = 0;

    int getObjective(int [][max_node]);
    void displayRoute();
};

int Solution::getObjective(int distanceNode[][max_node]){
    int result = 0;
    for(int k = 0; k < nb_vehicle; k++){
        for(int l = 0; l < nbNodeInRoute[k] - 1; l++){
            int node1 = routes[k][l];
            int node2 = routes[k][l + 1];
            result += distanceNode[node1][node2];
        }
    }
    return result;
}

void Solution::displayRoute(){
    int count_node = 0;
    for(int k = 0; k < nb_vehicle; k++){
        printf("Route %2d: %d - ", k + 1, nbNodeInRoute[k]);
        for(int l = 0; l < nbNodeInRoute[k]; l++){
            printf("%4d", routes[k][l]);
            if(routes[k][l] != 0){
                count_node++;
            }
        }
        printf("  -  %d\n", route_load[k]);
    }
    printf("Number service request: %d\n\n", count_node);
}