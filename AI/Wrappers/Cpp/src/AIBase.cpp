/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "AIBase.h"
#include "SkirmishAI.h"
#include "OptionValues.h"
#include "SkirmishAIs.h"

AIBase::AIBase(springai::OOAICallback* callback):
callback(callback),
skirmishAIId(callback != 0 ? callback->GetSkirmishAIId() : -1),
numUnits(0),
numEnemies(0),
hq(0),
frame(0),
alldone(false),
score(0),
deathOccurred(false){
	callback->GetSkirmishAIs()->SetTheScore(score);
}

int AIBase::GetIntOption(char const* const key, int dflt){
	char const*const val(callback->GetSkirmishAI()->GetOptionValues()->GetValueByKey(key));
	return val?atoi(val):dflt;
}

std::string AIBase::GetStringOption(char const* const key){
	return callback->GetSkirmishAI()->GetOptionValues()->GetValueByKey(key);
}

void
AIBase::unitCreatedEvent(SUnitCreatedEvent* evt){
	AddUnit(evt->unit);
}

void
AIBase::commandFinishedEvent(SCommandFinishedEvent* evt){
	int unitId = evt->unitId;
	springai::Unit* u(GetFriendlyUnitById(unitId));
	//std::cout << "Command finished by " << u << "\n";
	if(ustat[u2i[unitId]]==waypoints[u2i[unitId]].size()-1){
		done[u2i[unitId]]=true;
		//std::cout << "restart\n";
		ustat[u2i[unitId]]=0;
		u->MoveTo(waypoints[u2i[unitId]][ustat[u2i[unitId]]],0);
		//std::cout << "MOVE TO " << waypoints[u2i[unitId]][ustat[u2i[unitId]]] << "\n";
		//std::cout << "Destruct " << *u << "\n";
		//u->SelfDestruct(0,0);
	/*}else if(ustat[u2i[unitId]]==waypoints[u2i[unitId]].size()-2){
		std::cout << "patrol\n";
		// Last command... patrol between points
		u->PatrolTo(waypoints[u2i[unitId]][++ustat[u2i[unitId]]],0);
		std::cout << "PATROL TO " << waypoints[u2i[unitId]][ustat[u2i[unitId]]] << "\n";
	*/}else{
		//std::cout << "move\n";
		u->MoveTo(waypoints[u2i[unitId]][++ustat[u2i[unitId]]],0);
		//std::cout << "MOVE TO " << waypoints[u2i[unitId]][ustat[u2i[unitId]]] << "\n";
	}
}

void
AIBase::weaponFiredEvent(SWeaponFiredEvent* evt) {
	springai::WeaponDef* wpn(callback->GetWeaponDefs()[evt->weaponDefId]);
	float intensity(wpn->GetIntensity());
	std::cout << "IR event intensity: " << intensity << "\n";
	if(u2i.find(evt->unitId)!=u2i.end()){
		// My unit fired a weapon, lose points
		score -= 10;
		callback->GetSkirmishAIs()->SetTheScore(score);
	}
}

void
AIBase::enemyEnterRadarEvent(SEnemyEnterRadarEvent* evt){
	score+=1; // Some points for detecting an entity
	callback->GetSkirmishAIs()->SetTheScore(score);
	eu2i[evt->enemy]=numEnemies++;
}

void
AIBase::enemyDamagedEvent(SEnemyDamagedEvent* evt){
	springai::Unit* u(GetEnemyUnitById(evt->enemy));
	score += u->GetDef()->GetCost(callback->GetResourceByName("Metal"))/10;
	callback->GetSkirmishAIs()->SetTheScore(score);
}

void
AIBase::enemyDestroyedEvent(SEnemyDestroyedEvent* evt){
	springai::Unit* u(GetEnemyUnitById(evt->enemy));
	score += u->GetDef()->GetCost(callback->GetResourceByName("Metal"));
	callback->GetSkirmishAIs()->SetTheScore(score);
}


void
AIBase::unitDamagedEvent(SUnitDamagedEvent* evt){
	springai::Unit* u(GetFriendlyUnitById(evt->unit));
	score -= u->GetDef()->GetCost(callback->GetResourceByName("Metal"))/10;
	callback->GetSkirmishAIs()->SetTheScore(score);
}

void
AIBase::unitDestroyedEvent(SUnitDestroyedEvent* evt){
	springai::Unit* u(GetFriendlyUnitById(evt->unit));
	score -= u->GetDef()->GetCost(callback->GetResourceByName("Metal"));
	callback->GetSkirmishAIs()->SetTheScore(score);
	done[u2i[evt->unit]]=true;
	deathOccurred=true;
}

void
AIBase::enemyLeaveRadarEvent(SEnemyLeaveRadarEvent* evt){
	// Wait for a small amount of time, then turn off Radar
}


void AIBase::AddUnit(int unitId){
	friends=callback->GetFriendlyUnits();
	springai::Unit* u(GetFriendlyUnitById(unitId));
	if(u){
		std::string name(u->GetDef()->GetName());
		u->SetFireState(utils::FIRESTATE_RETURNFIRE);
		u2i[unitId]=numUnits++;
		ustat.push_back(0);
		std::vector<springai::AIFloat3> wpts;
		std::vector<float> raw(callback->GetUnitPaths(callback->GetGame()->GetMyTeam(),unitId));
		for(int i(0); i<raw.size(); i+=3){
			wpts.push_back(springai::AIFloat3(raw[i],raw[i+1],raw[i+2]));
		}
		waypoints.push_back(wpts);

		std::cout << u << "\n";
		if(wpts.size()>1){
			done.push_back(false);
			std::cout << "now have " << this->numUnits << " mobile units\n";
			u->MoveTo(waypoints[u2i[unitId]][++ustat[u2i[unitId]]],0);
			//std::cout << "MOVE TO " << waypoints[u2i[unitId]][ustat[u2i[unitId]]] << "\n";

		}else{
			done.push_back(true);
			// Do we need to do anything for non-mobile units?
		}
	}
}

springai::Unit* AIBase::GetEnemyUnitById(int id) const{
	springai::Unit* ut(0);
	GetUnitById(id,callback->GetEnemyUnits(),&ut);
	return ut;
}

springai::Unit* AIBase::GetFriendlyUnitById(int id) const{
	springai::Unit* ut(0);
	GetUnitById(id,friends,&ut);
	return ut;
}

int AIBase::HandleEvent(int topic, const void* data) {
	int retval(0);
	switch (topic) {
	case EVENT_UNIT_CREATED: {
		unitCreatedEvent((SUnitCreatedEvent*) data);
		break;
	}
	case EVENT_COMMAND_FINISHED: {
		commandFinishedEvent((SCommandFinishedEvent*) data);
		break;
	}
		case EVENT_WEAPON_FIRED: {
			weaponFiredEvent((SWeaponFiredEvent*) data);
			break;
		}
		case EVENT_ENEMY_LEAVE_LOS:{
			enemyLeaveLOSEvent((SEnemyLeaveLOSEvent*) data);
			break;
		}
		case EVENT_ENEMY_ENTER_LOS:{
			enemyEnterLOSEvent((SEnemyEnterLOSEvent*) data);
			break;
		}
		case EVENT_ENEMY_LEAVE_RADAR:{
			enemyLeaveRadarEvent((SEnemyLeaveRadarEvent*) data);
			break;
		}
		case EVENT_ENEMY_ENTER_RADAR:{
			enemyEnterRadarEvent((SEnemyEnterRadarEvent*) data);
			break;
		}
		case EVENT_ENEMY_DESTROYED: {
			enemyDestroyedEvent((SEnemyDestroyedEvent*) data);
			break;
		}
		case EVENT_ENEMY_DAMAGED: {
			enemyDamagedEvent((SEnemyDamagedEvent*) data);
			break;
		}
		case EVENT_UNIT_DAMAGED:{
			unitDamagedEvent((SUnitDamagedEvent*) data);
			break;
		}
		case EVENT_RADAR_CHANGED: {
			radarChangedEvent((SRadarChangedEvent*) data);
			break;
		}
		case EVENT_PROJECTILE_MOVED: {
			projectileMovedEvent((SProjectileMovedEvent*) data);
			break;
		}
		case EVENT_UNIT_DESTROYED: {
			unitDestroyedEvent((SUnitDestroyedEvent*) data);
			break;
		}
		default: {
			retval = defaultEvent(topic, data);
			break;
		}
	}

	// signal: everything went OK
	return retval;
}

template<typename Out>
void AIBase::split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}


std::vector<std::string> AIBase::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    AIBase::split(s, delim, std::back_inserter(elems));
    return elems;
}
	template<typename Out>
	void split(const std::string &s, char delim, Out result) {
	    std::stringstream ss;
	    ss.str(s);
	    std::string item;
	    while (std::getline(ss, item, delim)) {
	        *(result++) = item;
	    }
	}


	std::vector<std::string> split(const std::string &s, char delim) {
	    std::vector<std::string> elems;
	    split(s, delim, std::back_inserter(elems));
	    return elems;
	}
