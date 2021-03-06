#ifndef AIUTILS_H
#define AIUTILS_H

#include "OOAICallback.h"
#include "Unit.h"
#include "Team.h"
#include "UnitDef.h"
#include <ostream>
#include <sstream>

namespace utils{
enum {
	        MOVESTATE_NONE     = -1,
	        MOVESTATE_HOLDPOS  =  0,
	        MOVESTATE_MANEUVER =  1,
	        MOVESTATE_ROAM     =  2,
	};
	enum {
	        FIRESTATE_NONE       = -1,
	        FIRESTATE_HOLDFIRE   =  0,
	        FIRESTATE_RETURNFIRE =  1,
	        FIRESTATE_FIREATWILL =  2,
	        FIRESTATE_FIREATNEUTRAL =  3,
	};
	enum {
	  MODE_OFF = 0,
	  MODE_SEARCH = 1,
	  MODE_ACQUIRE = 2,
	  MODE_TRACK = 3,
	  MODE_ILLUMINATE = 4,
	  MODE_GUIDANCE = 5
	};
	static const int NUM_MODES(6);
	static const std::string MODES[NUM_MODES]={"OFF","SEARCH","ACQUIRE","TRACK","ILLUMINATE","GUIDANCE"};
};

static void GetUnitById(int id, std::vector<springai::Unit*> const& units, springai::Unit** unit){
	for(auto u: units){
		if(id==u->GetUnitId()){
			*unit = u;
			break;
		}
	}
}



static std::ostream& operator <<(std::ostream& ss, springai::AIFloat3 const& vec){
	ss<<vec.x<<","<<vec.y<<","<<vec.z;
	return ss;
}
static std::ostream& operator <<(std::ostream& ss, springai::Unit *const unit){
	springai::UnitDef* unitDef(unit->GetDef());
	ss<<"Unit "<<unitDef->GetName()<<"-"<<unit->GetUnitId()<<":"<<"\n  "
			<< "NAME: "<<unitDef->GetName()<<"\n  "
			<< "HNAME: "<<unitDef->GetHumanName()<<"\n  "
			<< "CATEGORY: "<<unitDef->GetCategoryString()<<"\n  "
			<< "AIR LOS: "<<unitDef->GetAirLosRadius()<<"\n  "
			<< "JAM RADIUS: "<<unitDef->GetJammerRadius()<<"\n  "
			<< "LOS HEIGHT: "<<unitDef->GetLosHeight()<<"\n  "
			<< "LOS RADIUS: "<<unitDef->GetLosRadius()<<"\n  "
			<< "RADAR RADIUS: "<<unitDef->GetRadarRadius()<<"\n  "
			<< "STUNNED: "<<unit->IsParalyzed()<<"\n  "
			<< "ACTIVE: "<<unit->IsActivated()<<"\n  "
			<< "BUILDING: "<<unit->IsBeingBuilt()<<"\n  "
			<< "LOC: "<<unit->GetPos()<<"\n  "
			<< "VEL: "<<unit->GetVel();
	return ss;
}

static std::ostream& operator <<(std::ostream& ss, springai::Unit & unit){
	ss<<unit.GetDef()->GetTooltip()<<"-"<<unit.GetUnitId();
	return ss;
}

#endif
