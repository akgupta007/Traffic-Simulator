#include "init.h"
#include <random>
#include <stdlib.h>
#include <time.h>
#include <bits/stdc++.h>
using namespace std;

void update_traffic(vehicle*& root, traffic Traffic, int count, road_params& road, vehicle_config* vcl, float weight[], int stop, int number, int ypos, int ran){
    traffic_move(root, stop, vcl, road);
    if (ran){
        random_device rd;
        mt19937 gen(rd());
        vector<float> weights(weight,weight+count+1);
        
        discrete_distribution<int> distribution(begin(weights), end(weights));
        number = distribution(gen);
        srand (time(NULL));
        int range = (int) road.width*80 - 20 - vcl[number-1].width;
        ypos = rand() % (range) + 10;
    }
    add_new_vehicle(root, number, vcl, road, ypos);
    remove_vehicle(root, road);
    return;
}
