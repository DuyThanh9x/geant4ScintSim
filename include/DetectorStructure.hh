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
//

#ifndef DetectorStructure_h
#define DetectorStructure_h 1

#include "G4Cache.hh"
#include "G4RotationMatrix.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include <CLHEP/Units/SystemOfUnits.h>

class Materials;
class DetectorMess;
class MPPC;
class Scintillators;

class G4Box;
class G4EllipticalTube;
class G4LogicalVolume;
class G4Material;
class G4Tubs;
class G4VisAttributes;
class G4VPhysicalVolume;

class DetectorStructure : public G4VUserDetectorConstruction
{
  public:
    DetectorStructure();
    ~DetectorStructure() override;

    G4VPhysicalVolume* Construct() override;
    G4VPhysicalVolume* ConstructDetector();

    void ConstructSDandField() override;

    // Set Material Commands for World and WLSfiber
    void SetWorldMaterial(G4String);
    void SetMaterial(G4String);
    void SetCoupleMaterial(G4String);

    //void SetPhotonDetGeometry(G4String);
    //void SetNumberOfCladding(G4int);  // Maximum 2 claddings

    void SetWLSLength(G4double);  // Total length of WLS fiber
    void SetWLSRadius(G4double);
    void SetClad1Radius(G4double);
    void SetClad2Radius(G4double);
    void SetMPPCHalfLength(G4double);
    void SetGap(G4double);
    void SetMPPCAlignment(G4double);
    // Set the ratio of x and y (x/y) radius of the ellipse
    //void SetXYRatio(G4double);
    // Set the Roughness in between each layer
    void SetSurfaceRoughness(G4double);
    // Set the reflectivity of the mirror
    void SetCoverEndReflectivity(G4double);
    // Set the polish of the mirror
    void SetCoverEndPolish(G4double);
    // Set the reflectivity of the mirror
    void SetMPPCReflectivity(G4double);
    // Set the polish of the mirror
    void SetMPPCPolish(G4double);

    //void SetMirror(G4bool);

    void SetBarLength(G4double);
    void SetBarWidth(G4double);
    void SetBarThick(G4double);
    void SetHoleRadius(G4double);
    void SetCoatingThickness(G4double);
    void SetCoatingRadius(G4double);

    G4double GetWLSLength();
    G4double GetWLSRadius();
    G4double GetWLSEnd();
    G4double GetWLSRMax();
    G4double GetSurfaceRoughness();
    G4bool IsPerfectFiber();

    G4double GetBarLength();
    G4double GetBarWidth();
    G4double GetBarThick();
    G4double GetHoleRadius();
    G4double GetHoleLength();
    

    G4double GetCoatingThickness();
    G4double GetCoatingRadius();

    G4Material* FindMaterial(G4String);

  private:
    std::vector<G4VisAttributes*> fVisAttributes;

    Materials* fMaterials = nullptr;

    G4LogicalVolume* fLogicHole = nullptr;
    G4LogicalVolume* logicScintillator = nullptr;
    G4LogicalVolume* logicScintSideXZ = nullptr;
    G4LogicalVolume* logicScintSideYZ = nullptr;
    G4LogicalVolume* logicScintCrnr = nullptr;
    G4LogicalVolume* fLogicWorld = nullptr;

    G4VPhysicalVolume* fPhysicWorld = nullptr;
    G4VPhysicalVolume* fPhysicHole = nullptr;

    G4double fWorldX = 10. * CLHEP::cm;
    G4double fWorldY = 10. * CLHEP::cm;
    G4double fWorldZ = 40. * CLHEP::cm;

    G4double fWLSR = 0.5 * CLHEP::mm;
    //G4double fWLSRY = ;
    G4double fWLSZ = 26 * CLHEP::cm;

    G4double fClad1R = -1;
    //G4double fClad1RY = -1;
    G4double fClad1Z = -1;

    G4double fClad2R = -1.;
    //G4double fClad2RY = -1.;
    G4double fClad2Z = -1.;

    G4double fClrfiberHalfL = -1;
    G4double fClrfiberHalfZ = -1;

    G4double fCoupleR = 1 * CLHEP::cm;
    //G4double fCoupleRY = 0.5 * CLHEP::cm;
    G4double fCoupleZ = 0.4 * CLHEP::cm;

    G4double fCoverEndR = -1.;
    G4double fCoverEndZ = 0.2 * CLHEP::mm;
    //G4bool fMirrorToggle = true;

    //G4String fMPPCShape = "Square";
    G4double fMPPCHalfL = -1;
    G4double fMPPCHalfZ = 0.05 * CLHEP::mm;
    G4double fMPPCDist = 0.;
    G4double fMPPCTheta = 0;

    G4double fWLSOrigin = 0.;
    G4double fCoupleOrigin = 0.;
    G4double fCoverEndOrigin = 0.;
    G4double fMPPCOriginX = 0.;
    G4double fMPPCOriginZ = 0.;

    //G4int fNumOfCladLayers = 02;

    G4double fCoverEndPolish = 1.;
    G4double fCoverEndReflectivity = 1.;
    G4double fMPPCPolish = 1.;
    G4double fMPPCReflectivity = 0.;
    G4double fExtrusionPolish = 1.;
    G4double fExtrusionReflectivity = 1.;
    G4double fSurfaceRoughness = 1.;
    //G4double fXYRatio = 1.;

    G4double fBarLength = 25. * CLHEP::cm;
    G4double fBarWidth = 2.5 * CLHEP::cm;
    G4double fBarThick = 1 * CLHEP::cm;
    G4double fHoleRadius = 0.6 * CLHEP::mm;
    G4double fHoleLength = -1.;
    G4double fCoatingThickness = 0.25 * CLHEP::mm;
    G4double fCoatingRadius = 1.875 * CLHEP::mm;

    void UpdateGeometryParameters();

    DetectorMess* fDetectorMessenger = nullptr;
    G4Cache<MPPC*> fmppcSD;
    //Scintillators* scinSD = nullptr;
};

#endif
