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

#include "ActionInitial.hh"

#include "DetectorStructure.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "StackingAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"

ActionInitial :: ActionInitial (DetectorStructure* det) : G4VUserActionInitialization(), fDetector(det)
{}

ActionInitial :: ~ActionInitial () {}

void ActionInitial :: BuildForMaster () const
{
  SetUserAction(new RunAction);
}

void ActionInitial :: Build () const
{
  SetUserAction(new PrimaryGeneratorAction(fDetector));
  auto eventAction = new EventAction();
  SetUserAction(eventAction);
  auto stepAct = new SteppingAction(fDetector, eventAction);
  SetUserAction(new RunAction(stepAct));

  SetUserAction(new TrackingAction());
  SetUserAction(new SteppingAction(fDetector, eventAction));
  SetUserAction(new StackingAction());
}
