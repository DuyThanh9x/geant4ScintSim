#ifndef Scintillators_h
#define Scintillators_h 1

#include "ScintillatorsHit.hh"
#include "G4VSensitiveDetector.hh"
#include <set>

class G4Step;
class G4HCofThisEvent;

class Scintillators : public G4VSensitiveDetector
{
  public:
    Scintillators (G4String);
    ~Scintillators () override = default;

    void Initialize(G4HCofThisEvent*) override;

    G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;

    void EndOfEvent(G4HCofThisEvent*) override;
  private:
  	ScintillatorsHitCollect *ScintHitCollect = nullptr;
  	G4int fHCID = -1;
};

#endif
