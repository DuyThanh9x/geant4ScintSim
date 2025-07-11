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
#include "PrimaryGeneratorMess.hh"

#include "PrimaryGeneratorAction.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"

//

PrimaryGeneratorMess :: PrimaryGeneratorMess (PrimaryGeneratorAction* gun)
  : fAction(gun)
{
  fGunDir = new G4UIdirectory("/scinsm/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fSetPolarizationCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/gun/optPhotonPolar", this);
  fSetPolarizationCmd->SetGuidance("Set linear polarization");
  fSetPolarizationCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  fSetPolarizationCmd->SetParameterName("angle", true);
  fSetPolarizationCmd->SetUnitCategory("Angle");
  fSetPolarizationCmd->SetDefaultValue(0.);
  fSetPolarizationCmd->AvailableForStates(G4State_Idle);

  fSetDecayTimeConstantCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/gun/setDecayTimeConstant", this);
  fSetDecayTimeConstantCmd->SetGuidance("Set the decay time constant");
  fSetDecayTimeConstantCmd->SetGuidance("for the starting time of each photon");
  fSetDecayTimeConstantCmd->SetParameterName("time_const", false);
  fSetDecayTimeConstantCmd->SetUnitCategory("Time");
  fSetDecayTimeConstantCmd->SetRange("time_const>=0");
  fSetDecayTimeConstantCmd->AvailableForStates(G4State_Idle);
}

//

PrimaryGeneratorMess :: ~PrimaryGeneratorMess()
{
  delete fGunDir;
  delete fSetPolarizationCmd;
  delete fSetDecayTimeConstantCmd;
}

//

void PrimaryGeneratorMess :: SetNewValue(G4UIcommand* command, G4String val)
{
  if (command == fSetPolarizationCmd)
    fAction->SetOptPhotonPolar(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  else if (command == fSetDecayTimeConstantCmd)
    fAction->SetDecayTimeConstant(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
}
