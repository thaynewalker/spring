/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "AIBase.h"

AIBase::AIBase(springai::OOAICallback* callback):
callback(callback),
skirmishAIId(callback != 0 ? callback->GetSkirmishAIId() : -1),
numUnits(0),
hq(0),
frame(0),
myPoints(0.0)
{
}

void
AIBase::unitCreatedEvent(SUnitCreatedEvent* evt){
	AddUnit(evt->unit);
}

void
AIBase::commandFinishedEvent(SCommandFinishedEvent* evt){
	int unitId = evt->unitId;
	springai::Unit* u(GetFriendlyUnitById(unitId));
	std::cout << "Command finished by " << u << "\n";
	if(ustat[u2i[unitId]]==waypoints[u2i[unitId]].size()-1){
		done[u2i[unitId]]=true;
		std::cout << "restart\n";
		ustat[u2i[unitId]]=0;
		u->MoveTo(waypoints[u2i[unitId]][ustat[u2i[unitId]]],0);
		std::cout << "MOVE TO " << waypoints[u2i[unitId]][ustat[u2i[unitId]]] << "\n";
		//std::cout << "Destruct " << *u << "\n";
		//u->SelfDestruct(0,0);
	/*}else if(ustat[u2i[unitId]]==waypoints[u2i[unitId]].size()-2){
		std::cout << "patrol\n";
		// Last command... patrol between points
		u->PatrolTo(waypoints[u2i[unitId]][++ustat[u2i[unitId]]],0);
		std::cout << "PATROL TO " << waypoints[u2i[unitId]][ustat[u2i[unitId]]] << "\n";
	*/}else{
		std::cout << "move\n";
		u->MoveTo(waypoints[u2i[unitId]][++ustat[u2i[unitId]]],0);
		std::cout << "MOVE TO " << waypoints[u2i[unitId]][ustat[u2i[unitId]]] << "\n";
	}
}

void AIBase::AddUnit(int unitId){
	const std::vector<springai::Unit*> friendlyUnits = callback->GetFriendlyUnits();
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

		if(wpts.size()>1){
			done.push_back(false);
			std::cout << "now have " << this->numUnits << " mobile units\n";
			std::cout << u << "\n";
			u->MoveTo(waypoints[u2i[unitId]][++ustat[u2i[unitId]]],0);
			std::cout << "MOVE TO " << waypoints[u2i[unitId]][ustat[u2i[unitId]]] << "\n";

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
	GetUnitById(id,callback->GetFriendlyUnits(),&ut);
	return ut;
}

int AIBase::HandleEvent(int topic, const void* data) {
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
		case EVENT_UNIT_DESTROYED: {
			unitDestroyedEvent((SUnitDestroyedEvent*) data);
			break;
		}
		default: {
			defaultEvent();
			break;
		}
	}

	// signal: everything went OK
	return 0;
}

double AIBase::iscore=0.0;
double AIBase::rscore=0.0;
bool AIBase::idone=false;
bool AIBase::rdone=false;
