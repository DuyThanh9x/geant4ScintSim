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
#include "SteppingAction.hh"
#include "Run.hh"

#include "DetectorStructure.hh"
#include "EventAction.hh"
#include "MPPC.hh"
#include "SteppingMess.hh"
#include "UserTrackInformation.hh"

#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4DecayTable.hh"
#include "G4DecayProducts.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4OpticalPhoton.hh"
#include "G4ProcessManager.hh"
#include "G4Run.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4UImanager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ios.hh"

//

SteppingAction :: SteppingAction (DetectorStructure* detector, EventAction* event)
  : G4UserSteppingAction(), fDetector(detector), fEventAction(event)
{
  Initialize();
  fSteppingMessenger = new SteppingMess(this);
  ResetCounters();
  
}

//

SteppingAction::~SteppingAction()
{
  delete fSteppingMessenger;
}

//

void SteppingAction :: Initialize()
{
 PrimaryParticleId = 0;
 PrimaryParticleInitialKineticEnergy = 0.0;
 PrimaryParticleInitialTotalEnergy = 0.0;
 PrimaryParticleInitial3Momentum = G4ThreeVector(0.0, 0.0, 0.0);
 PrimaryParticleInitialPosition = G4ThreeVector(0.0, 0.0, 0.0);
}
void SteppingAction :: SetBounceLimit(G4int i)
{
  fBounceLimit = i;
}

//

G4int SteppingAction :: GetNumberOfBounces()
{
  return fCounterBounce;
}

//

G4int SteppingAction :: GetNumberOfClad1Bounces()
{
  return fCounterClad1Bounce;
}

//

G4int SteppingAction :: GetNumberOfClad2Bounces()
{
  return fCounterClad2Bounce;
}



G4int SteppingAction :: GetNumberOfWLSBounces()
{
  return fCounterWLSBounce;
}



G4int SteppingAction :: ResetSuccessCounter()
{
  G4int temp = fCounterEnd;
  fCounterEnd = 0;
  return temp;
}

void SteppingAction :: UserSteppingAction (const G4Step* theStep)
{
  G4Track* theTrack = theStep->GetTrack();
  auto trackInformation = (UserTrackInformation*)theTrack->GetUserInformation();

  G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();

  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  G4String thePrePVname = " ";
  G4String thePostPVname = " ";

  if (thePostPV) {
    thePrePVname = thePrePV->GetName();
    thePostPVname = thePostPV->GetName();
  }
  
  auto RunPtr = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  if (theTrack->GetParentID() == 0 && theTrack->GetCurrentStepNumber() == 1 ) {
  	PrimaryParticleId = theTrack->GetDefinition()->GetPDGEncoding();
  	PrimaryParticleInitialKineticEnergy = theStep->GetPreStepPoint()->GetKineticEnergy();
  	PrimaryParticleInitialTotalEnergy = theStep->GetPreStepPoint()->GetTotalEnergy();
  	PrimaryParticleInitial3Momentum = theStep->GetPreStepPoint()->GetMomentum();
  	PrimaryParticleInitialPosition = theStep->GetPreStepPoint()->GetPosition();
  	
	if (RunPtr) {

	  	RunPtr->SetPrimaryParticleId (PrimaryParticleId);
	  	RunPtr->SetPrimaryParticleInitialKineticEnergy (PrimaryParticleInitialKineticEnergy);
	  	RunPtr->SetPrimaryParticleInitialTotalEnergy (PrimaryParticleInitialTotalEnergy);
	  	RunPtr->SetPrimaryParticleInitial3Momentum (PrimaryParticleInitial3Momentum);
	  	RunPtr->SetPrimaryParticleInitialPosition (PrimaryParticleInitialPosition);
	  
	  	
	}
  }
   
  if ( theTrack->GetParentID() == 0 && theStep->GetPostStepPoint()->GetProcessDefinedStep() != nullptr
      && theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName().find("DecayWithSpin") != std::string::npos)
  {
  	const G4double ekin_dynamicParticle = theStep->GetPostStepPoint()->GetKineticEnergy();
  	const G4ThreeVector decayPos = theStep->GetPostStepPoint()->GetPosition();
  	const G4String decayVolume = theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
  	std::size_t nSecond = theStep->GetNumberOfSecondariesInCurrentStep();
  	
  	if (nSecond == 0) G4cout<<">>>> Bad decay"<<G4endl;
    	if (ekin_dynamicParticle == 0 ) {
    		const std::vector<const G4Track*>* ptrVecSecondaries = theStep->GetSecondaryInCurrentStep();
    		for (auto secondaries : *ptrVecSecondaries) {
    			if ((secondaries) && (secondaries->GetDefinition()->GetParticleName() == "e+")) {
    					double cos = secondaries->GetMomentumDirection()*(theTrack->GetPolarization());
    					fEventAction->SetCosinePolarisedPositron(cos);
    					//if (secondaries->GetPolarization() != G4ThreeVector(0.0, 0.0, 0.0))
    					//G4cout <<"=====>> Polarized positron : "<<secondaries->GetPolarization()<<G4endl;
    					
    			}
    		}
    		if (RunPtr && (nSecond > 0)) { RunPtr->AddNumberDecaysAtRest();}
    	}
    
    	if (ekin_dynamicParticle > 0 ) {
    		
    		G4cout<< G4endl<< G4endl<<"=====>> "<<theTrack->GetDefinition()->GetParticleName()<<" decay In Flight"<<G4endl
    					<<"=====>> Position "<<decayPos/cm<<" [cm]"<<" in Volume " <<decayVolume
    					<< G4endl;
    		G4cout<<G4endl;
    		if (RunPtr && (nSecond > 0)) { RunPtr->AddNumberDecaysInFlight();}
    		
    	}
    	
    	
  }
  
  // Retrieve the status of the photon
  G4OpBoundaryProcessStatus theStatus = Undefined;

  static G4ThreadLocal G4ProcessManager* OpManager =
    G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

  if (OpManager) {
    G4int nproc = OpManager->GetPostStepProcessVector()->entries();
    G4ProcessVector* fPostStepDoItVector = OpManager->GetPostStepProcessVector(typeDoIt);

    for (G4int i = 0; i < nproc; ++i) {
      G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
      fOpProcess = dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
      if (fOpProcess) {
        theStatus = fOpProcess->GetStatus();
        break;
      }
    }
  }

  // Find the skewness of the ray at first change of boundary
  if (fInitGamma == -1
      && (theStatus == TotalInternalReflection || theStatus == FresnelReflection
          || theStatus == FresnelRefraction)
      && trackInformation->IsStatus(InsideOfFiber))
  {
    G4double px = theTrack->GetVertexMomentumDirection().x();
    G4double py = theTrack->GetVertexMomentumDirection().y();
    G4double x = theTrack->GetPosition().x();
    G4double y = theTrack->GetPosition().y();

    fInitGamma = x * px + y * py;

    fInitGamma = fInitGamma / std::sqrt(px * px + py * py) / std::sqrt(x * x + y * y);

    fInitGamma = std::acos(fInitGamma * rad);

    if (fInitGamma / deg > 90.0) {
      fInitGamma = 180 * deg - fInitGamma;
    }
  }
  // Record Photons that missed the photon detector but escaped from readout
  if (!thePostPV && trackInformation->IsStatus(EscapedFromReadOut)) {
    // G4cout << "SteppingAction: status = EscapedFromReadOut" << G4endl;
    fEventAction->AddEscaped();
    // UpdateHistogramSuccess(thePostPoint,theTrack);
    ResetCounters();

    return;
  }

  // Assumed photons are originated at the fiber OR
  // the fiber is the first material the photon hits
  switch (theStatus) {
    // Exiting the fiber
    case FresnelRefraction:
    case SameMaterial:
      fEventAction->AddExiting();

      if (thePostPVname == "WLS" || thePostPVname == "Clad1" || thePostPVname == "Clad2") {
        if (trackInformation->IsStatus(OutsideOfFiber))
          trackInformation->AddStatusFlag(InsideOfFiber);

        // Set the Exit flag when the photon refracted out of the fiber
      }
      else if (trackInformation->IsStatus(InsideOfFiber)) {
        // EscapedFromReadOut if the z position is the same as fiber's end
        if (theTrack->GetPosition().z() == fDetector->GetWLSEnd()) {
          trackInformation->AddStatusFlag(EscapedFromReadOut);
          fCounterEnd++;
          fEventAction->AddEscapedEnd();
        }
        else  // Escaped from side
        {
          trackInformation->AddStatusFlag(EscapedFromSide);
          trackInformation->SetExitPosition(theTrack->GetPosition());
          //  UpdateHistogramEscape(thePostPoint,theTrack);

          fCounterSide++;
          fEventAction->AddEscapedSide();
          ResetCounters();
        }

        trackInformation->AddStatusFlag(OutsideOfFiber);
        trackInformation->SetExitPosition(theTrack->GetPosition());
      }

      return;

    // Internal Reflections
    case TotalInternalReflection:

      fEventAction->AddTIR();

      // Kill the track if it's number of bounces exceeded the limit
      if (fBounceLimit > 0 && fCounterBounce >= fBounceLimit) {
        theTrack->SetTrackStatus(fStopAndKill);
        trackInformation->AddStatusFlag(murderee);
        ResetCounters();
        G4cout << "\n Bounce Limit Exceeded" << G4endl;
        return;
      }
      break;

    case FresnelReflection:

      fCounterBounce++;
      fEventAction->AddBounce();

      if (thePrePVname == "WLS") {
        fCounterWLSBounce++;
        fEventAction->AddWLSBounce();
      }
      else if (thePrePVname == "Clad1") {
        fCounterClad1Bounce++;
        fEventAction->AddClad1Bounce();
      }
      else if (thePrePVname == "Clad2") {
        fCounterClad2Bounce++;
        fEventAction->AddClad1Bounce();
      }

      // Determine if the photon has reflected off the read-out end
      if (theTrack->GetPosition().z() == fDetector->GetWLSEnd()) {
        if (!trackInformation->IsStatus(ReflectedAtReadOut)
            && trackInformation->IsStatus(InsideOfFiber))
        {
          trackInformation->AddStatusFlag(ReflectedAtReadOut);

          if (fDetector->IsPerfectFiber() && theStatus == TotalInternalReflection) {
            theTrack->SetTrackStatus(fStopAndKill);
            trackInformation->AddStatusFlag(murderee);
            // UpdateHistogramReflect(thePostPoint,theTrack);
            ResetCounters();
            return;
          }
        }
      }
      return;

    // Reflection off the mirror
    case LambertianReflection:
    case LobeReflection:
    case SpikeReflection:

      fEventAction->AddReflected();
      // Check if it hits the mirror
      if (thePostPVname == "CoverEnd") {
        trackInformation->AddStatusFlag(ReflectedAtCoverEnd);
        fEventAction->AddCoverEnd();
      }
      return;

    // Detected by a detector
    case Detection:
      // Detected automatically with G4OpBoundaryProcess->InvokeSD set true

      // Stop Tracking when it hits the detector's surface
      ResetCounters();
      theTrack->SetTrackStatus(fStopAndKill);
      return;

    default:
      break;
  }

  // Check for absorbed photons
  if (theTrack->GetTrackStatus() != fAlive && trackInformation->IsStatus(InsideOfFiber)) {
    // UpdateHistogramAbsorb(thePostPoint,theTrack);
    ResetCounters();
    return;
  }
}
