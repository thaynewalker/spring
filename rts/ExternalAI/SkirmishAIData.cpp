/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "ExternalAI/SkirmishAIData.h"

#include "System/creg/STL_Map.h"
#include "System/Platform/byteorder.h" // for swabDWord

CR_BIND_DERIVED(SkirmishAIBase, TeamController, )
CR_REG_METADATA(SkirmishAIBase, (
	CR_MEMBER(hostPlayer),
	CR_MEMBER(status)
))


CR_BIND(locData, )
CR_REG_METADATA(locData, (
	CR_MEMBER(id),
	CR_MEMBER(minX),
	CR_MEMBER(maxX),
	CR_MEMBER(minZ),
	CR_MEMBER(maxZ),
	CR_MEMBER(exposure),
	CR_MEMBER(stringrep)
))

CR_BIND(SkirmishAIStatistics, )
CR_REG_METADATA(SkirmishAIStatistics, (
	CR_MEMBER(numCommands),
	CR_MEMBER(unitCommands),
	CR_MEMBER(cpuTime)
))


CR_BIND_DERIVED(SkirmishAIData, SkirmishAIBase, )
CR_REG_METADATA(SkirmishAIData, (
	CR_MEMBER(shortName),
	CR_MEMBER(version),
	CR_MEMBER(optionKeys),
	CR_MEMBER(options),
	CR_MEMBER(isLuaAI),
	CR_MEMBER(currentStats),
	CR_MEMBER(score),
	CR_MEMBER(observations)
))


void SkirmishAIStatistics::swab() {
	swabDWordInPlace(cpuTime);
	TeamControllerStatistics::swab();
}
