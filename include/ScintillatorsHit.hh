#ifndef ScintillatorsHit_h
#define ScintillatorsHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

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
		
		void SetHitTrackParentID (G4int id) {trackID = id;};
		void SetTrackParticleName (G4String part) {track = part;};
		void SetTrackProcess (G4String nameProcess) {process = nameProcess;};
		void SetNameVolume (G4String name) {volume = name;};
		void SetVolumeNumber (G4int n) {volumeN = n;};
		void SetdE (G4double Edepo) {dE = Edepo;};
		void SetdX (G4double L) {dX = L;};
		
		G4int GetHitTrackParentID () const {return trackID;};
		G4String GetTrackParticleName () const {return track;};
		G4String GetTrackProcess () const {return process;};
		G4String GetNameVolume () const {return volume;};
		G4int GetVolumeNumber () const {return volumeN;};
		G4double GetdE () const {return dE;};
		G4double GetdX () const {return dX;};
		
	private:
		G4int trackID = -1;
		G4String track = "";
		G4String process = "";
		G4String volume = "";
		G4int volumeN = -1;
		G4double dE = -1;
		G4double dX = -1;
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
