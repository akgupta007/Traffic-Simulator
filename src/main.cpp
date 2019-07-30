#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <random>
#include <cmath>
#include <ctime>
#include <chrono>
#include "init.h"
using namespace std;

#define PI 3.14159265

void character_callback( GLFWwindow *window, unsigned int codepoint );
void charmods_callback( GLFWwindow *window, unsigned int codepoint, int mods );

traffic Traffic;
vehicle_config* vehicles;
string sel = "freq";
string type = "freq";


vector<float> get_vertices(vehicle* root, float y_shift){
    float cos_theta = cos( root->direction * PI / 180);
    float sin_theta = sin( root->direction * PI / 180);
    float x_c = root->xpos - root->length / 2;
    float y_c = y_shift - root->ypos - root->width/2;
    float x_1 = x_c + root->length*cos_theta/2 - root->width * sin_theta/2;
    float y_1 = y_c + root->length*sin_theta/2 + root->width*cos_theta/2;
    float x_4 = x_c + root->length*cos_theta/2 + root->width*sin_theta/2;
    float y_4 = y_c + root->length * sin_theta/2 - root->width*cos_theta/2;
    float x_3 = 2*x_c - x_1;
    float y_3 = 2*y_c - y_1;
    float x_2 = 2*x_c - x_4;
    float y_2 = 2*y_c - y_4;
    vector<float> temp = {x_1,y_1,0.0, x_2,y_2,0.0, x_3,y_3,0.0, x_4,y_4,0.0};
    return temp;
}
vector<float> get_position(vehicle* root, vehicle_config* vcl, float y_shift){
    vector<float> vertice;
    vehicle* current = root;
    while(current){
        float length = current->length;
        float width = current->width;
        float x = current->xpos;
        float y = y_shift - current->ypos;
        vector<float> temp = get_vertices(current, y_shift);
        vertice.insert(vertice.end(), temp.begin(), temp.end());
        current = current->next;
    }
    return vertice;
}

vector<float> get_color(vehicle* root, vehicle_config* vcl){
    vector<float> color;
    vehicle* current = root;
    while(current){
        int type = current->vehicle_type_no;
        string clr = vcl[type-1].color;
        vector<float> temp;
        if (clr == "red") temp = { 255,0,0.0, 255,0,0.0, 255,0,0.0, 255,0,0.0};
        if (clr == "green") temp = { 0,255,0.0, 0,255,0.0, 0,255,0.0, 0,255,0.0};
        if (clr == "blue") temp = { 0,0,255, 0,0,255, 0,0,255, 0,0,255};
        if (clr == "black") temp = { 0,0,0, 0,0,0, 0,0,0, 0,0,0};
        if (clr == "grey") temp = {0.2,0.2,0.2, 0.2,0.2,0.2, 0.2,0.2,0.2, 0.2,0.2,0.2};
        if (clr == "white") temp = {127,127,127, 127,127,127, 127,127,127, 127,127,127};
        
        color.insert(color.end(), temp.begin(), temp.end());
        current = current->next;
    }
    return color;
}

int main( void )
{
    string filename = "config.txt";
    int count = 0;
    int run_time = 0;
    road_params road;
    vector<vector<int>> traffic_control;
    vehicles = setConfig(filename, count, road, Traffic, run_time, traffic_control);

    float bar = 20;
    float SCREEN_WIDTH = road.length+20 + 110;
    float SCREEN_HEIGHT = road.width*80+200+bar;

    vehicle* root = NULL;
    int i=0;
    int stop = 0;

    GLFWwindow *window;
    
    // Initialize the library
    if ( !glfwInit( ) )
    {
        return -1;
    }
    
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "ROAD SIMULATOR", NULL, NULL );
    
    glfwSetCharModsCallback( window, charmods_callback );

    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent( window );
    
    float x = 0;
    float y = 0;
    
    glViewport( 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT ); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode( GL_PROJECTION ); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity( ); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1 ); // essentially set coordinate system
    glMatrixMode( GL_MODELVIEW ); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity( ); // same as above comment
    int t = 0;
    vector<int> temp_v = traffic_control.back();
    traffic_control.pop_back();
    float start;
    start = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
    float end;
    int ran = 0;
    // Loop until the user closes the window
    while ( !glfwWindowShouldClose( window ))
    {
        vector<float> vertice;
        vector<float> color;

        if(t%1000 == 0){
            stop = 1;
        }
        if(t%1000 == 400){
            stop = 0;
        }
        float offset = 100;
        //int center = 40;
        float weights[count+1];
        weights[0] = (road.width/Traffic.entering_fq)-1;
        for(int i=1;i<=count;i++){
            weights[i] = vehicles[i-1].prob;
        }
        int vcl_number = 0;
        int vcl_ypos = 0;
        end = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
        cout<<end - start<<endl;
        int curr_time = (int) end - start;
        if(((curr_time)%temp_v[2] < 25) && ((curr_time)/temp_v[2] == 1)){
            vcl_number = temp_v[0];
            vcl_ypos = temp_v[1];
            if(traffic_control.empty()){
                temp_v[0] = 0;
            }
            else{
                temp_v = traffic_control.back();
                traffic_control.pop_back();
            }
        }
        if (curr_time > 4500){
            ran = 1;
        }
        update_traffic(root, Traffic, count, road, vehicles, weights, stop, vcl_number, vcl_ypos, ran);
        vector<float> v = get_position(root, vehicles, road.width*80 + offset);
        vertice.insert(vertice.end(), v.begin(), v.end());
        vector<float> c = get_color(root, vehicles);
        color.insert(color.end(), c.begin(), c.end());
        //center += 80;

        int size = vertice.size()/3;

        glClear( GL_COLOR_BUFFER_BIT );
        glClearColor ( 0.5, 0.5, 0.5, 0.0 ) ;
        glColor3f (1.0, 1.0, 1.0);

        glPushAttrib(GL_ENABLE_BIT);

        glEnable(GL_LINE_STIPPLE); 
        glLineStipple(1, 0xFFFF);
        glLineWidth(3.0f);
        glBegin(GL_LINES);
        glVertex3f(0.0,offset,0.0);
        glVertex3f(road.length,offset,0.0);
        glEnd();

        for (int i=0; i<road.width-1; i++){
            glLineStipple(2, 0x00FF);
            glBegin(GL_LINES);
            glVertex3f(0.0,(i+1)*80+offset,0.0);
            glVertex3f(road.length,(i+1)*80 + offset,0.0);
            glEnd();
        }

        glLineStipple(1, 0xFFFF);

        glBegin(GL_LINES);
        glVertex3f(0.0,road.width*80+offset,0.0);
        glVertex3f(road.length,road.width*80+offset,0.0);
        glEnd();

        glLineWidth(20.0f);
        
        //pedestrian line
        glBegin(GL_LINES);
            glVertex3f(road.length+10, offset, 0.0);
            glVertex3f(road.length+10, road.width*80+offset, 0.0);
        glEnd();
        glBegin(GL_LINES);
            glVertex3f(road.length+10+80, offset, 0.0);
            glVertex3f(road.length+10+80, road.width*80+offset, 0.0);
        glEnd();
        //traffic light
        glLineWidth(50.0f);

        if(stop) glColor3f(1.0, 0.0, 0.0);
        else glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
            glVertex3f(road.length+10+95, offset, 0.0);
            glVertex3f(road.length+10+95, road.width*80+offset, 0.0);
        glEnd();


        glColor3f(0.0, 0.5, 0.0);
         glBegin(GL_POLYGON);
          glVertex3f(SCREEN_WIDTH, offset-10, 0.0);
          glVertex3f(0.0, offset-10, 0.0);
          glVertex3f(0.0, 0.0, 0.0);
          glVertex3f(SCREEN_WIDTH, 0.0, 0.0);
         glEnd();
         
         glBegin(GL_POLYGON);
          glVertex3f(SCREEN_WIDTH, SCREEN_HEIGHT-bar, 0.0);
          glVertex3f(0.0, SCREEN_HEIGHT-bar, 0.0);
          glVertex3f(0.0, SCREEN_HEIGHT-bar- offset+10, 0.0);
          glVertex3f(SCREEN_WIDTH, SCREEN_HEIGHT-bar - offset+10, 0.0);
         glEnd();


        glPopAttrib();
        // Render OpenGL here
        glEnableClientState( GL_VERTEX_ARRAY ); // tell OpenGL that you're using a vertex array for fixed-function attribute
        glEnableClientState( GL_COLOR_ARRAY );
        glVertexPointer( 3, GL_FLOAT, 0, &vertice.front() ); // point to the vertices to be used
        glColorPointer( 3, GL_FLOAT, 0, &color.front() );
        glDrawArrays( GL_QUADS, 0, size ); // draw the vertixes
        glDisableClientState( GL_COLOR_ARRAY );
        glDisableClientState( GL_VERTEX_ARRAY ); // tell OpenGL that you're finished using the vertex arrayattribute
        
        
        // Swap front and back buffers
        glfwSwapBuffers( window );
        
        // Poll for and process events
        glfwPollEvents( );
        t++;
    }
    
    glfwTerminate( );
    
    return 0;
}


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