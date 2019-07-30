#include "init.h"
#include <iostream>
using namespace std;
void remove_vehicle(vehicle*& root, road_params road){
    vehicle* current = root;
    while(current!= NULL){
        if(current->xpos - current->length > road.length + 110){
            if(current->prev != NULL) current->prev->next = current->next;
            if(current->next != NULL) current->next->prev = current->prev;
            if(root == current) root = current->next;
        }
        current = current->next;
    }
}