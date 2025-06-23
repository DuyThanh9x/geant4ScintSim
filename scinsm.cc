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
#include "G4PhysListFactory.hh"

//

int main(int argc, char** argv)
{
  auto runManager = G4RunManagerFactory::CreateRunManager();
  //#ifdef G4MULTITHREADED
  	//runManager->SetNumberOfThreads(1);
  //#endif
  G4int seed = time(nullptr);
  G4Random::setTheSeed(seed);
  G4cout << G4endl << " ===================================================== " << G4endl
         << " Initial seed = " << seed << G4endl
         << " ===================================================== " << G4endl << G4endl;

  // Detector construction
  auto detector = new DetectorStructure();
  runManager->SetUserInitialization(detector);

  // Physics list
  G4PhysListFactory factory;
  G4VModularPhysicsList* physicsList = factory.ReferencePhysList();
  physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
  auto opticalPhysics = new G4OpticalPhysics();
  
  auto opticalParams = G4OpticalParameters::Instance();
  opticalParams->SetBoundaryInvokeSD(true);

  opticalParams->SetWLSTimeProfile("exponential");
  
  physicsList->RegisterPhysics(opticalPhysics);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new ActionInitial(detector));
  

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  // Instantiate G4UIExecutive if interactive mode
  G4UIExecutive* ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  G4String fileName = "";
  if (ui) {
    // Define (G)UI terminal for interactive mode
    if (ui->IsGUI()) UImanager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
  }
  else {
  		 
  	 G4String command = "/control/execute ";
  	 fileName = argv[1];
    	 UImanager->ApplyCommand(command + fileName);
    	
       }
  // job termination
  delete visManager;
  delete runManager;
  return 0;
}

//
