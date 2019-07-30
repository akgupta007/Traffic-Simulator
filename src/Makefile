CC=g++-8
CFlags=-c
glewFLAG = -lGLEW -lstdc++
glfwFLAG = -lglfw -lGL
main: main.o update_vel.o add_vehicle.o remove_vehicle.o tmove.o parse.o update_traffic.o
	$(CC) -o simulation main.o update_vel.o add_vehicle.o remove_vehicle.o tmove.o parse.o update_traffic.o $(glfwFLAG) $(glewFLAG)
main.o: main.cpp init.h
	$(CC) $(CFlags) main.cpp $(glfwFLAG) $(glewFLAG)
update_vel.o: update_vel.cpp init.h
	$(CC) $(CFlags) update_vel.cpp
add_vehicle.o: add_vehicle.cpp init.h
	$(CC) $(CFlags) add_vehicle.cpp
remove_vehicle.o: remove_vehicle.cpp init.h
	$(CC) $(CFlags) remove_vehicle.cpp
tmove.o: tmove.cpp init.h
	$(CC) $(CFlags) tmove.cpp
parse.o: parse.cpp init.h
	$(CC) $(CFlags) parse.cpp
update_traffic.o: update_traffic.cpp init.h
	$(CC) $(CFlags) update_traffic.cpp

clean: 
	rm -rf *o simulation
