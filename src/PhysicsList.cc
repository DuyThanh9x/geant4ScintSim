#include "PhysicsList.hh"
#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4SpinDecayPhysics.hh"
#include "G4PhysListFactory.hh"

#include "G4StepLimiterPhysics.hh"

PhysicsList::PhysicsList()
  : G4VModularPhysicsList()
{
	SetVerboseLevel(1);
	/*G4PhysListFactory factory;
  	 factory.ReferencePhysList();
  	
  	for (G4int i = 0;; ++i) {
    		auto elem = const_cast<G4VPhysicsConstructor*>(physicsList->GetPhysics(i));
    		if (elem == nullptr) break;
    		G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
    		RegisterPhysics(elem);
  	}*/
  	physicsList = new FTFP_BERT;
  	physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
  	physicsList->ReplacePhysics(new G4SpinDecayPhysics());
  	physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  	
  	//+++++++++++++++ OpticalPhysics +++++++++++++++++++++++++++++++++++++
	//RegisterPhysics (new G4OpticalPhysics());
	//auto opticalParams = G4OpticalParameters::Instance();
  	//opticalParams->SetBoundaryInvokeSD(true);
  	//opticalParams->SetWLSTimeProfile("exponential");
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructParticle()
{
	physicsList->ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
	physicsList->ConstructProcess();
}

void PhysicsList::SetCuts()
{
  SetCutsWithDefault();
}


