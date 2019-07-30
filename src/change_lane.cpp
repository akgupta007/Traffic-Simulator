#include "init.h"

void copy_values(vehicle* vc1, vehicle* vc2){
    vc1->pos = vc2->pos;
    vc1->velocity = vc2->velocity;
    vc1->vehicle_type_no = vc2->vehicle_type_no;
    vc1->next = vc2->next;
}

int find_way(vehicle* vl, float pos, float length, vehicle_config* vcl, float pos_next){
    float dist;
    if(vl){
        if(vl->pos >= pos - length){
            return 0;
        }
        float vel = vl->velocity;
        float dist_reqd = vel*vel/(2*vcl[vl->vehicle_type_no-1].acceleration);
        dist = pos- length - vl->pos;
        if( dist < dist_reqd){
            return 0;
        }
    }
    dist = pos_next - pos;
    if(dist > 0){
        return dist;
    }
    return 0;
}

void change_lane(lane* ln, road_params road, vehicle_config* vcl){
    vehicle** current;
    vehicle** prev;
    road.length = road.length*2;
    int arr[road.width];
    current = new vehicle*[road.width];
    prev = new vehicle*[road.width];
    for (int i=0;i<road.width;i++){
        current[i] = ln[i].start;
        prev[i] = 0;
        if(ln[i].total_vehicles == 0){
            arr[i] = road.length;
        }
        else{
            arr[i] = current[i]->pos;
        }
    }

    int i = 0;
    for(int t=0;t<road.width;t++){
        if(arr[t] < arr[i]){
            i = t;
        }
    }
    while(arr[i] != road.length){
        float next_vehicle;
        if(current[i]->next){
            next_vehicle = current[i]->next->pos - vcl[current[i]->next->vehicle_type_no-1].length - current[i]->pos;
        }
        else{
            next_vehicle = road.length - current[i]->pos;
        }
        int poss1 = 0;
        int poss2 = 0;
        int l = 0;
        if( i-1 >= 0){
            if(arr[i-1] != road.length){
                l = vcl[current[i-1]->vehicle_type_no-1].length;
            }
            poss1 = find_way(prev[i-1], current[i]->pos, vcl[current[i]->vehicle_type_no-1].length, vcl, arr[i-1]- l);
        }
        if( i+1 < road.width){
            l = 0;
            if(arr[i+1] != road.length){
                l = vcl[current[i+1]->vehicle_type_no-1].length;
            }
            poss2 = find_way(prev[i+1], current[i]->pos, vcl[current[i]->vehicle_type_no-1].length, vcl, arr[i+1]- l);
        }
        if (poss1 >= poss2 && poss1 > next_vehicle + 5){
            vehicle* temp = (struct vehicle*) malloc (sizeof(struct vehicle));
            copy_values(temp, current[i]);
            copy_values(current[i], current[i]->next);
            temp->next = current[i-1];
            if(prev[i-1]){
                prev[i-1]->next = temp;
            }
            else{
                ln[i-1].start = temp;
            }
            ln[i].total_vehicles--;
            ln[i-1].total_vehicles++;
            current[i-1] = temp;
            arr[i-1] = temp->pos;
            arr[i] = current[i]->pos;
        }
        else if(poss2 > poss1 && poss2 > next_vehicle+5){
            vehicle* temp = (struct vehicle*) malloc (sizeof(struct vehicle));
            copy_values(temp, current[i]);
            copy_values(current[i], current[i]->next);
            temp->next = current[i+1];
            if(prev[i+1]){
                prev[i+1]->next = temp;
            }
            else{
                ln[i+1].start = temp;
            }
            ln[i].total_vehicles--;
            ln[i+1].total_vehicles++;
            current[i+1] = temp;
            arr[i+1] = temp->pos;
            arr[i] = current[i]->pos;
        }
        else{
            prev[i] = current[i];
            current[i] = current[i]->next;
            if(current[i]){
                arr[i] = current[i]->pos;
            }
            else{
                arr[i] = road.length;
            }
        }
        for(int t=0;t<road.width;t++){
            if(arr[t] < arr[i]){
                i = t;
            }
        }
    }

}