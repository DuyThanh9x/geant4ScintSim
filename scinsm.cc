// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//Addapted from example/extended/optical/wls/

#include "FTFP_BERT.hh"
#include "ActionInitial.hh"
#include "DetectorStructure.hh"

#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"
#include "G4RunManagerFactory.hh"
#include "G4Types.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include <ctime>
#include "G4DecayPhysics.hh"
#include "G4ParticleTable.hh"

//

int main(int argc, char** argv)
{
  // Instantiate G4UIExecutive if interactive mode
  G4UIExecutive* ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  auto runManager = G4RunManagerFactory::CreateRunManager();
  //#ifdef G4MULTITHREADED
  	//runManager->SetNumberOfThreads(1);
  //#endif
  G4int seed = 123;
  if (argc > 2) seed = atoi(argv[argc - 1]);

  // Choose the Random engine and set the seed
  // G4Random::setTheEngine(new CLHEP::RanecuEngine);
  //G4Random::setTheSeed(seed);
  G4Random::setTheSeed(time(nullptr));

  // Detector construction
  auto detector = new DetectorStructure();
  runManager->SetUserInitialization(detector);

  // Physics list
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
  auto opticalPhysics = new G4OpticalPhysics();
  //auto decayPhysics = new G4DecayPhysics();
  
  /*auto partIterator = new G4ParticleTable :: G4ParticleTableIterator() -> GetParticleIterator();
  partIterator->Reset();
  while( (*partIterator)() ){
	G4ParticleDefinition* particle = partIterator->value();
	G4ProcessManager* pmanager = particle->GetProcessManager();
	if (decayPhysics->IsApplicable(*particle)) {
		pmanager ->AddProcess(decayPhysics);
		// set ordering for PostStepDoIt and AtRestDoIt
		pmanager ->SetProcessOrdering(decayPhysics, idxPostStep);
		pmanager ->SetProcessOrdering(decayPhysics, idxAtRest);
	}
  }*/

  auto opticalParams = G4OpticalParameters::Instance();
  opticalParams->SetBoundaryInvokeSD(true);

  opticalParams->SetWLSTimeProfile("exponential");
  //opticalParams->SetScintYieldFactor(1);
  //opticalParams->SetScintTrackSecondariesFirst(true);
  //opticalParams->SetScintByParticleType(true);
  physicsList->RegisterPhysics(opticalPhysics);
  //physicsList->RegisterPhysics(decayPhysics);
  // Add Decay Process
  //G4Decay* theDecayProcess = new G4Decay();
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new ActionInitial(detector));

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  G4int nEvents = -1;
  G4String fileName = "";
  if (ui) {
    // Define (G)UI terminal for interactive mode
    if (ui->IsGUI()) UImanager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
  }
  else {
  		for (int i = 1; i < argc ; i++) {
  			if ((std::string(argv[i]) == "--run") && (i+1 < argc))
  				nEvents = std::stoi(argv[++i]);
  		 else if (std::string(argv[i]).rfind(".mac") != std::string::npos)
  				fileName = std::string(argv[i]);
  		}
  		if (!fileName.empty()) {
  			G4String command = "/control/execute ";
    			UImanager->ApplyCommand(command + fileName);
    			}
    		if(nEvents>0)
    			runManager->BeamOn(nEvents);
    	
    }
  
  
  
    

  // job termination
  delete visManager;
  delete runManager;
  return 0;
}

//
