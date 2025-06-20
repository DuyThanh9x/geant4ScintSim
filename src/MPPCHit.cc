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
#include "MPPCHit.hh"

#include "G4UnitsTable.hh"

G4ThreadLocal G4Allocator<MPPCHit>* MPPCHitAllocator = nullptr;

//

MPPCHit :: MPPCHit()
{
  fPosArrive = G4ThreeVector(0., 0., 0.);
  fPosExit = G4ThreeVector(0., 0., 0.);
}

//

MPPCHit :: MPPCHit (G4ThreeVector pExit, G4ThreeVector pArrive, G4double pTime,
                                 G4double pEnergy)
{
  fPosExit = pExit;
  fPosArrive = pArrive;
  fArrivalTime = pTime;
  fEnergy = pEnergy;
}

//

MPPCHit :: MPPCHit(const MPPCHit& right) : G4VHit()
{
  *this = right;
}

//

const MPPCHit& MPPCHit :: operator=(const MPPCHit& right)
{
  fPosExit = right.fPosExit;
  fPosArrive = right.fPosArrive;
  fArrivalTime = right.fArrivalTime;
  fEnergy = right.fEnergy;

  return *this;
}

//

G4bool MPPCHit :: operator==(const MPPCHit& right) const
{
  return fPosExit == right.fPosExit && fPosArrive == right.fPosArrive
         && fArrivalTime == right.fArrivalTime && fEnergy == right.fEnergy;
}

void MPPCHit::Print()
{
  G4cout << "Arrival time: " << std::setw(7) << G4BestUnit(fArrivalTime, "Time")
         << "Arrival position: (" << std::setw(7) << G4BestUnit(fPosArrive.x(), "Length") << ", "
         << std::setw(7) << G4BestUnit(fPosArrive.y(), "Length") << "); "
         << "Exit position: (" << std::setw(7) << G4BestUnit(fPosExit.x(), "Length") << ", "
         << std::setw(7) << G4BestUnit(fPosExit.y(), "Length") << "); "
         << "Energy: " << std::setw(7) << G4BestUnit(fEnergy, "Energy") << G4endl;
}
