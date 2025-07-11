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

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class DetectorStructure;
class PrimaryGeneratorMess;

class G4Event;
class G4GeneralParticleSource;
class G4ParticleGun;
class G4PhysicsTable;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction (DetectorStructure*);
    ~PrimaryGeneratorAction ();

   virtual void GeneratePrimaries(G4Event*) override;

    void BuildEmissionSpectrum();
    void SetOptPhotonPolar(G4double);
    void SetDecayTimeConstant(G4double);
    
  protected:
    G4PhysicsTable* fIntegralTable = nullptr;

  private:
    void SetOptPhotonPolar();
    void SetOptPhotonTime();

    DetectorStructure* fDetector = nullptr;
    G4GeneralParticleSource* fParticleGun = nullptr;
    //G4ParticleGun *fParticleGun = nullptr;
    PrimaryGeneratorMess* fGunMessenger = nullptr;

    static G4bool fFirst;
    G4double fTimeConstant = 0.;
};

#endif
