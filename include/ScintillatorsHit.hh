#ifndef ScintillatorsHit_h
#define ScintillatorsHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class ScintillatorsHit : public G4VHit
{
	public:
		ScintillatorsHit();
		ScintillatorsHit(const ScintillatorsHit&) = default;
		~ScintillatorsHit() override;
		
		ScintillatorsHit& operator = (const ScintillatorsHit&) = default;
		G4bool operator == (const ScintillatorsHit&) const;
		inline void* operator new(size_t);
		inline void operator delete(void*);
		
		void SetHitTrackID (G4int id) {trackID = id;};
		void SetHitParentID (G4int id) {parentID = id;};
		void SetTrackParticleName (G4String part) {track = part;};
		void SetNameVolume (G4String name) {volume = name;};
		void SetVolumeNumber (G4int n) {volumeN = n;};
		void SetStepProcess (G4String name) {pr = name;};
		void SetdE (G4double Edepo) {dE = Edepo;};
		void SetdX (G4double L) {dX = L;};
		void SetEndPointKineticE (G4double n) {EndPointKineticE = n;};
		void SetKineticE (G4double n) {KineticE = n;};
		void SetMomentumDirection (G4ThreeVector n) {MomentumDirection = n;};
		void SetStepNumber (G4int E) {StepNumber = E;};
		//void SetDaughterdX (G4double L) {DaughterdX = L;};
		
		G4int GetHitTrackID () const {return trackID;};
		G4int GetHitParentID () const {return parentID;};
		G4String GetTrackParticleName () const {return track;};
		G4String GetNameVolume () const {return volume;};
		G4int GetVolumeNumber () const {return volumeN;};
		G4String GetStepProcess () const {return pr;};
		G4double GetdE () const {return dE;};
		G4double GetdX () const {return dX;};
		G4double GetEndPointKineticE () const {return EndPointKineticE;};
		G4ThreeVector GetMomentumDirection () const {return MomentumDirection;};
		G4double GetKineticE () const {return KineticE;};
		G4double GetStepNumber () const {return StepNumber;};
		
	private:
		G4int trackID = -1;
		G4int parentID = -1;
		G4String track = "";
		G4String volume = "";
		G4int volumeN = -1;
		G4String pr = "";
		G4double dE = -1;
		G4double dX = -1;
		G4double KineticE = -1;
		G4double EndPointKineticE = -1;
		G4ThreeVector MomentumDirection = G4ThreeVector(0.0, 0.0, 0.0);
		G4int StepNumber = -1;
		//G4double DaughterdX = -1;
};

typedef G4THitsCollection <ScintillatorsHit> ScintillatorsHitCollect;
extern G4ThreadLocal G4Allocator <ScintillatorsHit>* ScintillatorsHitAllocator;

inline void* ScintillatorsHit :: operator new(size_t)
{
	if (!ScintillatorsHitAllocator)
		ScintillatorsHitAllocator = new G4Allocator <ScintillatorsHit>;
	return (void*) ScintillatorsHitAllocator -> MallocSingle();
}

inline void ScintillatorsHit :: operator delete(void* hit)
{
	ScintillatorsHitAllocator -> FreeSingle ( (ScintillatorsHit* ) hit);
}
#endif
