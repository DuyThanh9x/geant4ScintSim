#include "Scintillators.hh"
#include "ScintillatorsHit.hh"
#include "EventAction.hh"

#include "G4OpticalPhoton.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4MuonMinus.hh"
#include "G4Electron.hh"


Scintillators :: Scintillators (G4String name) : G4VSensitiveDetector(name)
{
	collectionName.insert("ScintHitCollection");
}

void Scintillators :: Initialize (G4HCofThisEvent* HCE)
{
	ScintHitCollect = new ScintillatorsHitCollect (SensitiveDetectorName, collectionName[0]);
	if (fHCID < 0) {
    		fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(ScintHitCollect);
  	}
  	HCE->AddHitsCollection(fHCID, ScintHitCollect);
}

G4bool Scintillators :: ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  if (!aStep) return false;
  
  G4double dE = aStep->GetTotalEnergyDeposit();
  if (dE == 0) return false;
  
  G4double dx = aStep->GetStepLength();
  if (dx <= 0) return false;
  
  G4Track* theTrack = aStep->GetTrack();
  G4int partID = theTrack->GetParentID();
  G4String part = theTrack->GetDefinition()->GetParticleName();
  G4String process = "";
  
  auto SaveHit = new ScintillatorsHit();

  if ((partID == 0) || (partID == 1)) {
  	if((part == "mu-") || (part == "e-") ) {
	  	SaveHit->SetHitTrackParentID(partID);
	  	SaveHit->SetTrackParticleName(part);
	  	if (partID == 1) {
	  		process = theTrack->GetCreatorProcess()->GetProcessName();
	  		SaveHit->SetTrackProcess(process);
	  	}
	  	else SaveHit->SetTrackProcess("primary");
	  	SaveHit->SetNameVolume(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName());
	  	SaveHit->SetVolumeNumber(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1));
	  	SaveHit->SetdE(dE);
	  	SaveHit->SetdX(dx);
	  	ScintHitCollect->insert(SaveHit);
	}
  }
  
  
  return true;
}
	
void Scintillators :: EndOfEvent(G4HCofThisEvent*) {
}
