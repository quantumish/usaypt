#include <chrono/physics/ChBody.h>
#include <chrono/physics/ChBodyEasy.h>
#include <chrono/physics/ChSystemSMC.h>

int main()
{
    chrono::ChSystemSMC system;
    auto body = std::make_shared<chrono::ChBody>();
    body->SetMass(10000);
    body->SetInertiaXX(chrono::ChVector<int>(4,4,4));
    system.Add(body);
    system.SetTimestepperType(chrono::ChTimestepper::Type::EULER_IMPLICIT);
    system.SetSolverType(chrono::ChSolver::Type::PSOR);
    while (system.GetChTime() < 10) {
	system.DoStepDynamics(0.02);
	std::cout << body->GetPos() << "\n";
    }
}

