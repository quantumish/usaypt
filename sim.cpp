#include <chrono/physics/ChBody.h>
#include <chrono/assets/ChBoxShape.h>
#include <chrono/physics/ChBodyEasy.h>
#include <chrono/physics/ChSystemSMC.h>
#include <chrono/assets/ChVisualization.h>

int main() {
    chrono::ChSystemSMC system;

    auto body = std::make_shared<chrono::ChBody>();

    auto mbox = std::make_shared<chrono::ChBoxShape>();

    mbox->GetBoxGeometry().Pos = chrono::ChVector<>(0,-1,0);
    mbox->GetBoxGeometry().Size = chrono::ChVector<>(10,0.5,10);
    body->AddAsset(mbox); 

    body->SetMass(10000);
    body->SetInertiaXX(chrono::ChVector<int>(4,4,4));

    system.Add(body);

    chrono::SetChronoDataPath("./testdata");

    system.SetTimestepperType(chrono::ChTimestepper::Type::EULER_IMPLICIT);
    system.SetSolverType(chrono::ChSolver::Type::PSOR);

    while (system.GetChTime() < 10) {
        system.DoStepDynamics(0.02);
        std::cout << body->GetPos() << "\n";
    }
}

