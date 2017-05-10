/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef SKIRMISH_AI_DATA_H
#define SKIRMISH_AI_DATA_H

#include <string>
#include <vector>

#include "ExternalAI/SkirmishAIBase.h"

#include "System/creg/creg_cond.h"
#include "System/UnorderedMap.hpp"

#include "ObservationData.h"

// Represents the observation state of an entity
/*class locData{
	CR_DECLARE_STRUCT(locData)

public:
        std::string id;
        float minX;
        float maxX;
        float minZ;
        float maxZ;
        std::vector<bool> exposure;
        locData():minX(9999999),maxX(-9999999),minZ(9999999),maxZ(-9999999){}
        locData(std::string const& _id, float mnx,float mxx,float mnz, float mxz,std::vector<bool> const& exp):id(_id),minX(mnx),maxX(mxx),minZ(mnz),maxZ(mxz),exposure(exp){}
        std::string stringrep;
        std::string const& stringRep(){
          std::stringstream ss;
          ss << id << "_" << minX << "_" << maxX << "_" << maxZ << "_" << minZ << "_";
          for(auto const& a:exposure){
            ss << (a?"1":"0");
          stringrep=ss.str();
          return stringrep;
        }
        void swab(){}
};*/

/**
 * Contains everything needed to initialize a Skirmish AI instance.
 * @see Game/GameSetup
 */
class SkirmishAIData : public SkirmishAIBase {
	CR_DECLARE(SkirmishAIData)

public:
	std::string shortName;
	std::string version;
	std::vector<std::string> optionKeys;
	spring::unordered_map<std::string, std::string> options;

	bool isLuaAI;

	SkirmishAIStatistics currentStats;
        int score;
        std::vector<locData> observations;
};

#endif // SKIRMISH_AI_DATA_H
