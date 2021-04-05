#include <chrono/physics/ChBody.h>
#include <chrono/assets/ChBoxShape.h>
#include <chrono/physics/ChSystemSMC.h>
#include <chrono/assets/ChVisualization.h>
#include <chrono_irrlicht/ChIrrApp.h>
#include <chrono_irrlicht/ChIrrAppInterface.h>
#include <chrono_irrlicht/ChIrrTools.h>

/*
 * Hewo there, fellow human
 * This is a smoke test for the project
 * also it is used to remind ourselves how
 * to write stuff down.
 *
 * The CMake file should compile this into
 * ./smoke, which you could run. The stdout
 * should show a point falling through space,
 * whereas the visualization should show
 * a blank irrlicht canvas.
 *
 */



// Use the namespaces of Chrono
using namespace chrono;
using namespace chrono::irrlicht;

// Use the main namespaces of Irrlicht
using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::io;
using namespace irr::gui;

int main() {
    chrono::ChSystemSMC system;

    auto body = std::make_shared<ChBody>();

    auto mbox = std::make_shared<ChBoxShape>();

    mbox->GetBoxGeometry().Pos = ChVector<>(0,-1,0);
    mbox->GetBoxGeometry().Size = ChVector<>(10,0.5,10);
    body->AddAsset(mbox); 

    body->SetMass(10000);
    body->SetInertiaXX(ChVector<int>(4,4,4));

    system.Add(body);

    SetChronoDataPath("./testdata");

    system.SetTimestepperType(ChTimestepper::Type::EULER_IMPLICIT);
    system.SetSolverType(ChSolver::Type::PSOR);


    // 4- Create the Irrlicht visualization
    ChIrrApp application(&system);

    // Bind assets
    application.AssetBindAll();
    application.AssetUpdateAll();

    tools::add_typical_Logo(application.GetDevice());
    tools::add_typical_Sky(application.GetDevice());
    tools::add_typical_Lights(application.GetDevice());
    tools::add_typical_Camera(application.GetDevice(), core::vector3df(0, 0, -6));


    // Timer for enforcing soft real-time
    ChRealtimeStepTimer realtime_timer;
    double time_step = 0.01;

    bool removed = false;

    while (system.GetChTime() < 10) {
        // Irrlicht must prepare frame to draw
        application.BeginScene(true, true, SColor(255, 140, 161, 192));

        // Irrlicht now draws simple lines in 3D world representing a
        // skeleton of the mechanism, in this instant:
        //
        // .. draw items belonging to Irrlicht scene, if any
        application.DrawAll();
        // .. draw a grid
        tools::drawGrid(application.GetVideoDriver(), 0.5, 0.5);
        // .. draw GUI items belonging to Irrlicht screen, if any
        application.GetIGUIEnvironment()->drawAll();

        system.DoStepDynamics(0.02);
        std::cout << body->GetPos() << "\n";
        application.EndScene();
    }
}

