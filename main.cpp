#include <string>
#include <chrono/physics/ChBody.h>
#include <chrono_irrlicht/ChIrrApp.h>

#include <chrono/physics/ChSystemNSC.h>

#include "config.h"

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

    // But the ground, on the groud!
    groundMarker->Impose_Abs_Coord(ChCoordsys<>(ChVector<double>(0, 0, 0)));
    
    // Make the ground not go anywhere
    groundBody->SetBodyFixed(true);

    // Set the name of the bearing marker 
    // TODO c++ string handling sucks deeply. Better way?
    groundMarker->SetName("GROUND");

    // Establish a boxy collision surface. Because
    groundBody->GetCollisionModel()->AddBox(nullptr, 1000, 5, 1000);

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

    // Establish a sphereical collision surface
    // Not quite sure how to add a custom shape yet?
    bearingBody->GetCollisionModel()->AddSphere(nullptr, 0.01);

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
}

