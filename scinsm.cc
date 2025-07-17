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
#include "PhysicsList.hh"

#include "G4RunManagerFactory.hh"
#include "G4Types.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include <ctime>

#include "G4AnalysisManager.hh"
#include "G4PhysListFactoryAlt.hh"
#include "G4PhysListRegistry.hh"
//#include "G4PhysListStamper.hh"
#include "G4PhysicsConstructorRegistry.hh"

//G4_DECLARE_PHYSLIST_FACTORY(PhysicsList);

namespace
{
void PrintAvailable(G4int verbosity)
{
  G4cout << G4endl;
  G4cout << "Here are the available physics lists:" << G4endl;
  g4alt::G4PhysListFactory factory;
  factory.PrintAvailablePhysLists();

  // if user asked for extra verbosity then print physics constructors as well
  if (verbosity > 1) {
    G4cout << G4endl;
    G4cout <<  "Here are the available physics constructors that can be added:" << G4endl;
    G4PhysicsConstructorRegistry* g4pctorFactory = G4PhysicsConstructorRegistry::Instance();
    g4pctorFactory->PrintAvailablePhysicsConstructors();
  }
}

void PrintUsage()
{
  G4cerr << " Usage: " << G4endl;
  G4cerr << " [-m macro ] [-p physList ]"
         << " [-t nThreads]" << G4endl << " [-v | --verbose] [-h | --help]"
         << G4endl;
  G4cerr << "   note: -t option is available only for multi-threaded mode." << G4endl;
  G4cerr << "   note: -v can be repeated to increase verbosity." << G4endl;
  G4cerr << G4endl;
}
}

//

int main(int argc, char** argv)
{
  if (argc > 13) {
    PrintUsage();
    return 1;
  }

  G4String macro;
  //G4String session;
  G4String physListName;
  char* physListNameEnv = nullptr;
  #ifdef G4MULTITHREADED
  G4int nofThreads = 6;
  #endif
  G4int verbosity = 0;

  for (G4int i = 1; i < argc; i = i + 2) {
    G4String g4argv(argv[i]);  // convert only once
    if (g4argv == "-m")
      macro = argv[i + 1];
    else if (g4argv == "-p")
      physListName = argv[i + 1];
#ifdef G4MULTITHREADED
    else if (g4argv == "-t") {
      nofThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
    }
#endif
    else if (g4argv == "-v" || g4argv == "--verbose") {
      ++verbosity;  // verbose flag doesn't take an argument
      --i;  // don't increment argc by two, just the one
    }
    else if (g4argv == "-h" || g4argv == "--help") {
      PrintUsage();
      return 1;
    }
    else {
      PrintUsage();
      return 1;
    }
  }

  auto runManager = G4RunManagerFactory::CreateRunManager();
  #ifdef G4MULTITHREADED
  if (nofThreads > 0)
  	runManager->SetNumberOfThreads(nofThreads);
  #endif

  g4alt::G4PhysListFactory factory;
  G4VModularPhysicsList* physList = nullptr;
  
  G4PhysListRegistry* plreg = G4PhysListRegistry::Instance();
  plreg->AddPhysicsExtension("Optical","G4OpticalPhysics");
  plreg->AddPhysicsExtension("SpinDecay","G4SpinDecayPhysics");
  //
  if (verbosity > 0) {
    G4cout << "Adding extensions" << G4endl
           << "   Optical     	===> G4OpticalPhysics" << G4endl << G4endl
           << "   SpinDecay    ===> G4SpinDecayPhysics" << G4endl << G4endl;
  }
  
  if (physListName.size()) {
    if (verbosity > 0) {
      G4cout << "Explicitly using '" << physListName << "'" << G4endl;
    }
    physList = factory.GetReferencePhysList(physListName);
  }
  else {
    if (verbosity > 0) {
      G4cout << "There is no -p flag;"
             << " using default" << G4endl;
    }
    physList = new PhysicsList();

    if (!physList) {
      // failed?  get what the user set, but we couldn't find
      physListNameEnv = std::getenv("PHYSLIST");
      if (physListNameEnv) {
        G4cout << "$PHYSLIST=" << physListNameEnv << G4endl;
      }
    }
  }
  
  if (!physList) {
    G4cerr << "===================== PhysicsList '"
           << (physListNameEnv ? physListNameEnv : physListName)
           << "' was not available in g4alt::PhysListFactory." << G4endl;
    PrintAvailable(verbosity);
  }


  G4int seed = time(nullptr);
  G4Random::setTheSeed(seed);
  G4cout << G4endl << " ===================================================== " << G4endl
         << " Initial seed = " << seed << G4endl
         << " ===================================================== " << G4endl << G4endl;

  // Detector construction
  auto detector = new DetectorStructure();
  runManager->SetUserInitialization(detector);
  //Physics List
  runManager->SetUserInitialization(physList);

  // User action initialization
  runManager->SetUserInitialization(new ActionInitial(detector));
  
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  // Instantiate G4UIExecutive if interactive mode
  G4UIExecutive* ui = nullptr;
  if (!macro.size()) {
    ui = new G4UIExecutive(argc, argv);
  }
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  if (!macro.size()) {
    // Define (G)UI terminal for interactive mode
    if (ui->IsGUI()) UImanager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
  }
  else {
  		 
  	 G4String command = "/control/execute ";
    	 UImanager->ApplyCommand(command + macro);
    	
       }
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->CloseFile();
  // job termination
  delete visManager;
  delete runManager;
  return 0;
}

//
