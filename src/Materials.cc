#include "Materials.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

Materials* Materials::fInstance = nullptr;

Materials :: Materials()
{
  fNistMan = G4NistManager::Instance();
  fNistMan->SetVerbose(2);

  CreateMaterials();
}

Materials :: ~Materials()
{
  delete fAir;
  delete fH;
  delete fScint;
  delete fWLS;
  delete fCladding1;
  delete fCladding2;
  delete fSilicone;
  delete fCoating;
}

Materials* Materials::GetInstance()
{
  if (!fInstance) {
    fInstance = new Materials();
  }	
  return fInstance;
}

G4Material* Materials::GetMaterial(const G4String material)
{
  G4Material* mat = fNistMan->FindOrBuildMaterial(material);

  if (!mat) mat = G4Material::GetMaterial(material);
  if (!mat) {
    G4ExceptionDescription ed;
    ed << "Material " << material << " not found!";
    G4Exception("Materials::GetMaterial", "", FatalException, ed);
  }

  return mat;
}

void Materials::CreateMaterials()
{
  G4double density;
  G4int ncomponents;
  G4double fractionmass;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;


  //--------------------------------------------------
  // Vacuum
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Galactic");

  //--------------------------------------------------
  // Air
  //--------------------------------------------------

  fAir = fNistMan->FindOrBuildMaterial("G4_AIR");

  //fH = fNistMan->FindOrBuildMaterial("G4_H");
	
  //fH = fNistMan->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  //--------------------------------------------------
  // Scintillator
  //--------------------------------------------------

  elements.push_back("C");
  natoms.push_back(8);
  elements.push_back("H");
  natoms.push_back(8);

  density = 1.050 * g / cm3;

  fScint = fNistMan->ConstructNewMaterial("scint", elements, natoms, density);
  fScint->GetIonisation()->SetBirksConstant(
        (0.898E-2)*g/cm2/MeV/density);
    std::vector<G4double> energySmall = {2.0 * eV, 3.47 * eV};
        
  std::vector<G4double> refractiveIndexPS = {1.59, 1.59};

  std::vector<G4double> absPS = {2. * m, 2. * m};
  std::vector<G4double> energy = {
    2.00 * eV, 2.03 * eV, 2.06 * eV, 2.09 * eV, 2.12 * eV, 2.15 * eV, 2.18 * eV, 2.21 * eV,
    2.24 * eV, 2.27 * eV, 2.30 * eV, 2.33 * eV, 2.36 * eV, 2.39 * eV, 2.42 * eV, 2.45 * eV,
    2.48 * eV, 2.51 * eV, 2.54 * eV, 2.57 * eV, 2.60 * eV, 2.63 * eV, 2.66 * eV, 2.69 * eV,
    2.72 * eV, 2.75 * eV, 2.78 * eV, 2.81 * eV, 2.84 * eV, 2.87 * eV, 2.90 * eV, 2.93 * eV,
    2.96 * eV, 2.99 * eV, 3.02 * eV, 3.05 * eV, 3.08 * eV, 3.11 * eV, 3.14 * eV, 3.17 * eV,
    3.20 * eV, 3.23 * eV, 3.26 * eV, 3.29 * eV, 3.32 * eV, 3.35 * eV, 3.38 * eV, 3.41 * eV,
    3.44 * eV, 3.47 * eV};

  std::vector<G4double> scintilFast = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    
    std::vector<G4double> energyE = {
    0.5*MeV, 1*MeV, 10*MeV, 100*MeV};
    
    std::vector<G4double> YieldE = {
    10/keV, 10/keV, 10/keV, 10/keV};

  // Add entries into properties table
  auto mptPolystyrene = new G4MaterialPropertiesTable();
  mptPolystyrene->AddProperty("RINDEX", energySmall, refractiveIndexPS);
  mptPolystyrene->AddProperty("ABSLENGTH", energySmall, absPS);
  mptPolystyrene->AddProperty("SCINTILLATIONCOMPONENT1", energy, scintilFast);
  mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
  //mptPolystyrene->AddProperty("ELECTRONSCINTILLATIONYIELD", energyE, YieldE);
  mptPolystyrene->AddConstProperty("RESOLUTIONSCALE", 1.0);
  mptPolystyrene->AddConstProperty("SCINTILLATIONTIMECONSTANT1",2.8 * ns);
  mptPolystyrene->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10. * ns);

  fScint->SetMaterialPropertiesTable(mptPolystyrene);

  elements.clear();
  natoms.clear();


  //--------------------------------------------------
  // Aluminium
  //--------------------------------------------------

  fNistMan->FindOrBuildMaterial("G4_Al");

  //--------------------------------------------------
  // TiO2
  //--------------------------------------------------

  elements.push_back("Ti");
  natoms.push_back(1);
  elements.push_back("O");
  natoms.push_back(2);

  density = 4.26 * g / cm3;

  G4Material* TiO2 = fNistMan->ConstructNewMaterial("TiO2", elements, natoms, density);

  elements.clear();
  natoms.clear();

  //--------------------------------------------------
  // Scintillator Coating - 15% TiO2 and 85% polystyrene by weight.
  //--------------------------------------------------

  density = 1.52 * g / cm3;

  fCoating = new G4Material("Coating", density, ncomponents = 2);

  fCoating->AddMaterial(TiO2, fractionmass = 15 * perCent);
  fCoating->AddMaterial(fScint, fractionmass = 85 * perCent);

  

  //--------------------------------------------------
  // Air
  //--------------------------------------------------

  std::vector<G4double> refractiveIndex = {1.0, 1.0};

  auto mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("RINDEX", energySmall, refractiveIndex);

  fAir->SetMaterialPropertiesTable(mpt);

  //--------------------------------------------------
  //  PMMA for WLSfibers
  //--------------------------------------------------

//--------------------------------------------------
  // WLS
  //--------------------------------------------------

  elements.push_back("C");
  natoms.push_back(9);
  elements.push_back("H");
  natoms.push_back(10);
  //elements.push_back("O");
  //natoms.push_back(2);

  density = 1.050 * g / cm3;

  fWLS = fNistMan->ConstructNewMaterial("WLSMaterial", elements, natoms, density);
    std::vector<G4double> refractiveIndexWLSfiber = {1.590, 1.590};
  std::vector<G4double> absWLSfiber = {
    2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m,
    2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m,
    2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m, 2.40 * m,
    2.40 * m, 2.40 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m,
    1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,
    1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm};

  std::vector<G4double> emissionFib = {0.05, 0.10, 0.30, 0.50, 0.75, 1.00, 1.50, 1.85, 2.30, 2.75,
                                       3.25, 3.80, 4.50, 5.20, 6.00, 7.00, 8.50, 9.50, 11.1, 12.4,
                                       12.9, 13.0, 12.8, 12.3, 11.1, 11.0, 12.0, 11.0, 17.0, 16.9,
                                       15.0, 9.00, 2.50, 1.00, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00,
                                       0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};

  // Add entries into properties table
  auto mptWLSfiber = new G4MaterialPropertiesTable();
  mptWLSfiber->AddProperty("RINDEX", energySmall, refractiveIndexWLSfiber);
  mptWLSfiber->AddProperty("WLSABSLENGTH", energy, absWLSfiber);
  mptWLSfiber->AddProperty("WLSCOMPONENT", energy, emissionFib);
  mptWLSfiber->AddConstProperty("WLSTIMECONSTANT", 7 * ns);

  fWLS->SetMaterialPropertiesTable(mptWLSfiber);
  elements.clear();
  natoms.clear();
  
  //--------------------------------------------------
  // Cladding1
  //--------------------------------------------------

  elements.push_back("C");
  natoms.push_back(5);
  elements.push_back("H");
  natoms.push_back(8);
  elements.push_back("O");
  natoms.push_back(2);

  density = 1.190 * g / cm3;

  fCladding1 = fNistMan->ConstructNewMaterial("Cladding1", elements, natoms, density);

  std::vector<G4double> refractiveIndexClad1 = {1.49, 1.49};

  std::vector<G4double> absClad = {20.0 * m, 20.0 * m};

  // Add entries into properties table
  auto mptClad1 = new G4MaterialPropertiesTable();
  mptClad1->AddProperty("RINDEX", energySmall, refractiveIndexClad1);
  mptClad1->AddProperty("ABSLENGTH", energySmall, absClad);

  fCladding1->SetMaterialPropertiesTable(mptClad1);
  elements.clear();
  natoms.clear();/**/

  //--------------------------------------------------
  // Double Cladding (fluorinated polyethylene)
  //--------------------------------------------------

  elements.push_back("C");
  natoms.push_back(2);
  elements.push_back("H");
  natoms.push_back(4);

  density = 1.430 * g / cm3;

  fCladding2 = fNistMan->ConstructNewMaterial("Cladding2", elements, natoms, density);

  std::vector<G4double> refractiveIndexClad2 = {1.42, 1.42};

  // Add entries into properties table
  auto mptClad2 = new G4MaterialPropertiesTable();
  mptClad2->AddProperty("RINDEX", energySmall, refractiveIndexClad2);
  mptClad2->AddProperty("ABSLENGTH", energySmall, absClad);

  fCladding2->SetMaterialPropertiesTable(mptClad2);
  elements.clear();
  natoms.clear();
}

