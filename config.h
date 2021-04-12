#pragma once

// I am pretty sure that all of the units are in metric. 
// Scale's like seconds, metres, kilograms, radians newtons, 
// and joules.

////////////  Bearing Object Properties  ///////////////

/**
 * How heavy should the bearing be?
 */

#define BEARING_WEIGHT 0.001

/**
 * What should the radius of the bearing be?
 */

#define BEARING_RADIUS 0.005


////////////  Ground Object Properties  ///////////////

/**
 * How wide shall ye make the ground?
 */

#define GROUND_WIDTH 100

/**
 * How long shall ye make the ground?
 */

#define GROUND_LENGTH 100

/**
 * How thick shall ye make the ground?
 */

#define GROUND_HEIGHT 0.1

/**
 * How friction-y should the ground be?
 *
 * F_static = F_kinetic = GROUND_FRICTION_FORCE
 */

#define GROUND_FRICTION_FORCE 0



//////////////// Simulation Settings /////////////////

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

#if RUN_SIMULATION
#define RUN_VISUALIZATION 1
#endif

/**
 * What is the temporal resolution of visulization/sims?
 * What does each "step" mean, in seconds?
 *
 * If weird behavior occurs in the visulization, its probably
 * because of the fact that this number is too large. I find
 * that 0.01 for SMC sims seems to work well.
 */

#define SIMULATION_RESOLUTION 0.01

/**
 * What is the max duration of the animation?
 */

#define SIMULATION_DURATION_MAX 5000

