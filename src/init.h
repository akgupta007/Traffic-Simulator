#pragma once

#include <string>
#include <vector>
using namespace std;
// inside parse file
struct vehicle_config {
	string type;
	float width;
	float length;
	float max_vel;
	float acceleration;
	float prob;
	string color;
};


struct road_params{
	int length;
	int width;
};

struct traffic{
	float entering_fq;
	int overtake_prob;
};

// inside main
struct vehicle{
    float xpos = 0;
    float ypos = 0;
    float width = 0;
    float length = 0;
    float acceleration = 0;
    float velocity = 1.0;
    int vehicle_type_no = 0;
    float direction = 0;
    vehicle* next = NULL;
    vehicle* prev = NULL;
};



// functions definition
vehicle_config* setConfig(string& filename, int& total_no_of_vehicles, road_params& road, traffic& Traffic, int& run_time, vector<vector<int>> &traffic_control);
void traffic_move(vehicle*& root, int stop, vehicle_config* vcl, road_params road);
void add_new_vehicle(vehicle*& root, int type, vehicle_config* v_config, road_params& road, int ypos);
void remove_vehicle(vehicle*& root, road_params road);
//void change_lane(lane* ln, road_params road, vehicle_config* vcl);
void update_velocity(vehicle*& root, int stop, vehicle_config* vcl, road_params& road);
void update_traffic(vehicle*& root, traffic Traffic, int count, road_params& road, vehicle_config* vcl, float weight[], int stop, int number, int ypos, int ran);
