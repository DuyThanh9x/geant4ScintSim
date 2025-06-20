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

#ifndef TrajectoryPoint_h
#define TrajectoryPoint_h 1

#include "G4Allocator.hh"
#include "G4StepStatus.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryPoint.hh"
#include "globals.hh"

class G4Track;
class G4Step;
class G4VProcess;

class TrajectoryPoint : public G4TrajectoryPoint
{
  public:
    TrajectoryPoint() = default;
    TrajectoryPoint(const G4Track*);
    TrajectoryPoint(const G4Step*);
    TrajectoryPoint(const TrajectoryPoint& right);
    ~TrajectoryPoint() override = default;

    inline void* operator new(size_t);
    inline void operator delete(void* aTrajectoryPoint);
    inline G4bool operator==(const TrajectoryPoint& right) const { return (this == &right); };

    inline G4double GetTime() const { return fTime; };
    inline const G4ThreeVector GetMomentum() const { return fMomentum; };
    inline G4StepStatus GetStepStatus() const { return fStepStatus; };
    inline G4String GetVolumeName() const { return fVolumeName; };

    const std::map<G4String, G4AttDef>* GetAttDefs() const override;
    std::vector<G4AttValue>* CreateAttValues() const override;

  private:
    G4double fTime = 0.;
    G4ThreeVector fMomentum;
    G4StepStatus fStepStatus = fUndefined;
    G4String fVolumeName;
};

extern G4ThreadLocal G4Allocator<TrajectoryPoint>* TrajPointAllocator;

inline void* TrajectoryPoint::operator new(size_t)
{
  if (!TrajPointAllocator) TrajPointAllocator = new G4Allocator<TrajectoryPoint>;
  return (void*)TrajPointAllocator->MallocSingle();
}

inline void TrajectoryPoint::operator delete(void* aTrajectoryPoint)
{
  TrajPointAllocator->FreeSingle((TrajectoryPoint*)aTrajectoryPoint);
}

#endif
