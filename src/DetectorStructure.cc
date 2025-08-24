//
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

#include "DetectorStructure.hh"

#include "DetectorMess.hh"
#include "Materials.hh"
#include "MPPC.hh"
#include "Scintillators.hh"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4EllipticalTube.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4ios.hh"
#include "globals.hh"



DetectorStructure::DetectorStructure() : fVisAttributes()
{
  fDetectorMessenger = new DetectorMess(this);

  fMPPCHalfL = fWLSR;
  fClrfiberHalfZ = fMPPCHalfZ  + 10. / 2 * nm;
  fHoleLength = fBarLength;
}



DetectorStructure::~DetectorStructure()
{
  delete fDetectorMessenger;
  delete fMaterials;
  for (auto visAttributes : fVisAttributes) {
    delete visAttributes;
  }
}

//

G4VPhysicalVolume* DetectorStructure::Construct()
{
  if (fPhysicWorld) {
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    G4LogicalSkinSurface::CleanSurfaceTable();
    G4LogicalBorderSurface::CleanSurfaceTable();
  }

  fMaterials = Materials::GetInstance();
  UpdateGeometryParameters();

  return ConstructDetector();
}

//

G4VPhysicalVolume* DetectorStructure::ConstructDetector()
{
  auto air = FindMaterial("G4_AIR");
    //--------------------------------------------------
  // World
  //--------------------------------------------------

  G4VSolid* World = new G4Box("World", fWorldX / 2, fWorldY / 2, fWorldZ / 2);

  fLogicWorld = new G4LogicalVolume(World, air, "World");

  fPhysicWorld =
    new G4PVPlacement(nullptr, G4ThreeVector(), fLogicWorld, "World", nullptr, false, 0);
    
  //--------------------------------------------------
  // Extrusion
  //--------------------------------------------------

  auto coating = FindMaterial("Coating");

  G4VSolid* Extrusion =
    new G4Box("Extrusion", GetBarWidth() / 2. , GetBarThick() / 2. , GetBarLength() / 2.);

  auto logicExtrusion = new G4LogicalVolume(Extrusion, coating, "Extrusion");

  auto TiO2Surface =
    new G4OpticalSurface("TiO2Surface", glisur, ground, dielectric_metal, fExtrusionPolish);

  auto TiO2SurfaceProperty = new G4MaterialPropertiesTable();

  std::vector<G4double> p_TiO2 = {2.00 * eV, 3.47 * eV};

  std::vector<G4double> refl_TiO2 = {fExtrusionReflectivity, fExtrusionReflectivity};
  std::vector<G4double> effi_TiO2 = {0., 0.};

  TiO2SurfaceProperty->AddProperty("REFLECTIVITY", p_TiO2, refl_TiO2);
  TiO2SurfaceProperty->AddProperty("EFFICIENCY", p_TiO2, effi_TiO2);

  TiO2Surface->SetMaterialPropertiesTable(TiO2SurfaceProperty);
  
  G4double posScin = GetBarThick()/2  + GetCoatingThickness() + 2 * GetCoatingRadius() + 2 * mm;
  G4double posScin1 = GetBarWidth()/2  + GetCoatingThickness() + 2 * GetCoatingRadius() + 1 * mm;
  G4double posScin2 = GetCoatingThickness() + 2 * GetCoatingRadius() ;
  
  auto rotScin = new G4RotationMatrix();
    rotScin->rotateZ(90. * deg);
  new G4PVPlacement(nullptr, G4ThreeVector(0., posScin + 0.5 * posScin,0.), logicExtrusion, "Extrusion", fLogicWorld, false, 0);//The 1st scint
  new G4PVPlacement(nullptr, G4ThreeVector(0.,0.5*posScin,0.), logicExtrusion, "Extrusion", fLogicWorld, false, 1);//The 2nd scint
  new G4PVPlacement(nullptr, G4ThreeVector(0.,- 0.5 * posScin,0.), logicExtrusion, "Extrusion", fLogicWorld, false, 2);//The 3nd scint
  new G4PVPlacement(nullptr, G4ThreeVector(0.,- posScin - 0.5 * posScin,0.), logicExtrusion, "Extrusion", fLogicWorld, false, 3);/**///The 4nd scint
  
  new G4PVPlacement(rotScin, G4ThreeVector(-posScin1 - 1.5 * mm , 0.5 * posScin2 ,0.), logicExtrusion, "Extrusion", fLogicWorld, false, 4);//The 5nd scint
  new G4PVPlacement(nullptr, G4ThreeVector(- 1.5 * posScin1,- posScin - 0.5 * posScin,0.), logicExtrusion, "Extrusion", fLogicWorld, false, 5);/**///The 6nd scint
  new G4PVPlacement(rotScin, G4ThreeVector( posScin1 + 1.5 * mm, 0.5 * posScin2,0.), logicExtrusion, "Extrusion", fLogicWorld, false, 6);//The 7nd scint
  new G4PVPlacement(nullptr, G4ThreeVector( 1.5 * posScin1,- posScin - 0.5 * posScin,0.), logicExtrusion, "Extrusion", fLogicWorld, false, 7);/**///The 8nd scint

  new G4LogicalSkinSurface("TiO2Surface", logicExtrusion, TiO2Surface);

  //--------------------------------------------------
  // Scintillator
  //--------------------------------------------------

  auto ScintMaterial = FindMaterial("scint");
  //auto ScintMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
  
  G4VSolid* Scintillator =
    new G4Box("Scintillator", GetBarWidth() / 2. - GetCoatingThickness() - GetCoatingRadius(),
              		      GetBarThick() / 2. - GetCoatingThickness() - GetCoatingRadius(), GetBarLength() / 2.);

  logicScintillator = new G4LogicalVolume(Scintillator, ScintMaterial, "Scintillator");
  
  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), logicScintillator, "Scintillator", logicExtrusion,
                    false, 0);
  
  auto userLimitScin = new G4UserLimits(DBL_MAX, DBL_MAX, 10*ms);
  logicScintillator->SetUserLimits(userLimitScin); 
            
  if (GetCoatingRadius() > 0.) {
    G4VSolid* ScintsideXZ =
      new G4Box("SideOfBarXZ", GetBarWidth() / 2. - GetCoatingThickness() - GetCoatingRadius(),
                	       GetCoatingRadius()/2, GetBarLength() / 2.);
                
    G4VSolid* ScintsideYZ =
      new G4Box("SideOfBarYZ", GetCoatingRadius()/2,
                	       GetBarThick() / 2. - GetCoatingThickness() - GetCoatingRadius(), GetBarLength() / 2.);

    G4VSolid* Scintcrnr = new G4Tubs("CrnrOfBar", 0.0 * cm, GetCoatingRadius(),
                                          GetBarLength() / 2., 0. * deg, 90. * deg);

    logicScintSideXZ = new G4LogicalVolume(ScintsideXZ, ScintMaterial, "SideOfBarXZ");
    
    logicScintSideYZ = new G4LogicalVolume(ScintsideYZ, ScintMaterial, "SideOfBarYZ");

    logicScintCrnr = new G4LogicalVolume(Scintcrnr, ScintMaterial, "CrnrOfBar");

    G4double pos = GetBarThick() / 2. - GetCoatingThickness() - GetCoatingRadius()/2;

    new G4PVPlacement(nullptr, G4ThreeVector(0., -pos, 0.), logicScintSideXZ, "SideOfBarXZ", logicExtrusion, false, 0);
                      

    new G4PVPlacement(nullptr, G4ThreeVector(0., pos, 0.), logicScintSideXZ, "SideOfBarXZ", logicExtrusion, false, 1);
                      

    pos = GetBarWidth() / 2. - GetCoatingThickness() - GetCoatingRadius()/2 ;
    
    new G4PVPlacement(nullptr, G4ThreeVector(pos, 0., 0.), logicScintSideYZ, "SideOfBarYZ", logicExtrusion, false, 0);
                      

    new G4PVPlacement(nullptr, G4ThreeVector(-pos, 0., 0.), logicScintSideYZ, "SideOfBarYZ", logicExtrusion, false, 1);
                      

    pos = GetBarThick() / 2. - GetCoatingThickness() - GetCoatingRadius();
    G4double posx = GetBarWidth() / 2. - GetCoatingThickness() - GetCoatingRadius();

    new G4PVPlacement(nullptr, G4ThreeVector(posx, pos, 0.), logicScintCrnr, "CrnrOfBar", logicExtrusion, false, 0);
                      
    auto rot1 = new G4RotationMatrix();
    rot1->rotateZ(-90. * deg);
    new G4PVPlacement(rot1, G4ThreeVector(-posx, pos, 0.), logicScintCrnr, "CrnrOfBar", logicExtrusion, false, 1);
                      

    auto rot2 = new G4RotationMatrix();
    rot2->rotateZ(-180. * deg);

    new G4PVPlacement(rot2, G4ThreeVector(-posx, -pos, 0.), logicScintCrnr, "CrnrOfBar", logicExtrusion, false, 2);
                      

    auto rot3 = new G4RotationMatrix();
    rot3->rotateZ(-270. * deg);

    new G4PVPlacement(rot3, G4ThreeVector(posx, -pos, 0.), logicScintCrnr, "CrnrOfBar", logicExtrusion, false, 3);
                      
  }

  if (GetWLSRadius() < GetHoleRadius()) {
    G4VSolid* Hole =
      new G4Tubs("Hole", 0., GetHoleRadius(), GetHoleLength() / 2., 0. * deg, 360. * deg);

    fLogicHole = new G4LogicalVolume(Hole, air, "Hole");

    fPhysicHole = new G4PVPlacement(nullptr, G4ThreeVector(), fLogicHole, "Hole", logicScintillator, false, 0);
  }

  //--------------------------------------------------
  // Fiber
  //--------------------------------------------------

  if (!(fLogicHole) || !(fPhysicHole)) {
    G4ExceptionDescription ed;
    ed << "The Fiber Hole has not been constructed";
    G4Exception("DetectorStructure", "scint001", FatalException, ed);
  }

  // Pointers to the most recently constructed volume
  G4LogicalVolume* logicPlacement = fLogicHole;
  G4VPhysicalVolume* physicPlacement = fPhysicHole;

  //--------------------------------------------------
  // Fiber Construction
  //--------------------------------------------------

  // Boundary Surface Properties
  G4OpticalSurface* opSurface = nullptr;

  if (fSurfaceRoughness < 1.)
    opSurface = new G4OpticalSurface("RoughSurface", glisur, ground, dielectric_dielectric,
                                     fSurfaceRoughness);

  G4LogicalVolume* logicWLS = nullptr;
  G4LogicalVolume* logicClad1 = nullptr;
  G4LogicalVolume* logicClad2 = nullptr;
  G4VPhysicalVolume* physicClad1 = nullptr;
  G4VPhysicalVolume* physicClad2 = nullptr;

  auto fCladding1 = FindMaterial("Cladding1");
  auto fCladding2 = FindMaterial("Cladding2");
  auto fWLSMaterial = FindMaterial("WLSMaterial");

  G4VSolid* Clad2 = new G4Tubs("Clad2", 0., fClad2R, fClad2Z / 2, 0., twopi);
      
  logicClad2 = new G4LogicalVolume(Clad2, fCladding2, "Clad2");

  physicClad2 = new G4PVPlacement(nullptr, G4ThreeVector(0.0, 0.0, fWLSOrigin), logicClad2, "Clad2", logicPlacement, false, 0);
                                    
  if (opSurface) {
        new G4LogicalBorderSurface("surfaceClad2Out", physicClad2, physicPlacement, opSurface);
        new G4LogicalBorderSurface("surfaceClad2In", physicPlacement, physicClad2, opSurface);
  }

  logicPlacement = logicClad2;
  physicPlacement = physicClad2;
      
  G4VSolid*  Clad1 = new G4Tubs("Clad1", 0., fClad1R, fClad1Z / 2, 0., twopi);

  logicClad1 = new G4LogicalVolume(Clad1, fCladding1, "Clad1");

  physicClad1 = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., fWLSOrigin), logicClad1, "Clad1", logicPlacement, false, 0);
                                     
      // Place the rough surface only if needed
  if (opSurface) {
        new G4LogicalBorderSurface("surfaceClad1Out", physicClad1, physicPlacement, opSurface);

        new G4LogicalBorderSurface("surfaceClad1In", physicPlacement, physicClad1, opSurface);
  }

  logicPlacement = logicClad1;
  physicPlacement = physicClad1;
      
  G4VSolid*  WLS = new G4Tubs("WLS", 0., fWLSR, fWLSZ / 2, 0., twopi);
      
  logicWLS = new G4LogicalVolume(WLS, fWLSMaterial, "WLS");

  logicWLS->SetUserLimits(new G4UserLimits(DBL_MAX, DBL_MAX, 10. * ms));

  G4VPhysicalVolume* physicWLS = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., fWLSOrigin), logicWLS, "WLS", logicPlacement, false, 0);
                          
      // Place the rough surface only if needed
  if (opSurface) {
        new G4LogicalBorderSurface("surfaceWLSOut", physicWLS, physicPlacement, opSurface);

        new G4LogicalBorderSurface("surfaceWLSIn", physicPlacement, physicWLS, opSurface);
  }

  //--------------------------------------------------
  // CoverEnd for reflection at one of the end
  //--------------------------------------------------

  // Place the mirror only if the user wants the mirror
  G4LogicalVolume* logicCoverEnd = nullptr;

  auto aluminum = FindMaterial("G4_Al");

  //
  G4VSolid* CoverEnd = new G4Box("CoverEnd", fCoverEndR / 2, fCoverEndR / 2, fCoverEndZ / 2);

  logicCoverEnd = new G4LogicalVolume(CoverEnd, aluminum, "CoverEnd");

  auto CoverEndSurface = new G4OpticalSurface("CoverEndSurface", glisur, ground, dielectric_metal, fCoverEndPolish);
      
  auto CoverEndSurfaceProperty = new G4MaterialPropertiesTable();

  std::vector<G4double> p_CoverEnd = {2.00 * eV, 3.47 * eV};
  std::vector<G4double> refl_CoverEnd = {fCoverEndReflectivity, fCoverEndReflectivity};
  std::vector<G4double> effi_CoverEnd = {0., 0.};

  CoverEndSurfaceProperty->AddProperty("REFLECTIVITY", p_CoverEnd, refl_CoverEnd);
  CoverEndSurfaceProperty->AddProperty("EFFICIENCY", p_CoverEnd, effi_CoverEnd);

  CoverEndSurface->SetMaterialPropertiesTable(CoverEndSurfaceProperty);

  new G4PVPlacement(nullptr, G4ThreeVector(0., posScin + 0.5*posScin, fCoverEndOrigin), logicCoverEnd, "CoverEnd",
                      fLogicWorld, false, 0);
  new G4PVPlacement(nullptr, G4ThreeVector(0., 0.5*posScin, fCoverEndOrigin), logicCoverEnd, "CoverEnd",
                      fLogicWorld, false, 1);
  new G4PVPlacement(nullptr, G4ThreeVector(0., -0.5*posScin, fCoverEndOrigin), logicCoverEnd, "CoverEnd",
                      fLogicWorld, false, 2);
  new G4PVPlacement(nullptr, G4ThreeVector(0., -posScin - 0.5*posScin, fCoverEndOrigin), logicCoverEnd, "CoverEnd",
                      fLogicWorld, false, 3);
  
  new G4PVPlacement(nullptr, G4ThreeVector(-posScin1 - 1.5 * mm, 0.5*posScin2, fCoverEndOrigin), logicCoverEnd, "CoverEnd",
                      fLogicWorld, false, 4);
  new G4PVPlacement(nullptr, G4ThreeVector(-1.5 * posScin1, -posScin - 0.5*posScin, fCoverEndOrigin), logicCoverEnd, "CoverEnd",
                      fLogicWorld, false, 5);
  new G4PVPlacement(nullptr, G4ThreeVector(posScin1 + 1.5 * mm, 0.5*posScin2, fCoverEndOrigin), logicCoverEnd, "CoverEnd",
                      fLogicWorld, false, 6);
  new G4PVPlacement(nullptr, G4ThreeVector(1.5 * posScin1, -posScin - 0.5*posScin, fCoverEndOrigin), logicCoverEnd, "CoverEnd",
                      fLogicWorld, false, 7);
    
  new G4LogicalSkinSurface("CoverEndSurface", logicCoverEnd, CoverEndSurface);
  

  //--------------------------------------------------
  // Coupling at the read-out end
  //--------------------------------------------------

  // Clear Fiber (Coupling Layer)
  G4VSolid* Couple = new G4Box("Couple", fCoupleR / 2, fCoupleR / 2, fCoupleZ / 2);

  //auto plastic = G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYETHYLENE");
  auto logicCouple = new G4LogicalVolume(Couple, air, "Couple");

  new G4PVPlacement(nullptr, G4ThreeVector(0., posScin + 0.5*posScin, fCoupleOrigin), logicCouple, "Couple",
                    fLogicWorld, false, 0);
  new G4PVPlacement(nullptr, G4ThreeVector(0., 0.5*posScin, fCoupleOrigin), logicCouple, "Couple",
                    fLogicWorld, false, 1);
  new G4PVPlacement(nullptr, G4ThreeVector(0., -0.5*posScin, fCoupleOrigin), logicCouple, "Couple",
                    fLogicWorld, false, 2);
  new G4PVPlacement(nullptr, G4ThreeVector(0., -posScin-0.5*posScin, fCoupleOrigin), logicCouple, "Couple",
                    fLogicWorld, false, 3);
 
  new G4PVPlacement(nullptr, G4ThreeVector(-posScin1 - 1.5 * mm, 0.5*posScin2, fCoupleOrigin), logicCouple, "Couple",
                    fLogicWorld, false, 4);
  new G4PVPlacement(nullptr, G4ThreeVector(-1.5 * posScin1, -posScin-0.5*posScin, fCoupleOrigin), logicCouple, "Couple",
                    fLogicWorld, false, 5);
  new G4PVPlacement(nullptr, G4ThreeVector(posScin1 + 1.5 * mm, 0.5*posScin2, fCoupleOrigin), logicCouple, "Couple",
                    fLogicWorld, false, 6);
  new G4PVPlacement(nullptr, G4ThreeVector(1.5 * posScin1, -posScin-0.5*posScin, fCoupleOrigin), logicCouple, "Couple",
                    fLogicWorld, false, 7);

  //--------------------------------------------------
  // A logical layer in front of MPPC
  //--------------------------------------------------

  // Purpose: Preventing direct dielectric to metal contact

  // Check for valid placement of MPPC
  if (fMPPCTheta > std::atan(fMPPCDist / fMPPCHalfL)) {
    fMPPCTheta = 0.;
    fMPPCOriginX = std::sin(fMPPCTheta) * (fMPPCDist + fClrfiberHalfZ);
    fMPPCOriginZ = -fCoupleZ / 2 + std::cos(fMPPCTheta) * (fMPPCDist + fClrfiberHalfZ);
    G4ExceptionDescription ed;
    ed << "Invalid alignment.  Alignment reset to 0.";
    G4Exception("DetectorStructure", "scint002", JustWarning, ed);
  }

  // Clear Fiber (Coupling Layer)
   
  G4VSolid* Clrfiber = new G4Box("ClearFiber", fClrfiberHalfL, fClrfiberHalfL, fClrfiberHalfZ);
  
  auto logicClrfiber = new G4LogicalVolume(Clrfiber, air, "ClearFiber");

  new G4PVPlacement(new G4RotationMatrix(CLHEP::HepRotationY(-fMPPCTheta)),
                    G4ThreeVector(fMPPCOriginX, 0 , fMPPCOriginZ), logicClrfiber, "ClearFiber",
                    logicCouple, false, 0);
              
  //--------------------------------------------------
  // MPPC (Sensitive Detector)
  //--------------------------------------------------

  // Physical Construction
  G4VSolid* MPPC = nullptr;

  MPPC = new G4Box("MPPC", fMPPCHalfL, fMPPCHalfL, fMPPCHalfZ);

  auto logicMPPC = new G4LogicalVolume(MPPC, aluminum, "MPPC");

  new G4PVPlacement(nullptr, G4ThreeVector(0., 0, 0.), logicMPPC, "MPPC", logicClrfiber, false, 0);
                    
  // MPPC Surface Properties
  auto MPPCSurface =
    new G4OpticalSurface("MPPCSurface", glisur, ground, dielectric_metal, fMPPCPolish);

  auto MPPCSurfaceProperty = new G4MaterialPropertiesTable();

  std::vector<G4double> p_mppc = {2.00 * eV, 3.47 * eV};
  std::vector<G4double> refl_mppc = {fMPPCReflectivity, fMPPCReflectivity};
  std::vector<G4double> effi_mppc = {1., 1.};

  MPPCSurfaceProperty->AddProperty("REFLECTIVITY", p_mppc, refl_mppc);
  MPPCSurfaceProperty->AddProperty("EFFICIENCY", p_mppc, effi_mppc);

  MPPCSurface->SetMaterialPropertiesTable(MPPCSurfaceProperty);

  new G4LogicalSkinSurface("MPPCSurface", logicMPPC, MPPCSurface);

  // visualization attributes -------------------------------------------------

  auto visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  visAttributes->SetVisibility(false);
  fLogicWorld->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2, 0.5));
  visAttributes->SetVisibility(true);
  logicExtrusion->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.9));
  visAttributes->SetVisibility(true);
  logicScintillator->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0, 0.8, 0.2, 0.2));
  visAttributes->SetVisibility(true);
  logicScintSideXZ->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  
  visAttributes = new G4VisAttributes(G4Colour(0.0, 0.8, 0.2, 0.2));
  visAttributes->SetVisibility(true);
  logicScintSideYZ->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0, 0.8, 0.2, 0.2));
  visAttributes->SetVisibility(true);
  logicScintCrnr->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.4, 0.0, 0.0, 0.5));
  visAttributes->SetVisibility(true);
  fLogicHole->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  if (logicClad1 != nullptr) {
    visAttributes = new G4VisAttributes(G4Colour(0.0, 0.8, 0.5, 0.5));
    visAttributes->SetVisibility(true);
    logicClad1->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }

  if (logicClad2 != nullptr) {
    visAttributes = new G4VisAttributes(G4Colour(0.0, 0.5, 0.8, 0.5));
    visAttributes->SetVisibility(true);
    logicClad2->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  }

  visAttributes = new G4VisAttributes(G4Colour(0.8, 0.8, 1.0));
  visAttributes->SetVisibility(true);
  logicWLS->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 0.5, 0.5));
  visAttributes->SetVisibility(true);
  logicCouple->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3, 0.5));
  visAttributes->SetVisibility(true);
  logicClrfiber->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.8));
  visAttributes->SetVisibility(true);
  logicMPPC->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  return fPhysicWorld;
}



void DetectorStructure::ConstructSDandField()
{
  if (!fmppcSD.Get()) {
    G4String mppcSDName = "/MPPC";
    auto mppcSD = new MPPC(mppcSDName);
    G4SDManager::GetSDMpointer()->AddNewDetector(mppcSD);
    fmppcSD.Put(mppcSD);
  }
  SetSensitiveDetector("MPPC", fmppcSD.Get(), true);
  G4String scinSDName = "/Scintillators";
  auto ScinSD = new Scintillators (scinSDName);
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(ScinSD);
  logicScintillator->SetSensitiveDetector(ScinSD);
  logicScintSideXZ->SetSensitiveDetector(ScinSD);
  logicScintSideYZ->SetSensitiveDetector(ScinSD);
  logicScintCrnr->SetSensitiveDetector(ScinSD);
  
  
}

//

void DetectorStructure::UpdateGeometryParameters()
{

  fClad1R = 1.03 * fWLSR;
  fClad1Z = fWLSZ;

  fClad2R = fClad1R + 0.03 * fWLSR;
  fClad2Z = fWLSZ;

  fClrfiberHalfL = fMPPCHalfL;
  fCoverEndR = fClad2R;

  fCoupleOrigin = fWLSOrigin + fWLSZ / 2 + fCoupleZ / 2;
  fCoverEndOrigin = fWLSOrigin - fWLSZ / 2 - fCoverEndZ / 2;
  fMPPCOriginX = std::sin(fMPPCTheta) * (fMPPCDist + fClrfiberHalfZ );
  fMPPCOriginZ = -fCoupleZ / 2 + std::cos(fMPPCTheta) * (fMPPCDist + fClrfiberHalfZ );
}



void DetectorStructure::SetWLSLength(G4double length)
// Set the TOTAL length of the WLS fiber
{
  fWLSZ = length;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetWLSRadius(G4double radius)
// Set the Y radius of WLS fiber
{
  fWLSR = radius;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetClad1Radius(G4double radius)
// Set the Y radius of Cladding 1
{
  fClad1R = radius;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetClad2Radius(G4double radius)
// Set the Y radius of Cladding 2
{
  fClad2R = radius;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetMPPCHalfLength(G4double halfL)
// Set the half length of the MPPC detector
// The half length will be the radius if MPPC is circular
{
  fMPPCHalfL = halfL;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetGap(G4double gap)
// Set the distance between fiber end and MPPC
{
  fMPPCDist = gap;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetMPPCAlignment(G4double theta)
// Set the Aligment of MPPC with respect to the z axis
// If theta is 0 deg, then the detector is perfectly aligned
// MPPC will be deviated by theta from z axis
// facing towards the center of the fiber
{
  fMPPCTheta = theta;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetSurfaceRoughness(G4double roughness)
// Set the Surface Roughness between Cladding 1 and WLS fiber
// Pre: 0 < roughness <= 1
{
  fSurfaceRoughness = roughness;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetCoverEndPolish(G4double polish)
// Set the Polish of the mirror, polish of 1 is a perfect mirror surface
// Pre: 0 < polish <= 1
{
  fCoverEndPolish = polish;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetCoverEndReflectivity(G4double reflectivity)
// Set the Reflectivity of the mirror, reflectivity of 1 is a perfect mirror
// Pre: 0 < reflectivity <= 1
{
  fCoverEndReflectivity = reflectivity;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorStructure::SetMPPCPolish(G4double polish)
// Set the Polish of the MPPC, polish of 1 is a perfect mirror surface
// Pre: 0 < polish <= 1
{
  fMPPCPolish = polish;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorStructure::SetMPPCReflectivity(G4double reflectivity)
// Set the Reflectivity of the MPPC, reflectivity of 1 is a perfect mirror
// Pre: 0 < reflectivity <= 1
{
  fMPPCReflectivity = reflectivity;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorStructure::SetBarLength(G4double length)
// Set the length of the scintillator bar
{
  fBarLength = length;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorStructure::SetBarWidth(G4double width)
// Set the side of the scintillator bar
{
  fBarWidth = width;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorStructure::SetBarThick(G4double thick)
// Set the side of the scintillator bar
{
  fBarThick = thick;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetHoleRadius(G4double radius)
// Set the radius of the fiber hole
{
  fHoleRadius = radius;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetCoatingThickness(G4double thick)
// Set thickness of the coating on the bars
{
  fCoatingThickness = thick;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

void DetectorStructure::SetCoatingRadius(G4double radius)
// Set inner radius of the corner bar coating
{
  fCoatingRadius = radius;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//

G4double DetectorStructure::GetWLSLength()
{
  return fWLSZ;
}

//

G4double DetectorStructure::GetBarLength()
{
  return fBarLength;
}

//

G4double DetectorStructure::GetBarWidth()
{
  return fBarWidth;
}

//
G4double DetectorStructure::GetBarThick()
{
  return fBarThick;
}

//

G4double DetectorStructure::GetHoleRadius()
{
  return fHoleRadius;
}

//

G4double DetectorStructure::GetHoleLength()
{
  return fHoleLength;
}

//

G4double DetectorStructure::GetWLSRadius()
{
  return fWLSR;
}

//

G4double DetectorStructure::GetCoatingThickness()
{
  return fCoatingThickness;
}

//

G4double DetectorStructure::GetCoatingRadius()
{
  return fCoatingRadius;
}

//

G4double DetectorStructure::GetWLSEnd()
{
  return fWLSOrigin + fWLSZ / 2;
}

//

G4double DetectorStructure::GetWLSRMax()
{
   return fClad2R;
}

//

G4double DetectorStructure::GetSurfaceRoughness()
{
  return fSurfaceRoughness;
}

//

// Return True if the fiber construction is ideal
G4bool DetectorStructure::IsPerfectFiber()
{
  return fSurfaceRoughness == 1. 
         && (fCoverEndPolish == 1. && fCoverEndReflectivity == 1.);
}

//

G4Material* DetectorStructure::FindMaterial(G4String name)
{
  G4Material* material = G4Material::GetMaterial(name, true);
  return material;
}
