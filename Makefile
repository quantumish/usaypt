build:
	g++ sim.cpp /usr/lib/libChronoEngine.so -I /usr/include/eigen3 -I /usr/include/chrono/collision/bullet/ -o sim
