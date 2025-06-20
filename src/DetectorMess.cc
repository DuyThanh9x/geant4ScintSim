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
#include "DetectorMess.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIdirectory.hh"

//

DetectorMess :: DetectorMess(DetectorStructure* det) : fDetector(det)
{
  fDetDir = new G4UIdirectory("/scinsm/");
  fDetDir->SetGuidance(" Geometry Setup ");

  fSetSurfaceRoughnessCmd = new G4UIcmdWithADouble("/scinsm/setSurfaceRoughness", this);
  fSetSurfaceRoughnessCmd->SetGuidance("Set the roughness between Clad1 and WLS");
  fSetSurfaceRoughnessCmd->SetParameterName("roughness", false);
  fSetSurfaceRoughnessCmd->SetRange("roughness>0 && roughness<=1");
  fSetSurfaceRoughnessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetSurfaceRoughnessCmd->SetToBeBroadcasted(false);

  fSetCoverEndPolishCmd = new G4UIcmdWithADouble("/scinsm/setCoverEndPolish", this);
  fSetCoverEndPolishCmd->SetGuidance("Set the polish of the mirror");
  fSetCoverEndPolishCmd->SetParameterName("polish", false);
  fSetCoverEndPolishCmd->SetRange("polish>0 && polish<=1");
  fSetCoverEndPolishCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetCoverEndPolishCmd->SetToBeBroadcasted(false);

  fSetCoverEndReflectivityCmd = new G4UIcmdWithADouble("/scinsm/setCoverEndReflectivity", this);
  fSetCoverEndReflectivityCmd->SetGuidance("Set the reflectivity of the mirror");
  fSetCoverEndReflectivityCmd->SetParameterName("reflectivity", false);
  fSetCoverEndReflectivityCmd->SetRange("reflectivity>=0 && reflectivity<=1");
  fSetCoverEndReflectivityCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetCoverEndReflectivityCmd->SetToBeBroadcasted(false);

  fSetMPPCPolishCmd = new G4UIcmdWithADouble("/scinsm/setMPPCPolish", this);
  fSetMPPCPolishCmd->SetGuidance("Set the polish of the mirror");
  fSetMPPCPolishCmd->SetParameterName("polish", false);
  fSetMPPCPolishCmd->SetRange("polish>0 && polish<=1");
  fSetMPPCPolishCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetMPPCPolishCmd->SetToBeBroadcasted(false);

  fSetMPPCReflectivityCmd = new G4UIcmdWithADouble("/scinsm/setMPPCReflectivity", this);
  fSetMPPCReflectivityCmd->SetGuidance("Set the reflectivity of the mirror");
  fSetMPPCReflectivityCmd->SetParameterName("reflectivity", false);
  fSetMPPCReflectivityCmd->SetRange("reflectivity>=0 && reflectivity<=1");
  fSetMPPCReflectivityCmd->AvailableForStates(G4State_PreInit);
  fSetMPPCReflectivityCmd->SetToBeBroadcasted(false);

  fSetWLSLengthCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setWLSLength", this);
  fSetWLSLengthCmd->SetGuidance("Set the half length of the WLS fiber");
  fSetWLSLengthCmd->SetParameterName("length", false);
  fSetWLSLengthCmd->SetRange("length>0.");
  fSetWLSLengthCmd->SetUnitCategory("Length");
  fSetWLSLengthCmd->SetDefaultUnit("mm");
  fSetWLSLengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetWLSLengthCmd->SetToBeBroadcasted(false);

  fSetWLSRadiusCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setWLSRadius", this);
  fSetWLSRadiusCmd->SetGuidance("Set the radius of the WLS fiber");
  fSetWLSRadiusCmd->SetParameterName("radius", false);
  fSetWLSRadiusCmd->SetRange("radius>0.");
  fSetWLSRadiusCmd->SetUnitCategory("Length");
  fSetWLSRadiusCmd->SetDefaultUnit("mm");
  fSetWLSRadiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetWLSRadiusCmd->SetToBeBroadcasted(false);

  fSetClad1RadiusCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setClad1Radius", this);
  fSetClad1RadiusCmd->SetGuidance("Set the radius of Cladding 1");
  fSetClad1RadiusCmd->SetParameterName("radius", false);
  fSetClad1RadiusCmd->SetRange("radius>0.");
  fSetClad1RadiusCmd->SetUnitCategory("Length");
  fSetClad1RadiusCmd->SetDefaultUnit("mm");
  fSetClad1RadiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetClad1RadiusCmd->SetToBeBroadcasted(false);

  fSetClad2RadiusCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setClad2Radius", this);
  fSetClad2RadiusCmd->SetGuidance("Set the radius of Cladding 2");
  fSetClad2RadiusCmd->SetParameterName("radius", false);
  fSetClad2RadiusCmd->SetRange("radius>0.");
  fSetClad2RadiusCmd->SetUnitCategory("Length");
  fSetClad2RadiusCmd->SetDefaultUnit("mm");
  fSetClad2RadiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetClad2RadiusCmd->SetToBeBroadcasted(false);

  fSetMPPCHalfLengthCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setMPPCHalfLength", this);
  fSetMPPCHalfLengthCmd->SetGuidance("Set the half length of MPPC detector");
  fSetMPPCHalfLengthCmd->SetParameterName("halfL", false);
  fSetMPPCHalfLengthCmd->SetRange("halfL>0.");
  fSetMPPCHalfLengthCmd->SetUnitCategory("Length");
  fSetMPPCHalfLengthCmd->SetDefaultUnit("mm");
  fSetMPPCHalfLengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetMPPCHalfLengthCmd->SetToBeBroadcasted(false);

  fSetGapCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setGap", this);
  fSetGapCmd->SetGuidance("Set the distance between MPPC and fiber end");
  fSetGapCmd->SetParameterName("theta", false);
  fSetGapCmd->SetUnitCategory("Length");
  fSetGapCmd->SetDefaultUnit("mm");
  fSetGapCmd->SetRange("theta>=0.");
  fSetGapCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetGapCmd->SetToBeBroadcasted(false);

  fSetMPPCAlignmentCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setAlignment", this);
  fSetMPPCAlignmentCmd->SetGuidance("Set the deviation of MPPC from z axis");
  fSetMPPCAlignmentCmd->SetParameterName("theta", false);
  fSetMPPCAlignmentCmd->SetUnitCategory("Angle");
  fSetMPPCAlignmentCmd->SetDefaultUnit("deg");
  fSetMPPCAlignmentCmd->SetRange("theta>-90. && theta<90.");
  fSetMPPCAlignmentCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetMPPCAlignmentCmd->SetToBeBroadcasted(false);

  fSetBarLengthCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setBarLength", this);
  fSetBarLengthCmd->SetGuidance("Set the length of the scintillator bar");
  fSetBarLengthCmd->SetParameterName("length", false);
  fSetBarLengthCmd->SetRange("length>0.");
  fSetBarLengthCmd->SetUnitCategory("Length");
  fSetBarLengthCmd->SetDefaultUnit("mm");
  fSetBarLengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetBarLengthCmd->SetToBeBroadcasted(false);

  fSetBarWidthCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setBarWidth", this);
  fSetBarWidthCmd->SetGuidance("Set the Width of the scintillator bar");
  fSetBarWidthCmd->SetParameterName("length", false);
  fSetBarWidthCmd->SetRange("length>0.");
  fSetBarWidthCmd->SetUnitCategory("Length");
  fSetBarWidthCmd->SetDefaultUnit("mm");
  fSetBarWidthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetBarWidthCmd->SetToBeBroadcasted(false);
  
  fSetBarThickCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setBarThick", this);
  fSetBarThickCmd->SetGuidance("Set the Thickness of the scintillator bar");
  fSetBarThickCmd->SetParameterName("length", false);
  fSetBarThickCmd->SetRange("length>0.");
  fSetBarThickCmd->SetUnitCategory("Length");
  fSetBarThickCmd->SetDefaultUnit("mm");
  fSetBarThickCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetBarThickCmd->SetToBeBroadcasted(false);

  fSetHoleRadiusCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setHoleRadius", this);
  fSetHoleRadiusCmd->SetGuidance("Set the radius of the fiber hole");
  fSetHoleRadiusCmd->SetParameterName("radius", false);
  fSetHoleRadiusCmd->SetRange("radius>0.");
  fSetHoleRadiusCmd->SetUnitCategory("Length");
  fSetHoleRadiusCmd->SetDefaultUnit("mm");
  fSetHoleRadiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetHoleRadiusCmd->SetToBeBroadcasted(false);

  fSetCoatingThicknessCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setCoatingThickness", this);
  fSetCoatingThicknessCmd->SetGuidance("Set thickness of the coating on the bars");
  fSetCoatingThicknessCmd->SetParameterName("thick", false);
  fSetCoatingThicknessCmd->SetUnitCategory("Length");
  fSetCoatingThicknessCmd->SetDefaultUnit("mm");
  fSetCoatingThicknessCmd->SetRange("thick>=0.");
  fSetCoatingThicknessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetCoatingThicknessCmd->SetToBeBroadcasted(false);

  fSetCoatingRadiusCmd = new G4UIcmdWithADoubleAndUnit("/scinsm/setCoatingRadius", this);
  fSetCoatingRadiusCmd->SetGuidance("Set inner radius of the corner bar coating");
  fSetCoatingRadiusCmd->SetParameterName("cradius", false);
  fSetCoatingRadiusCmd->SetUnitCategory("Length");
  fSetCoatingRadiusCmd->SetDefaultUnit("mm");
  fSetCoatingRadiusCmd->SetRange("cradius>=0.");
  fSetCoatingRadiusCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fSetCoatingRadiusCmd->SetToBeBroadcasted(false);
}

//

DetectorMess::~DetectorMess()
{
  delete fDetDir;

  delete fSetBarLengthCmd;
  delete fSetBarWidthCmd;
  delete fSetBarThickCmd;
  delete fSetClad1RadiusCmd;
  delete fSetClad2RadiusCmd;
  delete fSetCoatingThicknessCmd;
  delete fSetCoatingRadiusCmd;
  delete fSetGapCmd;
  delete fSetHoleRadiusCmd;
  delete fSetCoverEndPolishCmd;
  delete fSetCoverEndReflectivityCmd;
  delete fSetMPPCAlignmentCmd;
  delete fSetMPPCHalfLengthCmd;
  delete fSetMPPCPolishCmd;
  delete fSetMPPCReflectivityCmd;
  delete fSetSurfaceRoughnessCmd;
  delete fSetWLSLengthCmd;
  delete fSetWLSRadiusCmd;
}

//

void DetectorMess::SetNewValue(G4UIcommand* command, G4String val)
{
  if (command == fSetSurfaceRoughnessCmd) {
    fDetector->SetSurfaceRoughness(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if (command == fSetCoverEndPolishCmd) {
    fDetector->SetCoverEndPolish(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if (command == fSetCoverEndReflectivityCmd) {
    fDetector->SetCoverEndReflectivity(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if (command == fSetMPPCPolishCmd) {
    fDetector->SetMPPCPolish(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if (command == fSetMPPCReflectivityCmd) {
    fDetector->SetMPPCReflectivity(G4UIcmdWithADouble::GetNewDoubleValue(val));
  }
  else if (command == fSetWLSLengthCmd) {
    fDetector->SetWLSLength(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetWLSRadiusCmd) {
    fDetector->SetWLSRadius(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetClad1RadiusCmd) {
    fDetector->SetClad1Radius(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetClad2RadiusCmd) {
    fDetector->SetClad2Radius(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetMPPCHalfLengthCmd) {
    fDetector->SetMPPCHalfLength(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetGapCmd) {
    fDetector->SetGap(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetMPPCAlignmentCmd) {
    fDetector->SetMPPCAlignment(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetBarLengthCmd) {
    fDetector->SetBarLength(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetBarWidthCmd) {
    fDetector->SetBarWidth(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetBarThickCmd) {
    fDetector->SetBarThick(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetHoleRadiusCmd) {
    fDetector->SetHoleRadius(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetCoatingThicknessCmd) {
    fDetector->SetCoatingThickness(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
  else if (command == fSetCoatingRadiusCmd) {
    fDetector->SetCoatingRadius(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(val));
  }
}
