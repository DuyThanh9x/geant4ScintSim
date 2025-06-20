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

//

RunAction :: RunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  if (isMaster) {
  analysisManager->SetNtupleMerging(true);
  }
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetVerboseLevel(1);

  analysisManager->CreateH1("Energy", "Energy of optical photon", 100, 2. * CLHEP::eV,
                            3.2 * CLHEP::eV);
  analysisManager->CreateH1("Time", "Arrival time", 100, 0., 100. * CLHEP::ns);
  analysisManager->CreateH1("Number of photons", "Number of photons", 100, 0., 100.);
  
  analysisManager->CreateNtuple("MudEdX","Average Mu Energy deposit");
  analysisManager->CreateNtupleDColumn("MudEdX");
    analysisManager->FinishNtuple();
  analysisManager->CreateNtuple("ElecdEdX","Average Electron Energy deposit");
  analysisManager->CreateNtupleDColumn("ElecdEdX");
  analysisManager->FinishNtuple();
}

//

G4Run* RunAction::GenerateRun()
{
  fRun = new Run();
  return fRun;
}

//

void RunAction :: BeginOfRunAction (const G4Run*)
{
  G4AnalysisManager::Instance()->OpenFile("scinsm");
}

//

void RunAction :: EndOfRunAction (const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  if (analysisManager->GetH1(0)) {
    G4cout << " +++++++++++++++ print histograms statistics ++++++++++++++++++ "<< G4endl;
    if (isMaster) {
      G4cout << "for the entire run "  << G4endl << G4endl;
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl;
    }

    G4cout << " Mean number of photons detected/event: " << analysisManager->GetH1(2)->mean()
           << " rms = " << analysisManager->GetH1(2)->rms() << G4endl;
  }

  analysisManager->Write();
  analysisManager->CloseFile();
  
  if (isMaster) fRun->EndOfRun();
}
