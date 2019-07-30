#include "init.h"
#include <cmath>
using namespace std;

void add_new_vehicle(vehicle*& root, int type, vehicle_config* vcl, road_params& road, int ypos){
    if(type == 0) return;

    vehicle* current = root;
    vehicle* new_vcl = (struct vehicle*) malloc (sizeof(struct vehicle));
    new_vcl->vehicle_type_no = type;
    new_vcl->ypos = ypos;
    new_vcl->xpos = 0;
    new_vcl->acceleration = vcl[type-1].acceleration;
    new_vcl->width = vcl[type-1].width;
    new_vcl->length = vcl[type-1].length;
    new_vcl->velocity = 1.0;
    new_vcl->direction = 0.0;
    new_vcl->next = NULL;
    new_vcl->prev = NULL;
    if(root == NULL){
        root = new_vcl;
        return;
    }
    while(current != NULL){
        if(current->xpos - current->length < 0){
            if (current->ypos < ypos + 5 + vcl[type-1].width && current->ypos + current->width + 5 > ypos) return;
        }
        current = current->next;
    }
    current = root;
    while(current != NULL){
        if (current->ypos > ypos){
            new_vcl->prev = current->prev;
            if(current->prev != NULL) current->prev->next = new_vcl;
            new_vcl->next = current;
            current->prev = new_vcl;
            if(current == root) root = current->prev;
            return;
        }
        if(current->next == NULL){
            current->next = new_vcl;
            new_vcl->prev = current;
            return;
        }
        current = current->next;
    }

}


/*void add_new_vehicle(lane& ln, int type, vehicle_config* v_config, road_params& road){
    if (type == 0){
        return;
    }
    if (ln.total_vehicles == 0){
        vehicle* new_vehicle = (struct vehicle*) malloc (sizeof(struct vehicle));
        new_vehicle->vehicle_type_no = type;
        new_vehicle->pos = 0;

        float dist = road.length - 5;
        float vel ;
        float acc = v_config[type-1].acceleration;
        vel = 2*acc*dist;
        vel = sqrt(vel);
        if (vel > v_config[type-1].max_vel){
            new_vehicle->velocity = v_config[type-1].max_vel;
        }
        else{
            new_vehicle->velocity = vel;
        }
        
        new_vehicle->next = 0;
        ln.start = new_vehicle;
        ln.total_vehicles++;
        return;
    }
    else{
        float len = v_config[ln.start->vehicle_type_no-1].length;
        if(ln.start->pos <= len+10){
            return;
        }
        vehicle* old = (struct vehicle*) malloc (sizeof(struct vehicle));
        old->pos = ln.start->pos;
        old->velocity = ln.start->velocity;
        old->next = ln.start->next;
        old->vehicle_type_no = ln.start->vehicle_type_no;
        ln.start->pos = 0;
        
        float dist = old->pos - v_config[old->vehicle_type_no-1].length - 5;
        float vel ;
        float acc = v_config[type-1].acceleration;
        vel = 2*acc*dist;
        vel = sqrt(vel);
        ln.start->velocity = vel;
        ln.start->vehicle_type_no = type;
        ln.start->next = old;
        ln.total_vehicles++;
    }
}
*/