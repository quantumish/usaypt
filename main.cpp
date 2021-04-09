// Our config
#include "config.h"

// std libs
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
  // Create the body and its coordinate marker
  auto groundBody = std::make_shared<ChBody>();

  // Put the ground, on the groud!
  groundBody->SetPos(ChVector<>(0, 0, 0));

  // Make the ground not go anywhere
  groundBody->SetBodyFixed(true);

  // Set the name of the  ground
  groundBody->SetName("GROUND");

  // Create the surface material of the ground
  auto groundMaterial = std::make_shared<ChMaterialSurfaceSMC>();
  groundMaterial->SetFriction(0);

  // Establish a boxy collision surface. Because collision!
  groundBody->GetCollisionModel()->ClearModel(); // nuke default model
  groundBody->GetCollisionModel()->AddBox(groundMaterial, 100, 1,
										  100);  // chuck a box
  groundBody->GetCollisionModel()->BuildModel(); // compute model dims
  groundBody->SetCollide(true);                  // bounce! everybody bounce!

  // Establish a visualization shape
  auto groundVizShape = std::make_shared<ChBoxShape>();
  groundVizShape->GetBoxGeometry().Pos = ChVector<>(0, 0, 0);
  groundVizShape->GetBoxGeometry().Size = ChVector<>(100, 1, 100);
  groundBody->AddAsset(groundVizShape);

  // Add the finished body to the system
  system.AddBody(groundBody);

  // Return the bearing marker
  return groundBody;
}

std::shared_ptr<ChBody> createBearing(ChSystem &system, double mass = 5, double radius=1, double frictionForce=0.01);
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

	auto mfloorcolor = chrono_types::make_shared<ChColorAsset>();
	mfloorcolor->SetColor(ChColor(0.01*(rand()%100), 0.01*(rand()%100), 0.01*(rand()%100)));
	bearingBody->AddAsset(mfloorcolor);


	// Set the mass of the system
	bearingBody->SetMass(mass);

	// Add the finished body to the system
	system.AddBody(bearingBody);

	// Return the bearing marker
	return bearingBody;
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
	application.AddTypicalCamera(vector3df(-100, 100, -100));
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

	// Let's move this bearing up
	// A NOTE for why Impose_Abs_Coord is used and not
	// SetAbsCoord. That, my friend, is because
	// Impose_Abs_Coord sets the coordinates now AND
	// sets that as the "resting position" for the object.
	//
	// Unlike SetAbsCoord, a function does not clear momentum
	// and friends.

	auto prev = createBearing(system, 1, 1, 1);
	prev->SetPos(ChVector<double>(0,5,0));
	for (int i = 1; i < 30; i++) {
		auto bearingBody = createBearing(system, 1, 1, 1);
		bearingBody->SetPos(ChVector<double>(0,5+(10*i),0));
		auto link = std::make_shared<ChLinkLockLock>();
		link->Initialize(prev, bearingBody,
						 ChCoordsys<>(ChVector<>(0, 10*i, 0)));
		auto bearingVizShape = std::make_shared<ChCylinderShape>();
		bearingVizShape->GetCylinderGeometry().rad = 0.3;
		bearingVizShape->GetCylinderGeometry().p1 = ChVector<>(0,-5,0);
		bearingVizShape->GetCylinderGeometry().p2 = ChVector<>(0,5,0);
		link->AddAsset(bearingVizShape);
		system.Add(link);
		prev=bearingBody;
	}
		/**
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
