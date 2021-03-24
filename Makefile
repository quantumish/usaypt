build:
	g++ sim.cpp /usr/lib/libChronoEngine.so /usr/lib/libChronoEngine_irrlicht.so -I /usr/include/irrlicht -I /usr/include/eigen3 -I /usr/include/chrono/collision/bullet/ -o sim
