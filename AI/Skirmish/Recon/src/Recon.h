/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef _recon_recon_H
#define _recon_recon_H

#include "AIBase.h"
#include "AIFloat4.h"

#include <bitset>

namespace recon {

class Recon : public AIBase {
private:
        int port;
        std::string server;
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
	int risk;
	springai::AIFloat4 m;
	//std::unordered_map<int,std::string> id2name;
	std::unordered_map<std::string,std::vector<std::string>> assumptions;

protected:
	virtual int defaultEvent();
	virtual void unitCreatedEvent(SUnitCreatedEvent* evt);
	virtual void enemyEnterRadarEvent(SEnemyEnterRadarEvent* evt);
	virtual void unitDestroyedEvent(SUnitDestroyedEvent* evt);
	virtual void enemyDestroyedEvent(SEnemyDestroyedEvent* evt);
	std::vector<std::vector<bool>> exposure; // Each bit tells whether the entity has been observed at this angle or not
	std::vector<springai::AIFloat4> bbox; // Bounding box of observations
	std::vector<springai::AIFloat4> pbbox; // Initial bounding box of observations
	std::vector<std::string> names;
	std::vector<bool> status;
	std::vector<std::string> fnames;
	std::vector<bool> fstatus;


}; // class Recon

} // namespace recon

#endif // _recon_recon_H

