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

#include "TrajectoryPoint.hh"

#include "G4AttDef.hh"
#include "G4AttDefStore.hh"
#include "G4AttValue.hh"
#include "G4Step.hh"
#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"

G4ThreadLocal G4Allocator<TrajectoryPoint>* TrajPointAllocator = nullptr;

TrajectoryPoint::TrajectoryPoint(const G4Step* aStep)
  : G4TrajectoryPoint(aStep->GetPostStepPoint()->GetPosition())
{
  auto postStepPoint = aStep->GetPostStepPoint();
  fTime = postStepPoint->GetGlobalTime();
  fMomentum = postStepPoint->GetMomentum();
  fStepStatus = postStepPoint->GetStepStatus();
  if (postStepPoint->GetPhysicalVolume()) {
    fVolumeName = postStepPoint->GetPhysicalVolume()->GetName();
  }
  else {
    fVolumeName = G4String(" ");
  }
}

TrajectoryPoint::TrajectoryPoint(const G4Track* aTrack)
  : G4TrajectoryPoint(aTrack->GetPosition())
{
  fTime = aTrack->GetGlobalTime();
  fMomentum = aTrack->GetMomentum();
  fStepStatus = fUndefined;
  fVolumeName = aTrack->GetVolume()->GetName();
}

TrajectoryPoint::TrajectoryPoint(const TrajectoryPoint& right) : G4TrajectoryPoint(right)
{
  fTime = right.fTime;
  fMomentum = right.fMomentum;
  fStepStatus = right.fStepStatus;
  fVolumeName = right.fVolumeName;
}

const std::map<G4String, G4AttDef>* TrajectoryPoint::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String, G4AttDef>* store = G4AttDefStore::GetInstance("TrajectoryPoint", isNew);
  if (isNew) {
    G4String Pos("Pos");
    (*store)[Pos] = G4AttDef(Pos, "Position", "Physics", "G4BestUnit", "G4ThreeVector");

    G4String Time("Time");
    (*store)[Time] = G4AttDef(Time, "Time", "Physics", "G4BestUnit", "G4double");

    G4String Momentum("Momentum");
    (*store)[Momentum] = G4AttDef(Momentum, "Momentum", "Physics", "G4BestUnit", "G4ThreeVector");

    G4String StepStatus("StepStatus");
    (*store)[StepStatus] = G4AttDef(StepStatus, "StepStatus", "Physics", "", "G4StepStatus");

    G4String VolumeName("VolumeName");
    (*store)[VolumeName] = G4AttDef(VolumeName, "VolumeName", "Physics", "", "G4String");
  }
  return store;
}

std::vector<G4AttValue>* TrajectoryPoint::CreateAttValues() const
{
  auto values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("Time", G4BestUnit(fTime, "Time"), ""));
  values->push_back(G4AttValue("Momentum", G4BestUnit(fMomentum, "Momentum"), ""));
  values->push_back(G4AttValue("StepStatus", G4UIcommand::ConvertToString(fStepStatus), ""));
  values->push_back(G4AttValue("VolumeName", fVolumeName, ""));

  return values;
}
