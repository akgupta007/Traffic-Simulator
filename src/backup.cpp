#include "init.h"
#include <iostream>
using namespace std;
int lookUp(vehicle* root, road_params road){
    int flag = 1;
    vehicle* current = root->next;
    float dist = road.length + 400 - root->xpos;
    float vel = root->velocity;
    float dist_reqd = vel*vel/(2*root->acceleration);
    int vehicle_ahead = 0;
    while(current != NULL && current->ypos < root->ypos + root->width+5){
        if(current->xpos - current->length > root->xpos){
            vehicle_ahead = 1;
            float d = current->xpos - current->length - root->xpos;
            if(d < dist) dist = d;
            if ((current->acceleration > root->acceleration)) {flag = 0;}
        }
        current = current->next;
    }

    current = root->prev;
    float width = root->width;
    while(current != NULL && current->ypos > root->ypos - 80){
        if (current->xpos - current->length > root->xpos){
            if(current->ypos + current->width > root->ypos + 5){
                vehicle_ahead = 1;
                //if(current->ypos + current->width/2 < root->ypos)
                if (current->xpos - current->length - root->xpos <= dist) {
                    if(current->ypos + current->width/2 > root->ypos + 5) flag = 0;
                    dist = current->xpos - current->length - root->xpos;
                    break;
                }
            }
        }
        if (current->xpos < root->xpos - root->length){
            if(current->ypos + current->width > root->ypos - 5){
                float vel = current->velocity;
                float min_dist = vel*vel/(2*current->acceleration);
                if(root->xpos - root->length - current->xpos < min_dist) {flag = 0;}
            }
        }
        current = current->prev;
    }
    if (dist_reqd >= dist) root->velocity -= root->acceleration;
    else root->velocity += root->acceleration;
    if(root->ypos + root->width > road.width*80-10 || root->ypos < 10) return 0;
    if(!vehicle_ahead) return 0;
    return flag;
}

void traffic_move(vehicle*& root, int stop, vehicle_config* vcl, road_params road){
    vehicle* current = root;
    while(current != NULL){
        int flag = lookUp(current, road);
        if(flag == 1){
            current->ypos -= 1;
            vehicle* temp = current->prev;
            while(temp != NULL && temp->ypos > current->ypos){
                temp->next = current->next;
                current->prev = temp->prev;
                current->next = temp;
                temp->prev = current;
                if (current->prev) current->prev->next = current;
                if (temp->next) temp->next->prev = temp;
                if(!current->prev){
                    root = current;
                }
                temp = current->prev;
            }
        }
        /*else{
            flag = lookDown(current);
            if(flag == 1){
                current->ypos -= 1;
            }
        }*/
        current->xpos += current->velocity;
        current = current->next;
    }
}



/*
    if(stop){
        vehicle* current = ln.start;
        while(current){
            if(!current->next){
                if(current->pos >= road.length-4 - current->velocity && current->pos <= road.length-4){
                    return;
                }
                current->pos = current->pos + current->velocity;
                return;
            }
            else if(current->pos <= current->next->pos - vcl[current->next->vehicle_type_no-1].length - 10 - current->velocity){
                current->pos = current->pos + current->velocity;
            }
            current = current->next;
        }
        
    }
    else{
        vehicle* current = ln.start;
        while(current){
            current->pos = current->pos + current->velocity;
            current = current->next;
        }   
    }
}
*/