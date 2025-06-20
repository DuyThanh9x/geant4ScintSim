#include "ScintillatorsHit.hh"

G4ThreadLocal G4Allocator <ScintillatorsHit>* ScintillatorsHitAllocator = 0;

ScintillatorsHit :: ScintillatorsHit () {};
ScintillatorsHit :: ~ScintillatorsHit () {};
G4bool ScintillatorsHit :: operator==(const ScintillatorsHit& right) const
{
	return (this == &right) ? true : false;
}
