#include "init.h"
#include <random>
using namespace std;

void update_velocity(vehicle*& root, int stop, vehicle_config* vcl, road_params& road){
    vehicle* current = root;
    while(current != NULL){
        current = current->next;
    }
}

/*
void update_velocity(lane& ln, int stop, vehicle_config* vcl, road_params& road){
    if(ln.total_vehicles == 0){
        return;
    }
    vehicle* current = ln.start;
    while(current){
        if(current->next == 0){
            if(stop){
                float dist = road.length - current->pos;
                float vel = current->velocity;
                float dist_reqd = vel*vel/(2*vcl[current->vehicle_type_no-1].acceleration);
                if (dist <= dist_reqd){
                    current->velocity = current->velocity - vcl[current->vehicle_type_no-1].acceleration;
                }
                else{
                    current->velocity = current->velocity + vcl[current->vehicle_type_no-1].acceleration;
                }
            }
            else{
                current->velocity = current->velocity + vcl[current->vehicle_type_no-1].acceleration;
            }
        }
        else{
            float dist = current->next->pos - vcl[current->next->vehicle_type_no-1].length - current->pos-5;
            float vel = current->velocity;
            float dist_reqd = vel*vel/(2*vcl[current->vehicle_type_no-1].acceleration);
            if (dist < dist_reqd){
                current->velocity = current->velocity - vcl[current->vehicle_type_no-1].acceleration;
            }
            else{
                current->velocity = current->velocity + vcl[current->vehicle_type_no-1].acceleration;
            }
        }
        if(current->velocity <0){
            current->velocity = 0;
        }
        if(current->velocity > vcl[current->vehicle_type_no-1].max_vel){
            current->velocity = vcl[current->vehicle_type_no-1].max_vel;
        }
        current = current->next;
    }
}
*/