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
////Addapted from example/extended/optical/wls/

#ifndef MPPCHit_h
#define MPPCHit_h 1

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"
#include "G4VPhysicalVolume.hh"

class MPPCHit : public G4VHit
{
  public:
    MPPCHit();
    MPPCHit(G4ThreeVector pExit, G4ThreeVector pArrive, G4double pTime, G4double pEnergy);
    ~MPPCHit() override = default;

    MPPCHit(const MPPCHit& right);
    const MPPCHit& operator=(const MPPCHit& right);

    G4bool operator==(const MPPCHit& right) const;

    inline void* operator new(size_t);
    inline void operator delete(void* aHit);

    inline void SetArrivalPos(G4ThreeVector xyz) { fPosArrive = xyz; }
    inline G4ThreeVector GetArrivalPos() { return fPosArrive; }

    inline void SetExitPos(G4ThreeVector xyz) { fPosExit = xyz; }
    inline G4ThreeVector GetExitPos() { return fPosExit; }

    inline void SetArrivalTime(G4double t) { fArrivalTime = t; }
    inline G4double GetArrivalTime() { return fArrivalTime; }

    inline void SetEnergy(G4double en) { fEnergy = en; }
    inline G4double GetEnergy() { return fEnergy; }

    void Print() override;

  private:
    // the arrival time of the photon
    G4double fArrivalTime = 0.;
    // where the photon hit the detector (detector's coordinate)
    G4ThreeVector fPosArrive;
    // where the photon exited the fiber (world's coordinate)
    G4ThreeVector fPosExit;
    // energy of photon
    G4double fEnergy = 0.;
};

//--------------------------------------------------
// Type Definitions
//--------------------------------------------------

typedef G4THitsCollection<MPPCHit> MPPCHitsCollection;

extern G4ThreadLocal G4Allocator<MPPCHit>* MPPCHitAllocator;

//--------------------------------------------------
// Operator Overloads
//--------------------------------------------------

inline void* MPPCHit::operator new(size_t)
{
  if (!MPPCHitAllocator) MPPCHitAllocator = new G4Allocator<MPPCHit>;
  return (void*)MPPCHitAllocator->MallocSingle();
}

inline void MPPCHit::operator delete(void* aHit)
{
  MPPCHitAllocator->FreeSingle((MPPCHit*)aHit);
}

#endif
