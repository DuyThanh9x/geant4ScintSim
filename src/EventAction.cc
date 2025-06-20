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

#include "EventAction.hh"
#include "SteppingAction.hh"

#include "EventActionMess.hh"
#include "MPPCHit.hh"
#include "ScintillatorsHit.hh"
#include "Run.hh"
#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "Randomize.hh"



EventAction :: EventAction()
{
  fEventMessenger = new EventActionMess(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction :: ~EventAction()
{
  delete fEventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
  fNTIR = 0;
  fNExiting = 0;
  fEscapedEnd = 0;
  fEscapedSide = 0;
  fBounce = 0;
  fWLSBounce = 0;
  fClad1Bounce = 0;
  fClad2Bounce = 0;
  fReflected = 0;
  fEscaped = 0;
  fCoverEnd = 0;
  
  fTotalMudE = 0;
  //fTotalMuStep = 0;
  fTotalMudX = 0;
  fTotalElecdE = 0;
  //fTotalElecStep = 0;
  fTotalElecdX = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
  // Get Hits from the detector if any
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String colName = "MPPCHitCollection";
  fMPPCCollID = SDman->GetCollectionID(colName);

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  MPPCHitsCollection* mppcHC = nullptr;

  // Get the hit collections
  if (HCE) {
    if (fMPPCCollID >= 0) {
      mppcHC = (MPPCHitsCollection*)(HCE->GetHC(fMPPCCollID));
    }
  }

  // Get hit information about photons that reached the detector in this event
  G4int n_hit = 0;
  if (mppcHC) {
    n_hit = mppcHC->entries();
  }

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(2, mppcHC->entries());
  for (size_t i = 0; i < mppcHC->entries(); ++i) {
    auto pdHit = (*mppcHC)[i];
    analysisManager->FillH1(0, pdHit->GetEnergy());
    analysisManager->FillH1(1, pdHit->GetArrivalTime());
  }
  
  // Hit collection at Scintillators
  ScinHitID = SDman->GetCollectionID("ScintHitCollection");
  ScintillatorsHitCollect *scintCollect = nullptr;
  if (HCE) {
  	if(ScinHitID >= 0) {
  		scintCollect = (ScintillatorsHitCollect*)(HCE->GetHC(ScinHitID));
  	}
  }
  G4int nScinHit = 0;
  if(scintCollect) { nScinHit = scintCollect->entries();}
  for (size_t i = 0; i < nScinHit ; ++i) {
    auto HitAccess = (*scintCollect)[i];
    if((HitAccess->GetTrackParticleName() == "mu-")) {
    	AddMudE(HitAccess->GetdE());
    	AddMudX(HitAccess->GetdX());
    }
    if((HitAccess->GetTrackParticleName() == "e-")) {
    	AddElecdE(HitAccess->GetdE());
    	AddElecdX(HitAccess->GetdX());
    }
  }
  if (fTotalMudX) {
  G4double averageMu = (fTotalMudE/fTotalMudX) ;
  analysisManager->FillNtupleDColumn(0,0,averageMu / (CLHEP::MeV/CLHEP::cm) );
  analysisManager->AddNtupleRow(0);
  /*G4cout << " Average muon dEdX " << fTotalMudE/fTotalMudX&&(HitAccess->GetHitTrackParentID() == 0
           << G4endl; */
  }
  if (fTotalElecdX ) {
  G4double averageE = (fTotalElecdE/fTotalElecdX) ;
  analysisManager->FillNtupleDColumn(1,0,averageE / (CLHEP::MeV/CLHEP::cm) );
  analysisManager->AddNtupleRow(1);
  //G4cout << " Average electron dEdX " << fTotalElecdE/fTotalElecdX<< G4endl;* CLHEP::MeV/CLHEP::cm
  }
  
  
  

  if (fVerboseLevel > 1) {
    G4cout << "-------------------------------------" << G4endl
           << " In this event, number of:  " << G4endl << "  TIR:           " << fNTIR << G4endl
           << "  Exiting:       " << fNExiting << G4endl << "  Escaped Mid:   " << fEscapedSide
           << G4endl << "  Escaped End:   " << fEscapedEnd << G4endl
           << "  Bounced:       " << fBounce << G4endl << "  WLS Bounce:    " << fWLSBounce
           << G4endl << "  Clad1 Bounce:  " << fClad1Bounce << G4endl
           << "  Clad2 Bounce:  " << fClad2Bounce << G4endl << "  Reflected:     " << fReflected
           << G4endl << "  Escaped:       " << fEscaped << G4endl << "  Mirror:        " << fCoverEnd
           << G4endl << "  Detector hit:  " << n_hit << G4endl/**/ ;
           
    
  }
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int EventAction::GetEventNo()
{
  return fpEventManager->GetConstCurrentEvent()->GetEventID();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::SetEventVerbose(G4int level)
{
  fVerboseLevel = level;
}
