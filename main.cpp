// Our config
#include "config.h"

// std libs
#include <chrono/physics/ChMarker.h>
#include <string>

// chrono libs
#include <chrono/physics/ChBody.h>
#include <chrono/physics/ChSystemSMC.h>
#include <chrono/physics/ChMaterialSurfaceSMC.h>

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

std::shared_ptr<ChBody>
createGround(ChSystem &system) {
    double groundWidth = GROUND_WIDTH;
    double groundLength = GROUND_LENGTH;
    double groundHeight = GROUND_HEIGHT;

   // Create the body and its coordinate marker
    auto groundBody = std::make_shared<ChBody>();

    // Put the ground, on the ground!
    groundBody->SetPos(ChVector<>(0, -(groundHeight/2), 0));

    // Set the name of the ground
    groundBody->SetName("GROUND");

    // Make the ground not go anywhere
    groundBody->SetBodyFixed(true);

    // Create the surface material of the ground
    auto groundMaterial = std::make_shared<ChMaterialSurfaceSMC>();
    groundMaterial->SetFriction(GROUND_FRICTION_FORCE);

    // Establish a boxy collision surface. Because collision!
    groundBody->GetCollisionModel()->ClearModel(); // nuke default model
    groundBody->GetCollisionModel()->AddBox(groundMaterial, groundWidth, groundHeight,
            groundLength);  // chuck a box
    groundBody->GetCollisionModel()->BuildModel(); // compute model dims
    groundBody->SetCollide(true);                  // bounce! everybody bounce!

    // Establish a visualization shape
    auto groundVizShape = std::make_shared<ChBoxShape>();
    groundVizShape->GetBoxGeometry().Pos = ChVector<>(0, 0, 0);
    groundVizShape->GetBoxGeometry().Size = ChVector<>(groundWidth, groundHeight, groundLength);
    groundBody->AddAsset(groundVizShape);

    // Add the finished body to the system
    system.AddBody(groundBody);

    // Return the bearing marker
    return groundBody;
}

std::shared_ptr<ChBody> createBearing(ChSystem &system, double mass=BEARING_MASS, double radius=BEARING_RADIUS, double frictionForce=BEARING_FRICTION);
std::shared_ptr<ChBody> createBearing(ChSystem &system, double mass, double radius, double frictionForce) {
    static int id = 0;

    // Create the body and its coordinate marker
    auto bearingBody = std::make_shared<ChBody>();

    // Set the name of the bearing
    // TODO c++ string handling sucks deeply. Better way?
    bearingBody->SetName((std::string("BEARING ") + std::to_string(id++)).c_str());

    // Create the surface material of the bearing
    auto bearingMaterial = std::make_shared<ChMaterialSurfaceSMC>();
    bearingMaterial->SetFriction(frictionForce);

    // Establish a sphereical collision surface
    // Not quite sure how to add a custom shape yet?
    bearingBody->GetCollisionModel()->ClearModel(); // nuke default model
    bearingBody->GetCollisionModel()->AddSphere(bearingMaterial, radius); // chuck a circle
    bearingBody->GetCollisionModel()->BuildModel(); // compute model dims
    bearingBody->SetCollide(true); // bounce! everybody bounce!

    // Establish a visualization shape
    auto bearingVizShape = std::make_shared<ChSphereShape>();
    bearingVizShape->GetSphereGeometry().center = ChVector<>(0,0,0);
    bearingVizShape->GetSphereGeometry().rad = radius;
    bearingBody->AddAsset(bearingVizShape);

    auto bearingColor = chrono_types::make_shared<ChColorAsset>();
    bearingColor->SetColor(ChColor(0.01*(rand()%100), 0.01*(rand()%100), 0.01*(rand()%100)));
    bearingBody->AddAsset(bearingColor);

    // Set the mass of the system
    bearingBody->SetMass(mass);

    // Add the finished body to the system
    system.AddBody(bearingBody);

    // Return the bearing marker
    return bearingBody;
}

std::shared_ptr<ChBody> createLink(ChSystem &system, double mass=LINK_MASS, double radius=LINK_RADIUS, double length=LINK_LENGTH, double frictionForce=LINK_FRICTION);
std::shared_ptr<ChBody> createLink(ChSystem &system, double mass, double radius, double length, double frictionForce) {
    static int id = 0;

    // Create the body and its coordinate marker
    auto linkBody = std::make_shared<ChBody>();

    // Set the name of the bearing
    // TODO c++ string handling sucks deeply. Better way?
    linkBody->SetName((std::string("LINK ") + std::to_string(id++)).c_str());

    // Create the surface material of the link
    auto linkMaterial = std::make_shared<ChMaterialSurfaceSMC>();
    linkMaterial->SetFriction(frictionForce);

    // Establish a sphereical collision surface
    // Not quite sure how to add a custom shape yet?
    linkBody->GetCollisionModel()->ClearModel(); // nuke default model
    linkBody->GetCollisionModel()->AddCylinder(linkMaterial, radius, radius, length);
    linkBody->GetCollisionModel()->BuildModel(); // compute model dims
    linkBody->SetCollide(true); // bounce! everybody bounce!

    // Establish a visualization shape
    auto linkVizShape = std::make_shared<ChCylinderShape>();
    linkVizShape->GetCylinderGeometry().rad = radius;
    linkVizShape->GetCylinderGeometry().p1 = ChVector<>(0,(length/2),0);
    linkVizShape->GetCylinderGeometry().p2 = ChVector<>(0,-(length/2),0);
    linkBody->AddAsset(linkVizShape);

    auto linkColor = chrono_types::make_shared<ChColorAsset>();
    linkColor->SetColor(ChColor(0.01*(rand()%100), 0.01*(rand()%100), 0.01*(rand()%100)));
    linkBody->AddAsset(linkColor);

    // Set the mass of the system
    linkBody->SetMass(mass);

    // Add the finished body to the system
    system.AddBody(linkBody);

    // Return the bearing marker
    return linkBody;
}

typedef std::shared_ptr<ChBody> ChBodyPointer;

/**
 * Create one of those 0--0 pieces 
 *
 * @param {ChBodyPointer} positionedBearing    the bearing that you have placed where you want already
 * @param {ChBodyPointer} unpositionedBearing    the bearing that you want to connect, no need to set its position
 *
 * **BOTH OF THESE MUST BE ADDED TO THE SYSTEM ALREADY!!!!!!!!**
 * returns the body shared pointer representing the middle stick
 */

ChBodyPointer linkBearings(ChSystem &system, ChBodyPointer &positionedBearing, ChBodyPointer &unpositionedBearing, double seperation=LINK_LENGTH, double connectorMass=LINK_MASS, double connectorRadius=LINK_RADIUS, double connectorFrictionForce=LINK_FRICTION, double linkBearingPadding=LINK_PADDING);
ChBodyPointer linkBearings(ChSystem &system, ChBodyPointer &positionedBearing, ChBodyPointer &unpositionedBearing, double seperation, double connectorMass, double connectorRadius, double connectorFrictionForce, double linkBearingPadding) {
    // Move the unpositioned element up to the correct position
    auto currentPosition = positionedBearing->GetPos();
    unpositionedBearing->SetPos(ChVector<>(currentPosition.x(), currentPosition.y()+seperation+linkBearingPadding*2, currentPosition.z()));

    // Create the link cylinder
    auto linkBody = createLink(system, connectorMass, connectorRadius, seperation, connectorFrictionForce);

    // Set the position of the link cylinder
    linkBody->SetPos(ChVector<>(currentPosition.x(), currentPosition.y()+(seperation/2)+linkBearingPadding, currentPosition.z()));

    // Solder the bearings together with the links
    auto bottomSolder = std::make_shared<ChLinkLockLock>(); // bottom with link
    auto topSolder = std::make_shared<ChLinkLockLock>(); // top with link

    // Establish the links
    bottomSolder -> Initialize(positionedBearing, linkBody,
            ChCoordsys<>(ChVector<>(currentPosition.x(), currentPosition.y()+(linkBearingPadding/2), currentPosition.z())));
    topSolder -> Initialize(linkBody, unpositionedBearing,
            ChCoordsys<>(ChVector<>(currentPosition.x(), currentPosition.y()+seperation+((3*linkBearingPadding)/2), currentPosition.z())));

    // Add the constraints to the system
    system.Add(bottomSolder);
    system.Add(topSolder);

    return linkBody;
}

/**
 * Create one of those (0 0) pieces 
 *
 * @param {ChBodyPointer} positionedBearing    the bearing that you have placed where you want already
 * @param {ChBodyPointer} unpositionedBearing    the bearing that you want to connect, no need to set its position
 *
 * **BOTH OF THESE MUST BE ADDED TO THE SYSTEM ALREADY!!!!!!!!**
 * returns nada
 */

void groupBearings(ChSystem &system, ChBodyPointer &positionedBearing, ChBodyPointer &unpositionedBearing, double seperation=BEARING_PADDING);
void groupBearings(ChSystem &system, ChBodyPointer &positionedBearing, ChBodyPointer &unpositionedBearing, double seperation) {
    // Move the unpositioned element up to the correct position
    auto currentPosition = positionedBearing->GetPos();
    unpositionedBearing->SetPos(ChVector<>(currentPosition.x(), currentPosition.y()+seperation, currentPosition.z()));

    // Create the link that links the two objects together
    auto bearingsLink = std::make_shared<ChLinkLockSpherical>();

    // Establish the links
    bearingsLink -> Initialize(positionedBearing, unpositionedBearing,
            ChCoordsys<>(ChVector<>(currentPosition.x(), currentPosition.y()+(seperation/2), currentPosition.z())));

    // Add the constraint to the system
    system.Add(bearingsLink);
}

int main() {
    // Set the working data path
    SetChronoDataPath("./static");

    // Create the physical system we will be working in
    ChSystemSMC system;

    // Clear gravity
    //system.Set_G_acc(ChVector<>(0, 0, 0));

#if RUN_VISUALIZATION
    // Create the visualization application
    ChIrrApp application(&system);

    // Setup the visualization niceties and a default camera
    application.AddTypicalLights();
    application.AddTypicalCamera(vector3df(-0.1, 0.1, -0.1));
#endif



    /**
     * <Important>
     * @@@ Care about this bit that follows!! @@@
     *
     * The following is the only non-driver code in this
     * whole darn thing.
     */

    // Create a ground
    // TODO there is NOOOOO reaction forces emitted by
    // ground. we need a "ground ground" below the gorund
    // which will have a reaction force
    auto groundBody = createGround(system);

    auto prev = createBearing(system);
    prev->SetPos(ChVector<double>(0,0.01,0));

    for (int i=0; i<CHAIN_LENGTH; i++) {
        auto next1 = createBearing(system);
        linkBearings(system, prev, next1);

        auto next2 = createBearing(system);
        groupBearings(system, next1, next2);

        prev = next2;
    }

    /**
     **
     * </Important>
     */


#if RUN_VISUALIZATION
    // Bind all assets and render them to irrlicht
    application.AssetBindAll();
    application.AssetUpdateAll();
    // Set each animation step to the SIMULATION_RESOLUTION
    application.SetTimestep(SIMULATION_RESOLUTION);
    application.SetTryRealtime(true);
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

            // std::cout << "Time: " << system.GetChTime() << "\n" <<
            //	"x:" << bearingBody->GetPos().x() << " " <<
            //	"y:" << bearingBody->GetPos().y() << " " <<
            //	"z:" << bearingBody->GetPos().z() << " " <<
            //	"rot (" << bearingBody->GetRot().GetVector().x() <<
            //	"," << bearingBody->GetRot().GetVector().y() <<
            //	"," << bearingBody->GetRot().GetVector().z() << ")" << "\n\n";

#if RUN_VISUALIZATION
            application.DoStep();
            application.EndScene();
#else
            system.DoStepDynamics(SIMULATION_RESOLUTION);
#endif
        }

#endif
    }
