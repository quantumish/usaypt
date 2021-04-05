#include <string>
#include <chrono/physics/ChBody.h>
#include <chrono_irrlicht/ChIrrApp.h>

#include <chrono/physics/ChSystemNSC.h>

#include "config.h"

// Use the namespaces of Chrono
using namespace chrono;

std::shared_ptr<ChMarker> createBearing(ChSystem &system, int id) {
    // Create the body and its coordinate marker
    auto bearing_body = std::make_shared<ChBody>();
    auto bearing_marker = std::make_shared<ChMarker>();

    // Add the marker to the body
    bearing_body->AddMarker(bearing_marker);

    // Set the name of the bearing marker 
    // TODO c++ string handling sucks deeply. Better way?
    bearing_marker->SetName((std::string("BEARING marker ") + std::to_string(id)).c_str());

    // Add the finished body to the system
    system.AddBody(bearing_body);

    // Return the bearing marker
    return bearing_marker;
}

void simulate(ChSystem &system, double upTo) {
    while (system.GetChTime() < upTo) {
        system.DoStepDynamics(0.1);

        GetLog() << "hewo" << "\n";
    }
}

int main() {
    // Create the physical system we will be working in
    ChSystemNSC system;

    // Create a temp bearing that's ID = 12
    createBearing(system, 12);

    // Print the hierachy to the logger
    system.ShowHierarchy(GetLog());

    simulate(system, 10);
}

