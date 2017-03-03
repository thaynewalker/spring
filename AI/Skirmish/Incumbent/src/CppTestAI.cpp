/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "CppTestAI.h"


#include "ExternalAI/Interface/AISEvents.h"
#include "ExternalAI/Interface/AISCommands.h"

// generated by the C++ Wrapper scripts
#include "OOAICallback.h"
#include "Unit.h"
#include "UnitDef.h"
#include "Game.h"

#include <string>
#include <iostream>
#include <ostream>

cpptestai::CCppTestAI::CCppTestAI(springai::OOAICallback* callback):
		callback(callback),
		skirmishAIId(callback != NULL ? callback->GetSkirmishAIId() : -1)
		{}

cpptestai::CCppTestAI::~CCppTestAI() {}

static inline std::string IntToString(int i, const std::string& format = "%i")
{
	char buf[64];
	SNPRINTF(buf, sizeof(buf), format.c_str(), i);
	return std::string(buf);
}

static std::ostream& operator <<(std::ostream& ss, springai::AIFloat3 const& vec){
	ss<<vec.x<<","<<vec.y<<","<<vec.z;
	return ss;
}
static std::ostream& operator <<(std::ostream& ss, springai::Unit *const unit){
	springai::UnitDef* unitDef(unit->GetDef());
	ss<<"Unit "<<unitDef->GetType()<<"-"<<unit->GetUnitId()<<":"<<"\n  "
			<< "NAME: "<<unitDef->GetName()<<"\n  "
			<< "HNAME: "<<unitDef->GetHumanName()<<"\n  "
			<< "CATEGORY: "<<unitDef->GetCategoryString()<<"\n  "
			<< "AIR LOS: "<<unitDef->GetAirLosRadius()<<"\n  "
			<< "JAM RADIUS: "<<unitDef->GetJammerRadius()<<"\n  "
			<< "LOS HEIGHT: "<<unitDef->GetLosHeight()<<"\n  "
			<< "LOS RADIUS: "<<unitDef->GetLosRadius()<<"\n  "
			<< "RADAR RADIUS: "<<unitDef->GetRadarRadius()<<"\n  "
			<< "LOC: "<<unit->GetPos()<<"\n  "
			<< "VEL: "<<unit->GetVel();
	return ss;
}
int cpptestai::CCppTestAI::HandleEvent(int topic, const void* data) {

	switch (topic) {
		case EVENT_UNIT_CREATED: {
			//struct SUnitCreatedEvent* evt = (struct SUnitCreatedEvent*) data;
			//int unitId = evt->unit;

			// TODO: wrap events and commands too

			const std::vector<springai::Unit*> friendlyUnits = callback->GetFriendlyUnits();
			std::string msgText = std::string("HELLO Engine (from Incumbent), Unit# ") + IntToString(friendlyUnits.size());
			if (!friendlyUnits.empty()) {
				springai::Unit* unit = friendlyUnits[0];
				std::cout << unit << "\n";
				springai::UnitDef* unitDef = unit->GetDef();
				std::string unitDefName = unitDef->GetName();
				msgText = msgText + ", first friendly units def name is: " + unitDefName;
			}
			callback->GetGame()->SendTextMessage(msgText.c_str(), 0);

			break;
		}
		case EVENT_COMMAND_FINISHED: {
			break;
		}
		case EVENT_WEAPON_FIRED: {
			std::cout << "IR event" << "\n";
			break;
		}
		case EVENT_ENEMY_LEAVE_LOS:{
			// Wait for a small amount of time, then turn off
			break;
		}
		case EVENT_ENEMY_ENTER_LOS:{
			// Ground units may fire weapon or something
			break;
		}
		case EVENT_ENEMY_LEAVE_RADAR:{
			// Wait for a small amount of time, then turn off radar (for SA units)
			break;
		}
		case EVENT_ENEMY_ENTER_RADAR:{
			// EW - Simulate a comms event from the unit to other units
			// SAM - Turn on radar
			break;
		}
		case EVENT_ENEMY_DESTROYED: {
			// Points
			break;
		}
		case EVENT_ENEMY_DAMAGED: {
			// Points
			break;
		}
		case EVENT_UNIT_DAMAGED:{
			// Point loss
			break;
		}
		case EVENT_UNIT_DESTROYED: {
			// Point loss
			break;
		}
		default: {
			break;
		}
	}

	// signal: everything went OK
	return 0;
}
