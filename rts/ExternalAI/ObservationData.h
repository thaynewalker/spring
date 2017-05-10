#ifndef OBSDATA_H
#define OBSDATA_H

#include <string>
#include <vector>
#include <sstream>

#include "System/creg/creg_cond.h"

// Represents the observation state of an entity
struct locData{
        CR_DECLARE_STRUCT(locData)
        std::string id;
        float minX;
        float maxX;
        float minZ;
        float maxZ;
        std::vector<bool> exposure;
        locData():minX(9999999),maxX(-9999999),minZ(9999999),maxZ(-9999999),exposure(360){}
        locData(std::string const& _id, float mnx,float mxx,float mnz, float mxz,std::vector<bool> const& exp):id(_id),minX(mnx),maxX(mxx),minZ(mnz),maxZ(mxz),exposure(exp){}
        std::string stringrep;
        std::string const& stringRep(){
          std::stringstream ss;
          ss << id << "_" << minX << "_" << maxX << "_" << maxZ << "_" << minZ << "_";
          for(auto const& a:exposure){
            ss << (a?"1":"0");
          }
          stringrep=ss.str();
          return stringrep;
        }
};      

#endif
