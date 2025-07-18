#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsList : public G4VModularPhysicsList
{
  public:
    PhysicsList();
    virtual ~PhysicsList();

    virtual void ConstructParticle();
    virtual void ConstructProcess();
    void SetCuts() override;
  private:
    G4VModularPhysicsList* physicsList;
};
#endif
