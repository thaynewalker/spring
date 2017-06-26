/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef _recon_recon_H
#define _recon_recon_H

#include "AIBase.h"
#include "AIFloat4.h"
#include "Cheats.h"

#include <bitset>
#include <chrono>
#include <ctime>
#include <unordered_map>

namespace recon {

class Recon : public AIBase {
private:
        int port;
        int pmodtime;
        std::string server;
        std::chrono::time_point<std::chrono::system_clock> start, end;
        std::unordered_map<std::string,std::vector<std::string> > events;
        std::unordered_map<int,springai::AIFloat3> ploc;
        std::unordered_map<int,float> phead;
        std::unordered_map<int,int> ptimes;
        std::unordered_map<int,int> times;
public:
	Recon(springai::OOAICallback* callback);

	void update(int unitId, springai::AIFloat3 const& pos){
		bbox[eu2i[unitId]].x1=std::min(bbox[eu2i[unitId]].x1,pos.x);
		bbox[eu2i[unitId]].x2=std::max(bbox[eu2i[unitId]].x2,pos.x);
		bbox[eu2i[unitId]].y1=std::min(bbox[eu2i[unitId]].y1,pos.z);
		bbox[eu2i[unitId]].y2=std::max(bbox[eu2i[unitId]].y2,pos.z);
		if(pos.x<pbbox[eu2i[unitId]].x1 ||
				pos.x>pbbox[eu2i[unitId]].x2 ||
				pos.z<pbbox[eu2i[unitId]].y1 ||
				pos.z>pbbox[eu2i[unitId]].y2){
			// All new position discovered
			score +=3;
			// Reset exposure matrix
			exposure[eu2i[unitId]].resize(0);
			exposure[eu2i[unitId]].resize(180);
		}
	}
	void expose(int unitId,int val){exposure[eu2i[unitId]][val/2]=true;}
	bool getExposure(int unitId, int val){return exposure[eu2i[unitId]][val/2];}
	int aggressiveness;
	int recordEvents;
	int risk;
	//springai::AIFloat4 m;
	std::unordered_map<std::string,std::vector<std::string>> assumptions;

protected:
	virtual void setUpStructures();
	virtual int defaultEvent(int topic, const void* data);
	virtual void unitCreatedEvent(SUnitCreatedEvent* evt);
	virtual void enemyEnterRadarEvent(SEnemyEnterRadarEvent* evt);
	virtual void unitDestroyedEvent(SUnitDestroyedEvent* evt);
	virtual void enemyDestroyedEvent(SEnemyDestroyedEvent* evt);
	virtual void projectileMovedEvent(SProjectileMovedEvent* evt);
	virtual void radarChangedEvent(SRadarChangedEvent* evt);
	std::vector<std::vector<bool>> exposure; // Each bit tells whether the entity has been observed at this angle or not
	std::vector<springai::AIFloat4> bbox; // Bounding box of observations
	std::vector<springai::AIFloat4> pbbox; // Initial bounding box of observations
	//std::vector<std::string> names;


}; // class Recon

} // namespace recon

#endif // _recon_recon_H

