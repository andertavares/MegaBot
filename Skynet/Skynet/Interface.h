#include <BWAPI.h>
#include <boost\tr1\memory.hpp>

typedef BWAPI::TilePosition TilePosition;
typedef BWAPI::Position Position;
typedef BWAPI::Player* Player;

class UnitClass;
typedef std::tr1::shared_ptr<UnitClass> Unit;

#include "Vector.h"
#include "WalkPosition.h"
#include "Unit.h"
#include "UnitGroup.h"