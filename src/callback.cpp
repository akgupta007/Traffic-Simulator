
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "init.h"
using namespace std;


string sel = "freq";
string type = "freq";
void character_callback( GLFWwindow *window, unsigned int codepoint )
{
    std::cout << codepoint << std::endl;
}

void charmods_callback( GLFWwindow *window, unsigned int codepoint, int mods )
{
    if(codepoint >= 48 && codepoint <58){
        sel = to_string(codepoint - 48);
    }
    else if(codepoint == 102){
        sel = "freq";
    }
    else if(codepoint == 97){
        type = "accel";
    }
    else if(codepoint == 122){
        type = "freq";
    }
    else if(codepoint == 113){
        type = "speed";
    }
    if(codepoint == 119){
        if (sel == "freq"){
                Traffic.entering_fq += 0.01;
            }
        else if(stoi(sel) >= 0){
            if(type == "accel"){
                        vehicles[stoi(sel)].acceleration+=0.01;
                    }
            else if(type == "speed"){
                        vehicles[stoi(sel)].max_vel+=0.1;
                    }
            else{
                vehicles[stoi(sel)].prob+=0.02;   
            }
            cout<< type <<" increase for vehicle: "<<sel<<endl;
        }
    }
    else if(codepoint == 115){
        
        if (sel == "freq"){
                Traffic.entering_fq -= 0.01;
                if(Traffic.entering_fq < 0){
                    Traffic.entering_fq = 0;
                }
            }
        else{
            if(type == "accel"){
                vehicles[stoi(sel)].acceleration -= 0.01;
                if(vehicles[stoi(sel)].acceleration < 0){
                    vehicles[stoi(sel)].acceleration = 0;
                }
            }
            else if(type == "speed"){
                vehicles[stoi(sel)].max_vel-=0.1;
                if(vehicles[stoi(sel)].max_vel < 0){
                    vehicles[stoi(sel)].max_vel = 0;
                }
            }
            else{
                vehicles[stoi(sel)].prob -= 0.02;
                if(vehicles[stoi(sel)].prob < 0){
                    vehicles[stoi(sel)].prob = 0;
                }
            }
            cout<< type <<" decrease for vehicle: "<<sel<<endl;

        }
    }
    if ( mods == 1 )
    {
        std::cout << "Shift key pressed aswell" << std::endl;
    }
}