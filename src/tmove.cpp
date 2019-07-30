#include "init.h"
#include <iostream>
using namespace std;
int lookUp(vehicle* root, road_params road, int stop){
    int flag = 0;
    vehicle* current = root->next;
    vehicle* ahead = NULL;
    float dist = road.length + 400 - root->xpos;
    float vel = root->velocity;
    float dist_reqd = vel*vel/(root->acceleration)+20;
    int vehicle_ahead = 0;
    int no_up = 0;
    int no_down = 0;
    while(current != NULL && current->ypos < root->ypos + root->width+10){
        if(current->xpos - current->length > root->xpos){
            vehicle_ahead = 1;
            float d = current->xpos - current->length - root->xpos;
            if(d < dist) {dist = d; ahead = current;}
        }
        else{
            float v = current->velocity;
            float safe_dist = v*v/(2*current->acceleration)+20;
            if(safe_dist > root->xpos - root->length - current->xpos){
                no_down = 1;
            }
        }
        current = current->next;
    }

    current = root->prev;
    while(current != NULL && current->ypos > root->ypos - 80){
        if(current->ypos + current->width > root->ypos - 10){
            if (current->xpos - current->length > root->xpos){
                vehicle_ahead = 1;
                float d = current->xpos - current->length - root->xpos;
                if(d < dist) {dist = d; ahead = current;}
            }

            else{
                float v = current->velocity;
                float safe_dist = v*v/(2*current->acceleration)+20;
                if(safe_dist > root->xpos - root->length - current->xpos){
                    no_up = 1;
                }
            }
        }
        current = current->prev;
    }

    if(vehicle_ahead){
        if (dist < dist_reqd+50){
            float root_center = root->ypos + root->width/2;
            float ahead_center = ahead->ypos + ahead->width/2;
            if(ahead_center >= root_center){
                flag = 1;
            }
            else{
                flag = -1;
            }
        }
        if(dist <= dist_reqd){
            root->velocity -= root->acceleration/2;
        }
        else{
            root->velocity += root->acceleration;
        }
    }
    else{
        if (stop){
            float temp_d = road.length - root->xpos;
            if(temp_d+80 > dist_reqd && temp_d+40 < dist_reqd && temp_d+80>0){
                root->velocity -= root->acceleration/2;
            }
            else{
                root->velocity += root->acceleration;
            }
        }
        else{
            root->velocity += root->acceleration;
        }
        
    }
    if(root->velocity < 0){
        root->velocity = 0;
    }
    if(flag == 1){
        if(root->ypos < 10 || no_up) flag = 0;
    }
    else if(flag == -1){
        if(root->ypos + root->width > road.width*80-10 || no_down) flag = 0;
    }
    if(flag == 1){
        root->direction += 0.5;
        if(root->direction > 15){
            root->direction = 15;
        }
    }
    else if(flag == -1){
        root->direction -= 0.5;
        if(root->direction < -15){
            root->direction = -15;
        }
    }
    else{
        root->direction /= 1.5;
    }
    return flag;
}

void traffic_move(vehicle*& root, int stop, vehicle_config* vcl, road_params road){
    vehicle* current = root;
    while(current != NULL){
        int flag = lookUp(current, road, stop);
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
        if(flag == -1){
            current->ypos += 1;
            vehicle* temp = current->next;
            while(temp != NULL && temp->ypos < current->ypos){
                current->next = temp->next;
                temp->prev = current->prev;
                temp->next = current;
                current->prev = temp;
                if (temp->prev) temp->prev->next = temp;
                if (current->next) current->next->prev = current;
                if(!temp->prev){
                    root = temp;
                }
                temp = current->next;
            }
        }
        current->xpos += current->velocity;
        current = current->next;
    }
}