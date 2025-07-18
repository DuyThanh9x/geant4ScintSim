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
//
/// \file optical/wls/src/WLSStackingAction.cc
/// \brief Implementation of the WLSStackingAction class
//Addapted from example/extended/optical/wls/
//
#include "StackingAction.hh"

#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"

//

G4ClassificationOfNewTrack StackingAction :: ClassifyNewTrack (const G4Track* aTrack)
{
  G4ParticleDefinition* particleType = aTrack->GetDefinition();

  // keep primary particle
  if (aTrack->GetParentID() == 0) return fUrgent;

  if (particleType == G4OpticalPhoton::OpticalPhotonDefinition()) {
  	//if(aTrack->GetCreatorProcess()->GetProcessName() == "Scintillation") {
    // keep optical photon
    	//++fPhotonCounter;
    	//return fUrgent;
    	return fKill;
    	//}
  }
  else {
    // discard all other secondaries
    // return fKill;
  }
  return fUrgent;
}

//

void StackingAction :: NewStage()
{
   //G4cout << "Number of scintillation optical photons produces in this event : "
          //<< fPhotonCounter << G4endl;
}

//

void StackingAction :: PrepareNewEvent()
{
  fPhotonCounter = 0;
}
