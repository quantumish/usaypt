#pragma once

/**
 * Do we start simulation?
 * For 99.999% of the use case, this should be left on.
 * However, there are 1% chance where I want it off 
 * for debugging
 */

#define RUN_SIMULATION 1

/**
 * Do we run the visulization?
 * Sometimes, I just want to see the debug output w/o irrlicht.
 */

#define RUN_VISUALIZATION 0

/**
 * What is the temporal resolution of visulization/sims?
 * What does each "step" mean, in seconds?
 */

#define SIMULATION_RESOLUTION 0.02

/**
 * What is the max duration of the animation?
 */

#define SIMULATION_DURATION_MAX 5000


