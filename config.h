#pragma once

// I am pretty sure that all of the units are in metric. 
// Scale's like seconds, centimetres, kilograms, radians newtons, 
// and joules.

////////////      Chain Properties      ///////////////

/**
 * What should the distance between a link and a bearing be?
 */

#define LINK_PADDING 0.1

/**
 * What should the distance between bearings in the big container be?
 */

#define BEARING_PADDING 1.5


/**
 * How long should the chain be? 0-0 = one length
 */

#define CHAIN_LENGTH 5



////////////   Link Object Properties   ///////////////

/**
 * How heavy should the link be?
 */

#define LINK_MASS 1

/**
 * How long should the link be?
 */

#define LINK_LENGTH 1

/**
 * What should the radius of the link be?
 */

#define LINK_RADIUS 0.1

/**
 * What should the friction of the link surface be?
 */

#define LINK_FRICTION 0.001



////////////  Bearing Object Properties  ///////////////

/**
 * How heavy should the bearing be?
 */

#define BEARING_MASS 1

/**
 * What should the radius of the bearing be?
 */

#define BEARING_RADIUS 0.5

/**
 * What should the friction of the bearing surface be?
 */

#define BEARING_FRICTION 0.001



////////////  Ground Object Properties  ///////////////

/**
 * How wide shall ye make the ground?
 */

#define GROUND_WIDTH 10

/**
 * How long shall ye make the ground?
 */

#define GROUND_LENGTH 10

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

