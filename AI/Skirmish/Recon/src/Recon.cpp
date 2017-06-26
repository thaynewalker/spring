/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "Recon.h"


#include "ExternalAI/Interface/AISEvents.h"
#include "ExternalAI/Interface/AISCommands.h"

// generated by the C++ Wrapper scripts
#include "OOAICallback.h"
#include "Unit.h"
#include "Team.h"
#include "UnitDef.h"
#include "AIUtils.h"
#include "Game.h"
#include "Resource.h"
#include "SkirmishAIs.h"

#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <iostream>

recon::Recon::Recon(springai::OOAICallback* callback):AIBase(callback){
  callback->GetCheats()->SetEnabled(true);
  aggressiveness=GetIntOption("aggressiveness");
  recordEvents=GetIntOption("recordevents");
  //std::cout << "recordEvents:"<<recordEvents<<"\n";
  risk=GetIntOption("risk");
  port=GetIntOption("port");
  server=GetStringOption("server");
  start = std::chrono::system_clock::now();
  pmodtime=-1;

  std::vector<std::string> assump(split(callback->GetAssumptions(callback->GetGame()->GetMyTeam()),','));
  for(auto const& a:assump){
    std::vector<std::string> rec(split(a,'_'));
    assumptions[rec[0]]=std::vector<std::string>(6);
    assumptions[rec[0]][0]=rec[1]; // minX
    assumptions[rec[0]][1]=rec[2]; // maxX
    assumptions[rec[0]][2]=rec[3]; // minZ
    assumptions[rec[0]][3]=rec[4]; // maxZ
    assumptions[rec[0]][4]=rec[5]; // dead or alive
    assumptions[rec[0]][5]=rec[6]; // exposure
  }

}

void
recon::Recon::enemyEnterRadarEvent(SEnemyEnterRadarEvent* evt){
  AIBase::enemyEnterRadarEvent(evt);

  std::string name(names[evt->enemy]);
  springai::Unit* u(GetEnemyUnitById(evt->enemy));
  springai::AIFloat3 pos(u->GetPos());
  auto assump(assumptions.find(name));
  if(assump != assumptions.end()){
    int ix(eu2i[evt->enemy]);
    callback->GetSkirmishAIs()->SetObservation(ix,atof(assump->second[0].c_str()),atof(assump->second[1].c_str()),atof(assump->second[2].c_str()),atof(assump->second[3].c_str()),assump->second[4].c_str(),name.c_str());
    pbbox[ix].x1=atof(assump->second[0].c_str());
    pbbox[ix].x2=atof(assump->second[1].c_str());
    pbbox[ix].y1=atof(assump->second[2].c_str());
    pbbox[ix].y2=atof(assump->second[3].c_str());
    if(pos.x<pbbox[ix].x1 ||
        pos.x>pbbox[ix].x2 ||
        pos.z<pbbox[ix].y1 ||
        pos.z>pbbox[ix].y2){
      // This is a new position for this entity
    }else{
      int i(0);
      for(auto const& a:assump->second[5]){
        exposure[ix][i++]=a=='1';
      }
    }
  }else{
    //Unknown
  }
  update(u->GetUnitId(),pos);
}

void
recon::Recon::unitCreatedEvent(SUnitCreatedEvent* evt){
  static bool inited(false);
  if(!inited){
    setUpStructures();
    inited=true;
  }
  AIBase::AddUnit(evt->unit);
  std::string name(callback->GetUnitName(callback->GetGame()->GetMyTeam(),evt->unit,""));
  //id2name.insert(std::make_pair(evt->enemy,name));
  //names.push_back(name);
  if(aggressiveness>0){
    GetFriendlyUnitById(evt->unit)->SetFireState(utils::FIRESTATE_FIREATWILL);
  }
  if(risk==0){
    // Tell unit to go back home if fired at (TODO: make an accessor for incoming missile count.)
  }
}

void
recon::Recon::unitDestroyedEvent(SUnitDestroyedEvent* evt){
  AIBase::unitDestroyedEvent(evt);
  status[u2i[evt->unit]]=false;
  std::stringstream ss;
  ss << "DEATH_"<<int(round(evt->timeOffset));
  std::string id(names[evt->unit]);
  events[id].push_back(ss.str());
}

void
recon::Recon::enemyDestroyedEvent(SEnemyDestroyedEvent* evt){
  AIBase::enemyDestroyedEvent(evt);
  status[eu2i[evt->enemy]]=false;
  std::stringstream ss;
  ss << "DEATH_"<<int(round(evt->timeOffset));
  std::string id(names[evt->enemy]);
  events[id].push_back(ss.str());
}

void
recon::Recon::projectileMovedEvent(SProjectileMovedEvent* evt){
  if(!recordEvents)return;
  int inTime(round(evt->timeOffset));
  if(inTime > ptimes[evt->id]){
    std::stringstream ss;
    ptimes[evt->id]=inTime;
    ss << "M-" << evt->id;
    std::string id(ss.str());
    ss.str(std::string());
    ss << evt->x<<"_"<<evt->y<<"_"<<evt->z<<"_"<<inTime;
    //std::cout << id <<"="<<ss.str() << "\n";
    events[id].push_back(ss.str());
  }
}

void
recon::Recon::radarChangedEvent(SRadarChangedEvent* evt){
  if(!recordEvents)return;
  std::string id(names[evt->unitId]);

  //std::cout << "radar change for unit \"" << id << "\"-->"<< utils::MODES[evt->state] << "\n";
  std::stringstream ss;
  ss << utils::MODES[evt->state]<<"_"<<int(round(evt->timeOfChange));
  events[id].push_back(ss.str());
}

void
recon::Recon::setUpStructures(){
  AIBase::setUpStructures();
  exposure.resize(numEnemies);
  for(auto& ex:exposure){
    ex.resize(180);
  }
  bbox.resize(numEnemies);
  pbbox.resize(numEnemies);
  for(auto enemy:eu2i){
    bbox[enemy.second]=springai::AIFloat4(9999999.,9999999.,-9999999.,-9999999.);
    pbbox[enemy.second]=springai::AIFloat4(-9999999.,-9999999.,9999999.,9999999.);
  }

  int team(callback->GetGame()->GetMyTeam());

  for(auto const& p:units){
    springai::Unit* u(p.second);
    std::string name(callback->GetUnitName(team,p.first,""));
    springai::AIFloat3 pos(u->GetPos());
    auto assump(assumptions.find(name));
    if(assump != assumptions.end()){
      int i(0);
      for(auto const& a:assump->second[5]){
        exposure[numEnemies-1][i++]=a=='1';
      }
    }else{
      //Unknown
    }
  }
}

int
recon::Recon::defaultEvent(int topic, const void* data){
  static std::chrono::seconds timeout(300);
  AIBase::defaultEvent(topic,data);
  int modTime(0);
  if(topic == EVENT_UPDATE){
    SUpdateEvent* evt(0);
    evt=(SUpdateEvent*) data;
    modTime=round(evt->currModTime)-4; // Adjust for initial 4-second countdown...

    if(modTime>pmodtime){ // Update once per second
      pmodtime=modTime;
      end = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end-start;
      if(frame%100000==0)std::cout << "elapsed time: " << elapsed_seconds.count() << "/" << timeout.count() << "\n";
      //friends=callback->GetFriendlyUnits();
      //std::cout << "Recon score: " << score << "\n";
      //std::vector<springai::Unit*> const& enemies(callback->GetEnemyUnits());
      //friends=callback->GetFriendlyUnits();
      //std::chrono::time_point<std::chrono::system_clock> start, end;
      //start = std::chrono::system_clock::now();

      std::vector<springai::AIFloat3> fpos(u2i.size());
      std::vector<double> fsens(u2i.size());
      int num(0);

      for(auto ff:u2i){
        if(!status[ff.first]){continue;} // Dead unit
        springai::Unit* f(units[ff.first]);
        int uid(f->GetUnitId());
        springai::AIFloat3 fPos(f->GetPos());
        if(recordEvents && !done[ff.second] && f->GetSpeed()>0.0 && modTime > times[uid] && names.size()){ // If moving and time of update is significant
          // Ignore position updates if the heading is unchanged
          float hdg(ploc[uid].HeadingTo(fPos));
          if(!fequal(hdg,phead[uid],.5)){
            phead[uid]=hdg;
            ploc[uid]=fPos;
            times[uid]=modTime;
            std::stringstream ss;
            std::string id(names[uid]);
            ss << fPos.x<<"_"<<fPos.y<<"_"<<fPos.z<<"_"<<modTime;
            //std::cout << id <<"="<<ss.str() << "\n";
            events[id].push_back(ss.str());
          }
        }
        ++num;
        springai::UnitDef* fDef(f->GetDef());
        if(!fDef){
          f->GetDef();
        }
        if(fDef->HasRadarSensor()){
          fpos.push_back(fPos);
          fsens.push_back(fDef->GetRadarSensitivity());
        }
      }


      for(auto ee:eu2i){
        if(!status[ee.first]){continue;} // Dead unit
        springai::Unit* e(units[ee.first]);
        if(e->GetRadarState()){ // We assume a radius of 2R when enemy radar is on
          int uid(e->GetUnitId());
          // Apparently these calls are expensive. Make them once here.
          springai::UnitDef* eDef(e->GetDef());
          springai::AIFloat3 ePos(e->GetPos());
          if(recordEvents && !done[ee.second] && e->GetSpeed()>0.0 && modTime > times[uid]){ // If moving and time of update is significant
            float hdg(ploc[uid].HeadingTo(ePos));
            if(!fequal(hdg,phead[uid],.5)){
              phead[uid]=hdg;
              ploc[uid]=ePos;
              times[uid]=modTime;
              std::stringstream ss;
              std::string id(names[uid]);
              ss << ePos.x<<"_"<<ePos.y<<"_"<<ePos.z<<"_"<<modTime;
              //std::cout << id <<"="<<ss.str() << "\n";
              events[id].push_back(ss.str());
            }
          }
          update(uid,ePos);
          for(int i(0); i<num; ++i){
            if(fpos[i].distance(ePos) < eDef->GetRadarRadius()*2*eDef->GetRadarObservability()*fsens[i]){
              int hdg(ePos.HeadingTo(fpos[i]));
              if(!getExposure(uid,hdg)){ // Observed at a new angle
                //std::cout << "Enemy " << *e << " seen at " << hdg << " by " << *f << "\n";
                expose(uid,hdg);
                //callback->GetSkirmishAIs()->AddObservation(numEnemies-1,ePos.x,ePos.z,hdg,id2name[uid].c_str());
                score++;
              }
            }else{
              //std::cout << "Enemy " << *e << " out of range ("<< (e->GetDef()->GetRadarRadius()*e->GetDef()->GetRadarRadius()) <<") of " << *f << "\n";
            }
          }
        }else{
          //std::cout << *e << " radar off \n";
        }
      }
      /*for(auto e:enemies){
			if(e->GetRadarState()){ // We assume a radius of 2R when enemy radar is on
				// Apparently these calls are expensive. Make them once here.
				springai::UnitDef* eDef(e->GetDef());
				springai::AIFloat3 ePos(e->GetPos());
				update(e->GetUnitId(),ePos);
				for(auto f:friends){
					springai::UnitDef* fDef(f->GetDef());
					springai::AIFloat3 fPos(f->GetPos());
					if(fDef->HasRadarSensor() && fPos.distance(ePos) < eDef->GetRadarRadius()*2*eDef->GetRadarObservability()*fDef->GetRadarSensitivity()){
						int hdg(ePos.HeadingTo(fPos));
						if(!getExposure(e->GetUnitId(),hdg)){ // Observed at a new angle
							//std::cout << "Enemy " << *e << " seen at " << hdg << " by " << *f << "\n";
							expose(e->GetUnitId(),hdg);
							//callback->GetSkirmishAIs()->AddObservation(numEnemies-1,ePos.x,ePos.z,hdg,id2name[e->GetUnitId()].c_str());
							score++;
						}
					}else{
						//std::cout << "Enemy " << *e << " out of range ("<< (e->GetDef()->GetRadarRadius()*e->GetDef()->GetRadarRadius()) <<") of " << *f << "\n";
					}
				}
			}else{
				//std::cout << *e << " radar off \n";
			}
		}*/


      if(alldone && !statusSent){

        struct sockaddr_in myaddr;      /* our address */
        struct sockaddr_in remaddr;     /* remote address */
        socklen_t slen(sizeof(myaddr));            /* length of addresses */
        int recvlen;                    /* # bytes received */
        int fd;                         /* our socket */
        int msgcnt = 0;                 /* count # of messages we received */


        // Send final scores to the caller
        /* create a TCP socket */

        while ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
          std::cout << "cannot create socket... trying again in 1\n";
          sleep(1);
        }

        hostent* svr;
        if(!(svr = gethostbyname(server.c_str()))){
          perror("no such host\n");
        }

        memset((char *)&remaddr, 0, sizeof(remaddr));
        remaddr.sin_family = AF_INET;
        bcopy((char *)svr->h_addr, (char *)&remaddr.sin_addr.s_addr, svr->h_length);
        remaddr.sin_port = htons(port);

        while(connect(fd, (struct sockaddr *)&remaddr, sizeof(remaddr)) < 0) {
          std::cout << "connect failed... trying again in 1\n";
          sleep(1);
        }

        /* now loop, receiving data and printing what we received */
        std::cout << "Sending scores to " << server <<":"<<port<<" @ " << std::time(0) << "\n";
        std::stringstream ss;
        ss<<"Incumbent:"<<callback->GetSkirmishAIs()->GetScore(callback->GetSkirmishAIId()==0?1:0)<<",Recon:"<<score;
        ss<<",Assumptions:";
        std::string sep("");
        for(auto const& e:eu2i){
          //ss<<callback->GetSkirmishAIs()->GetObservationAsString(i)<<";";
          ss<<sep<<names[e.first]<<"_";
          ss<<bbox[e.second].x1<<"_"<<bbox[e.second].x2<<"_"<<bbox[e.second].y1<<"_"<<bbox[e.second].y2<<"_";
          for(auto const& a: exposure[e.second]){
            ss<<a?"1":"0";
          }
          sep="+";
        }
        ss << ",Events:";
        for(auto const& evt:events){
          ss << evt.first<<"=";
          sep = "";
          for(auto const& s:evt.second){
            ss << sep<< s;
            sep="+";
          }
          ss << ";";
        }
        //std::cout<<ss.str().c_str() << "\n";
        if(write(fd, ss.str().c_str() , strlen(ss.str().c_str()))<0) {
          perror("error sending results");
          close(fd);
          return 0;
        }
        close(fd);
        statusSent=true;

        callback->GetGame()->SendTextMessage("/ReloadForce",0);
      }
      if(statusSent){
        callback->GetGame()->SendTextMessage("/ReloadForce",0);
      }
      if(!alldone && (elapsed_seconds.count()>timeout.count() || done.size() && allDone())){
        alldone=true;

        //for(auto i:eu2i){
        //std::cout << i.first << " exposure " << exposure[i.second].count() << "\n";
        //}

        //callback->GetGame()->SendTextMessage("/AIKill 0",0);
        //callback->GetGame()->SendTextMessage("/AIKill 1",0);
        return score;
      }
    }
  }
	return 0;
}