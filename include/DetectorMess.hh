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

//Addapted from example/extended/optical/wls/

#ifndef DetectorMess_h
#define DetectorMess_h 1

#include "DetectorStructure.hh"

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;

class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithoutParameter;

class DetectorMess : public G4UImessenger
{
  public:
    DetectorMess (DetectorStructure*);
    ~DetectorMess () override;

    void SetNewValue(G4UIcommand*, G4String) override;

  private:
    DetectorStructure* fDetector = nullptr;

    G4UIdirectory* fDetDir = nullptr;

    G4UIcmdWithADoubleAndUnit* fSetWLSLengthCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetWLSRadiusCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetClad1RadiusCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetClad2RadiusCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetMPPCHalfLengthCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetGapCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetMPPCAlignmentCmd = nullptr;
    G4UIcmdWithADouble* fSetSurfaceRoughnessCmd = nullptr;
    G4UIcmdWithADouble* fSetCoverEndPolishCmd = nullptr;
    G4UIcmdWithADouble* fSetCoverEndReflectivityCmd = nullptr;
    G4UIcmdWithADouble* fSetMPPCPolishCmd = nullptr;
    G4UIcmdWithADouble* fSetMPPCReflectivityCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetBarLengthCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetBarWidthCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetBarThickCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetHoleRadiusCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetCoatingThicknessCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fSetCoatingRadiusCmd = nullptr;
};

#endif
