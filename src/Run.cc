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

#include "Run.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
//
Run::Run() : G4Run() {}

Run::~Run() {}

void Run::RecordEvent(const G4Event* anEvent)
{
	G4int nEvt = anEvent->GetEventID();
	if (nEvt % 1000 == 0) {
		G4cout << "=========== Event ID number " << nEvt << G4endl;
	}
	G4Run::RecordEvent(anEvent);
}

void Run::Merge(const G4Run* aRun)
{
	const Run* localRun = static_cast<const Run*>(aRun);
	PrimaryParticleIdRun = localRun->GetPrimaryParticleId();
	PrimaryParticleInitialKineticEnergyRun = localRun->GetPrimaryParticleInitialKineticEnergy();
	PrimaryParticleInitialTotalEnergyRun = localRun->GetPrimaryParticleInitialTotalEnergy();
	PrimaryParticleInitial3MomentumRun = localRun->GetPrimaryParticleInitial3Momentum();
	PrimaryParticleInitialPositionRun = localRun->GetPrimaryParticleInitialPosition();
	NumDecaysAtRestRun += localRun->GetNumberDecaysAtRest();
	NumDecaysInFlightRun += localRun->GetNumberDecaysInFlight();
	
	G4Run::Merge(aRun);
}

void Run::EndOfRun() const
{
  if (numberOfEvent == 0) return;
  auto TotNbofEvents = G4double(numberOfEvent);
  G4int prec = G4cout.precision(3);
  G4cout << "\n+++++++++++++++++++: Run summary :+++++++++++++++++++\n";
  G4cout << "Total number of events                    : " << TotNbofEvents << G4endl
  	 << "Primary PDG code                          : " << PrimaryParticleIdRun << G4endl
  	 << "Primary initial kinetic energy [MeV]      : " << PrimaryParticleInitialKineticEnergyRun << G4endl
  	 << "Primary initial total energy [MeV]        : " << PrimaryParticleInitialTotalEnergyRun << G4endl
  	 << "Primary initial momentum [MeV]            : " << PrimaryParticleInitial3MomentumRun << G4endl
  	 << "Primary initial position [cm]             : " << PrimaryParticleInitialPositionRun / CLHEP::cm << G4endl
  	 << "Number of decays At Rest                  : " << NumDecaysAtRestRun << G4endl
  	 << "Number of decays In Flight                : " << NumDecaysInFlightRun << G4endl;
  G4cout << G4endl;
  G4cout.precision(prec);
  
  //auto analysisManager = G4AnalysisManager::Instance();
  
}
