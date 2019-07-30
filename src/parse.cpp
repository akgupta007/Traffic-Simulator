#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include "init.h"
using namespace std;

vehicle_config* setConfig(string& filename, int& total_no_of_vehicles, road_params& road, traffic& Traffic, int& run_time, vector<vector<int>> &traffic_control) {
	ifstream stream(filename);
	if(!stream){
		cout<< "Could not open file";
		return 0;
	}
	string line;
	int input = 0;
	vehicle_config* vehicles = new vehicle_config[5];
	int count = -1;
	while (getline(stream, line)) {
		if (line.find("#Number of vehicles") != string::npos){
			auto index = line.find_last_of(' ');
			total_no_of_vehicles = stoi(line.substr(++index));
		}
		else if (line.find("#vehicles config") != string::npos) {
			input = 1;
		}
		else if (line.find("#Road Parameters") != string::npos) {
			input = 0;
		}
		else if (line.find("#Traffic Config") != string::npos) {
			input = 2;
		}
		else if (line.find("#RUN TIME") != string::npos) {
			auto index = line.find_last_of(' ');
			run_time = stoi(line.substr(++index));
		}
		else if (line.find("#Traffic Control") != string::npos) {
			input = 3;
		}
		else if (input == 1) {
			string data;
			string eq;
			stringstream ss(line);
			ss >> data;
			if (data == "type") {
				ss >> eq >> data;
				vehicles[++count].type = data;
			}
			else if (data == "width") {
				ss >> eq >> data;
				vehicles[count].width = stof(data);
			}
			else if (data == "length") {
				ss >> eq >> data;
				vehicles[count].length = stof(data);
			}
			else if (data == "acceleration") {
				ss >> eq >> data;
				vehicles[count].acceleration = stof(data);
			}
			else if (data == "Max_Velocity") {
				ss >> eq >> data;
				vehicles[count].max_vel = stof(data);
			}
			else if (data == "probability") {
				ss >> eq >> data;
				vehicles[count].prob = stof(data);
			}
			else if (data == "color") {
				ss >> eq >> data;
				vehicles[count].color = data;
			}
		}
		else if (input == 0){
			string data;
			string eq;
			stringstream ss(line);
			ss >> data;
			if (data == "Road_length"){
				ss >> eq >> road.length;
			}
			else if (data == "Road_width"){
				ss >> eq >> road.width;
			}
		}
		else if(input == 2){
			string data;
			string eq;
			stringstream ss(line);
			ss >> data;
			if (data == "overtake_probability"){
				ss >> eq >> Traffic.overtake_prob;
			}
			else if (data == "Entering_frequency"){
				ss >> eq >> data;
				Traffic.entering_fq = stof(data);
			}
		}
		else if(input == 3){
			string number;
			string pos;
			string tym;
			stringstream ss(line);
			ss >> number >> pos >> tym;
			vector<int> v{stoi(number), stoi(pos), stoi(tym)};
			traffic_control.push_back(v);
		}
	}
	count = count+1;
	stream.close();
	return vehicles;
}
