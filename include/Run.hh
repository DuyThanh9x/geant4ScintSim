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

#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "G4ThreeVector.hh"

class Run : public G4Run
{
  public:
    Run();
    ~Run();
    virtual void RecordEvent(const G4Event* anEvent) override;
    virtual void Merge(const G4Run* aRun) override;
    void EndOfRun() const;
    
    void SetPrimaryParticleId (const G4int id) { PrimaryParticleIdRun = id;};
    void SetPrimaryParticleInitialKineticEnergy (const G4double E) { PrimaryParticleInitialKineticEnergyRun = E;};
    void SetPrimaryParticleInitialTotalEnergy (const G4double E) { PrimaryParticleInitialTotalEnergyRun = E;};
    void SetPrimaryParticleInitial3Momentum (const G4ThreeVector& valu) { PrimaryParticleInitial3MomentumRun = valu;};
    void SetPrimaryParticleInitialPosition (const G4ThreeVector& valu) { PrimaryParticleInitialPositionRun = valu;};
    void AddNumberDecays() { ++NumDecaysRun;}
    
    G4int GetPrimaryParticleId() const { return PrimaryParticleIdRun;}
    G4double GetPrimaryParticleInitialKineticEnergy() const { return PrimaryParticleInitialKineticEnergyRun;}
    G4double GetPrimaryParticleInitialTotalEnergy() const { return PrimaryParticleInitialTotalEnergyRun;}
    G4ThreeVector GetPrimaryParticleInitial3Momentum() const { return PrimaryParticleInitial3MomentumRun;}
    G4ThreeVector GetPrimaryParticleInitialPosition() const { return PrimaryParticleInitialPositionRun;}
    G4int GetNumberDecays() const { return NumDecaysRun;}
  private:
  G4int PrimaryParticleIdRun;
  G4double PrimaryParticleInitialKineticEnergyRun;
  G4double PrimaryParticleInitialTotalEnergyRun;
  G4ThreeVector PrimaryParticleInitial3MomentumRun;
  G4ThreeVector PrimaryParticleInitialPositionRun;
  G4int NumDecaysRun;
};

#endif
