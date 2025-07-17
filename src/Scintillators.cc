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
  //if (dE == 0) return false;
  
  G4double dx = aStep->GetStepLength();
  //if (dx <= 0) return false;
  
  G4Track* theTrack = aStep->GetTrack();
  G4int partID = theTrack->GetParentID();
  G4int StrackID = theTrack->GetTrackID();
  G4String part = theTrack->GetDefinition()->GetParticleName();
  G4String process = "";
  
  auto SaveHit = new ScintillatorsHit();
  
  	if((part == "mu+") || (part == "e+") ) {
		SaveHit->SetHitTrackID(StrackID);
		SaveHit->SetHitParentID(partID);
		SaveHit->SetTrackParticleName(part);
	  	//if (partID == 0) {
	  	//	SaveHit->SetTrackProcess("primary");
	  	//} else
	  		//{
	  	process = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
	  			//theTrack->GetCreatorProcess()->GetProcessName();
	  	SaveHit->SetStepProcess(process);
	  		//}
	  	//}
	  	//
	  	//
		//SaveHit->SetNameVolume(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName());
		  	//SaveHit->SetVolumeNumber(aStep->GetPostStepPoint()->GetTouchableHandle()->GetCopyNumber(1));
		SaveHit->SetEndPointKineticE(aStep->GetPostStepPoint()->GetKineticEnergy());
		SaveHit->SetKineticE(aStep->GetPreStepPoint()->GetKineticEnergy());
		SaveHit->SetMomentumDirection(aStep->GetPreStepPoint()->GetMomentumDirection());
		SaveHit->SetdE(dE);
		SaveHit->SetdX(dx);
    		SaveHit->SetStepNumber(theTrack->GetCurrentStepNumber());
      }  	 
  
  ScintHitCollect->insert(SaveHit);
  
  return true;
}
	
void Scintillators :: EndOfEvent(G4HCofThisEvent*) {
}
