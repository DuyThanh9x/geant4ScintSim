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

#ifndef EventAction_h
#define EventAction_h 1

#include "G4Types.hh"
#include "G4UserEventAction.hh"

class EventActionMess;

class EventAction : public G4UserEventAction
{
  public:
    EventAction ();
    ~EventAction () override;

    void BeginOfEventAction (const G4Event*) override;
    void EndOfEventAction (const G4Event*) override;

    G4int GetEventNo ();
    void SetEventVerbose (G4int);

    void AddTIR () { fNTIR += 1; };
    void AddExiting () { fNExiting += 1; };
    void AddEscapedEnd () { fEscapedEnd += 1; };
    void AddEscapedSide () { fEscapedSide += 1; };
    void AddBounce () { fBounce += 1; };
    void AddWLSBounce () { fWLSBounce += 1; };
    void AddClad1Bounce () { fClad1Bounce += 1; };
    void AddClad2Bounce () { fClad2Bounce += 1; };
    void AddReflected () { fReflected += 1; };
    void AddEscaped () { fEscaped += 1; };
    void AddCoverEnd () { fCoverEnd += 1; };
    
    void AddMudE (G4double MudE) {fTotalMudE += MudE;}
    //void AddMuStep () {fTotalMuStep += 1;}
    void AddMudX (G4double Mudx) {fTotalMudX += Mudx;}
    void AddElecdE (G4double ElecdE) {fTotalElecdE += ElecdE;}
    //void AddElecStep () {fTotalElecStep += 1;}
    void AddElecdX (G4double Elecdx) {fTotalElecdX += Elecdx;}
    void SetCosinePolarisedPositron (G4double cv) {CosinePolarisedPositron = cv;}

  private:
    EventActionMess* fEventMessenger = nullptr;

    G4int fVerboseLevel = 0;

    G4int fMPPCCollID = 0;
    
    G4int ScinHitID = -1;

    G4int fNTIR = 0;
    G4int fNExiting = 0;
    G4int fEscapedEnd = 0;
    G4int fEscapedSide = 0;
    G4int fBounce = 0;
    G4int fWLSBounce = 0;
    G4int fClad1Bounce = 0;
    G4int fClad2Bounce = 0;
    G4int fReflected = 0;
    G4int fEscaped = 0;
    G4int fCoverEnd = 0;
    
    G4double fTotalMudE = 0;
    G4double CosinePolarisedPositron = -2;
    G4double fTotalMudX = 0;
    G4double fTotalElecdE = 0;
    //G4double fTotalElecStep = 0;
    G4double fTotalElecdX = 0;
};

#endif
