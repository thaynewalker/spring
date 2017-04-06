/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef _recon_recon_H
#define _recon_recon_H

#include "AIBase.h"

namespace recon {

class Recon : public AIBase {
private:
public:
	Recon(springai::OOAICallback* callback);
protected:
	virtual void defaultEvent();
	virtual void enemyLeaveRadarEvent(SEnemyLeaveRadarEvent* evt);
	virtual void enemyEnterRadarEvent(SEnemyEnterRadarEvent* evt);
	virtual void weaponFiredEvent(SWeaponFiredEvent* evt);
	virtual void commandFinishedEvent(SCommandFinishedEvent* evt);
	virtual void enemyLeaveLOSEvent(SEnemyLeaveLOSEvent* evt){}
	virtual void enemyEnterLOSEvent(SEnemyEnterLOSEvent* evt){}
	virtual void enemyDestroyedEvent(SEnemyDestroyedEvent* evt);
	virtual void enemyDamagedEvent(SEnemyDamagedEvent* evt);
	virtual void unitDamagedEvent(SUnitDamagedEvent* evt);
	virtual void unitDestroyedEvent(SUnitDestroyedEvent* evt);

}; // class Recon

} // namespace recon

#endif // _recon_recon_H

