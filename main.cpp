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

// Use the main namespaces of Irrlicht
using namespace irrlicht;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::io;
using namespace irr::gui;

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

std::shared_ptr<ChBody> createGround(ChSystem &system) {
    // Create the body and its coordinate marker
    auto groundBody = std::make_shared<ChBody>();

    // Put the ground, on the groud!
    groundBody->SetPos(ChVector<>(0,0,0));
    
    // Make the ground not go anywhere
    groundBody->SetBodyFixed(true);

    // Set the name of the  ground
    groundBody->SetName("GROUND");

    // Create the surface material of the ground
    auto groundMaterial = std::make_shared<ChMaterialSurfaceNSC>();
    groundMaterial->SetFriction(0);

    // Establish a boxy collision surface. Because collision!
    groundBody->GetCollisionModel()->ClearModel(); // nuke default model
    groundBody->GetCollisionModel()->AddBox(groundMaterial, 10, 0, 10); // chuck a box
    groundBody->GetCollisionModel()->BuildModel(); // compute model dims
    groundBody->SetCollide(true); // bounce! everybody bounce!

    // Establish a visualization shape
    auto groundVizShape = std::make_shared<ChBoxShape>();
    groundVizShape->GetBoxGeometry().Pos = ChVector<>(0,0,0); 
    groundVizShape->GetBoxGeometry().Size = ChVector<>(10, 0, 10);
    groundBody->AddAsset(groundVizShape); 

    // Add the finished body to the system
    system.AddBody(groundBody);

    // Return the bearing marker
    return groundBody;
}

std::shared_ptr<ChBody> createBearing(ChSystem &system, int id) {
    // Create the body and its coordinate marker
    auto bearingBody = std::make_shared<ChBody>();

    // Set the name of the bearing 
    // TODO c++ string handling sucks deeply. Better way?
    bearingBody->SetName((std::string("BEARING ") + std::to_string(id)).c_str());

    // Create the surface material of the bearing
    auto bearingMaterial = std::make_shared<ChMaterialSurfaceNSC>();
    bearingMaterial->SetFriction(0);

    // Establish a sphereical collision surface
    // Not quite sure how to add a custom shape yet?
    bearingBody->GetCollisionModel()->ClearModel(); // nuke default model
    bearingBody->GetCollisionModel()->AddSphere(bearingMaterial, 0.5); // chuck a circle
    bearingBody->GetCollisionModel()->BuildModel(); // compute model dims
    bearingBody->SetCollide(true); // bounce! everybody bounce!

    // Establish a visualization shape
    auto bearingVizShape = std::make_shared<ChSphereShape>();
    bearingVizShape->GetSphereGeometry().center = ChVector<>(0,0,0); 
    bearingVizShape->GetSphereGeometry().rad = 0.5; 
    bearingBody->AddAsset(bearingVizShape); 

    // Add the finished body to the system
    system.AddBody(bearingBody);

    // Return the bearing marker
    return bearingBody;
}

int main() {
    // Set the working data path
    SetChronoDataPath("./static");

    // Create the physical system we will be working in
    ChSystemNSC system;

    // Clear gravity
    //system.Set_G_acc(ChVector<>(0, 0, 0));

    #if RUN_VISUALIZATION
    // Create the visualization application
    ChIrrApp application(&system);

    // Setup the visualization niceties and a default camera
    application.AddTypicalLights();
    application.AddTypicalCamera(vector3df(-20, 20, -20));
    #endif



    /**
     * <Important>
     * @@@ Care about this bit that follows!! @@@
     *
     * The following is the only non-driver code in this
     * whole darn thing.
     */

    // Create a ground
    auto groundBody = createGround(system);

    // Create a temp bearing that's ID = 0
    auto bearingBody = createBearing(system, 0);
    
    // Let's move this bearing up
    // A NOTE for why Impose_Abs_Coord is used and not
    // SetAbsCoord. That, my friend, is because 
    // Impose_Abs_Coord sets the coordinates now AND 
    // sets that as the "resting position" for the object.
    //
    // Unlike SetAbsCoord, a function does not clear momentum
    // and friends.

    bearingBody->SetPos(ChVector<double>(0,10,0));

    // TODO the createBearing function should set the weight, but for nowwe need a weight
    bearingBody->SetMass(25);

    // Apply a force on the ball   
    //                             
    //                             
    // Punch the ground up       // force vector       // where to apply     // is force w.r.t. object or w.r.t. system?
    //bearingBody->Accumulate_force(ChVector<>(30, 0, -30), bearingBody->GetPos(), false);



    /**
     * </Important>
     */



    #if RUN_VISUALIZATION
    // Bind all assets and render them to irrlicht
    application.AssetBindAll();
    application.AssetUpdateAll();
    // Set each animation step to the SIMULATION_RESOLUTION
    application.SetTimestep(SIMULATION_RESOLUTION);
    #endif

    // Print the hierachy to the logger
    system.ShowHierarchy(GetLog());

    #if RUN_SIMULATION

    #if RUN_VISUALIZATION
    while (application.GetDevice()->run() && system.GetChTime() < SIMULATION_DURATION_MAX) {
        application.BeginScene(true, true, SColor(255, 140, 161, 192));
        application.DrawAll();
    #else
    while (system.GetChTime() < SIMULATION_DURATION_MAX) {
    #endif

        std::cout << "Time: " << system.GetChTime() << "\n" <<
            "x:" << bearingBody->GetPos().x() << " " <<
            "y:" << bearingBody->GetPos().y() << " " <<
            "z:" << bearingBody->GetPos().z() << " " <<
            "rot (" << bearingBody->GetRot().GetVector().x() << 
            "," << bearingBody->GetRot().GetVector().y() << 
            "," << bearingBody->GetRot().GetVector().z() << ")" << "\n\n";

    #if RUN_VISUALIZATION
        application.DoStep();
        application.EndScene();
    #else
        system.DoStepDynamics(SIMULATION_RESOLUTION);
    #endif
    }

    #endif
}

