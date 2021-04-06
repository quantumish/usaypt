// Our config
#include "config.h"

// std libs
#include <string>

// chrono libs
#include <chrono/physics/ChBody.h>
#include <chrono/physics/ChSystemNSC.h>
#include <chrono/physics/ChMaterialSurfaceNSC.h>

// chrono visulization libs
#include <chrono_irrlicht/ChIrrApp.h>

// Use the namespaces of Chrono
using namespace chrono;

/*
 * (#!/Condution | At Home) TODO to resolve
 * - Figure out what unit of measurement Chrono uses. 
 *   metres and seconds and radians assumed
 *
 */

/*
 * Points of order
 * - Objects seem to fall in the y direction
 *
 */

std::shared_ptr<ChMarker> createGround(ChSystem &system) {
    // Create the body and its coordinate marker
    auto groundBody = std::make_shared<ChBody>();
    auto groundMarker = std::make_shared<ChMarker>();

    // Add the marker to the body
    groundBody->AddMarker(groundMarker);

    // Put the ground, on the groud!
    groundMarker->Impose_Abs_Coord(ChCoordsys<>(ChVector<double>(0, 0, 0)));
    
    // Make the ground not go anywhere
    groundBody->SetBodyFixed(true);

    // Set the name of the bearing marker 
    // TODO c++ string handling sucks deeply. Better way?
    groundMarker->SetName("GROUND marker");

    // Create the surface material of the ground
    auto groundMaterial = std::make_shared<ChMaterialSurfaceNSC>();
    groundMaterial->SetFriction(0.5);

    // Establish a boxy collision surface. Because
    groundBody->GetCollisionModel()->ClearModel(); // nuke default model
    groundBody->GetCollisionModel()->AddBox(groundMaterial, 1000000, 5, 10000000); // chuck a box
    groundBody->GetCollisionModel()->BuildModel(); // compute model dims
    groundBody->SetCollide(true); // bounce! everybody bounce!

    // Add the finished body to the system
    system.AddBody(groundBody);

    // Return the bearing marker
    return groundMarker;
}

std::shared_ptr<ChMarker> createBearing(ChSystem &system, int id) {
    // Create the body and its coordinate marker
    auto bearingBody = std::make_shared<ChBody>();
    auto bearingMarker = std::make_shared<ChMarker>();

    // Add the marker to the body
    bearingBody->AddMarker(bearingMarker);

    // Set the name of the bearing marker 
    // TODO c++ string handling sucks deeply. Better way?
    bearingMarker->SetName((std::string("BEARING marker ") + std::to_string(id)).c_str());

    // Create the surface material of the bearing
    auto bearingMaterial = std::make_shared<ChMaterialSurfaceNSC>();
    bearingMaterial->SetFriction(0.5);

    // Establish a sphereical collision surface
    // Not quite sure how to add a custom shape yet?
    bearingBody->GetCollisionModel()->ClearModel(); // nuke default model
    bearingBody->GetCollisionModel()->AddSphere(bearingMaterial, 0.01); // chuck a circle
    bearingBody->GetCollisionModel()->BuildModel(); // compute model dims
    bearingBody->SetCollide(true); // bounce! everybody bounce!

    // Add the finished body to the system
    system.AddBody(bearingBody);

    // Return the bearing marker
    return bearingMarker;
}

int main() {
    // Create the physical system we will be working in
    ChSystemNSC system;

    // Create a ground
    createGround(system);

    // Create a temp bearing that's ID = 0
    auto bearingMarker = createBearing(system, 0);
    
    // Let's move this bearing up
    // A NOTE for why Impose_Abs_Coord is used and not
    // SetAbsCoord. That, my friend, is because 
    // Impose_Abs_Coord sets the coordinates now AND 
    // sets that as the "resting position" for the object.
    //
    // Unlike SetAbsCoord, a function does not clear momentum
    // and friends.

    bearingMarker->Impose_Abs_Coord(ChCoordsys<>(ChVector<double>(0, 10000, 0)));

    // Print the hierachy to the logger
    system.ShowHierarchy(GetLog());

    #if START_SIMULATION
    while (system.GetChTime() < 100) {
        GetLog() << "Time: " << system.GetChTime() << "\n" <<
            "x:" << bearingMarker->GetAbsCoord().pos.x() << " " <<
            "y:" << bearingMarker->GetAbsCoord().pos.y() << " " <<
            "z:" << bearingMarker->GetAbsCoord().pos.z() << " " <<
            "rot (" << bearingMarker->GetAbsCoord().rot.GetVector().x() << 
            "," << bearingMarker->GetAbsCoord().rot.GetVector().y() << 
            "," << bearingMarker->GetAbsCoord().rot.GetVector().z() << ")" << "\n\n";

        system.DoStepDynamics(0.01);
    }
    #endif
}

