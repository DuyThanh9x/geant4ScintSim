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

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4Types.hh"
#include "G4UserSteppingAction.hh"
#include "G4ThreeVector.hh"

class DetectorStructure;
class SteppingMess;
class EventAction;
class Run;

class G4OpBoundaryProcess;
class G4Track;
class G4StepPoint;

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction (DetectorStructure*, EventAction*);
    virtual ~SteppingAction ();

    virtual void UserSteppingAction(const G4Step*) override;
    void Initialize();
    
    // Set the bounce limit, 0 for no limit
    void SetBounceLimit(G4int);

    G4int GetNumberOfBounces();
    G4int GetNumberOfClad1Bounces();
    G4int GetNumberOfClad2Bounces();
    G4int GetNumberOfWLSBounces();
    
    // return number of successful events and reset the counter
    G4int ResetSuccessCounter();

  private:
    G4int PrimaryParticleId;
    G4double PrimaryParticleInitialKineticEnergy;
    G4double PrimaryParticleInitialTotalEnergy;
    G4ThreeVector PrimaryParticleInitial3Momentum;
    G4ThreeVector PrimaryParticleInitialPosition;
    
    // Artificially kill the photon after it has bounced more than this number
    G4int fBounceLimit = 100000;
    // number of photons that reach the end
    G4int fCounterEnd = 0;
    // number of photons that didn't make it to the end
    G4int fCounterSide = 0;
    // total number of bounces that a photon been through
    G4int fCounterBounce = 0;
    // number of bounces that a photon been through within the fibre
    G4int fCounterWLSBounce = 0;
    // number of bounces that a photon been through from Cladding 1 to 2
    G4int fCounterClad1Bounce = 0;
    // number of bounces that a photon been through from Cladding 2 to World
    G4int fCounterClad2Bounce = 0;

    // initial gamma of the photon
    G4double fInitGamma = -1.;
    // initial theta of the photon
    G4double fInitTheta = -1.;
    
    //G4double fMudedx = 0;
    //G4double felectrondedx = 0;

    G4OpBoundaryProcess* fOpProcess = nullptr;

    DetectorStructure* fDetector = nullptr;
    SteppingMess* fSteppingMessenger = nullptr;
    EventAction* fEventAction = nullptr;

    inline void ResetCounters()
    {
      fCounterBounce = 0;
      fCounterWLSBounce = 0;
      fCounterClad1Bounce = 0;
      fCounterClad2Bounce = 0;
      fInitGamma = -1;
      fInitTheta = -1;
    }
};

#endif
