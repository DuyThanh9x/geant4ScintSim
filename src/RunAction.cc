//
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
//

#include "RunAction.hh"

#include "DetectorStructure.hh"
#include "Run.hh"
#include "SteppingAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"
#include "G4ProcessManager.hh"
#include "G4MuonMinus.hh"
#include "globals.hh"

//

RunAction :: RunAction(SteppingAction* steppingAction) : G4UserRunAction(), SteppingAct (steppingAction)
{
  auto analysisManager = G4AnalysisManager::Instance();
  if (isMaster) {
  analysisManager->SetNtupleMerging(true);
  
  analysisManager->SetDefaultFileType("root");
  G4AnalysisManager::Instance()->OpenFile("scinsm");
  }
  analysisManager->SetVerboseLevel(1);

  /*analysisManager->CreateH1("Energy", "Energy of optical photon", 100, 2. * CLHEP::eV,
                            3.2 * CLHEP::eV);
  analysisManager->CreateH1("Time", "Arrival time", 100, 0., 100. * CLHEP::ns);
  analysisManager->CreateH1("Number of photons", "Number of photons", 100, 0., 100.);
  
  analysisManager->CreateNtuple("MudEdX","Average Mu Energy deposit");
  analysisManager->CreateNtupleDColumn("MudEdX");
    analysisManager->FinishNtuple();
  analysisManager->CreateNtuple("PositronEnergy","Positron Energy");
  analysisManager->CreateNtupleDColumn("PosdEdX");
  //analysisManager->FinishNtuple();
  //analysisManager->CreateNtuple("ElecKineticE","Electron Kinetic Energy");
  analysisManager->CreateNtupleDColumn("PosE");
  analysisManager->FinishNtuple();*/
  
  
  analysisManager->CreateH1("Direction","Positron angular distribution",70,-1,1);
  analysisManager->SetH1XAxisTitle(0,"cos#theta");
  analysisManager->SetH1YAxisTitle(0,"#events");
  
}

RunAction::~RunAction() {
}

//

G4Run* RunAction::GenerateRun()
{
  return new Run();
}

//

void RunAction :: BeginOfRunAction (const G4Run* aRun)
{
  G4cout << "========= RunID: "<< aRun->GetRunID() << " starts!" << G4endl;
  
  if (SteppingAct) {
    SteppingAct->Initialize();
    }
}

//

void RunAction :: EndOfRunAction (const G4Run* aRun)
{
  const Run* run = static_cast<const Run*>(aRun);
  if (run == nullptr) return;
  auto analysisManager = G4AnalysisManager::Instance();
  
  analysisManager->Write();
  
  if (isMaster) run->EndOfRun();
}
