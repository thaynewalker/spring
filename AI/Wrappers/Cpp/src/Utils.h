/*
 * Utils.h
 *
 *  Created on: Mar 3, 2017
 *      Author: thayne
 */

#ifndef AI_SKIRMISH_INCUMBENT_SRC_UTILS_H_
#define AI_SKIRMISH_INCUMBENT_SRC_UTILS_H_

inline float distance(float dx, float dy, float dz){
	return sqrt(dx*dy*dz);
}

inline float distance(float x1, float x2, float y1, float y2, float z1, float z2){
	return distance(x1-x2,y1-y2,z1-z2);
}

// Floating point comparisons
static const double TOLERANCE = 0.000001;    // floating point tolerance

template<typename T>
inline bool fless(T a, T b, T TOL=0.000001) { return (a < b - TOL); }
template<typename T>
inline bool fgreater(T a, T b, T TOL=0.000001) { return (a > b + TOL); }
template<typename T>
inline bool fequal(T a, T b, T TOL=0.000001){ return (a >= b-TOL) && (a <= b+TOL); }

template<typename T>
inline bool fleq(T a, T b, T TOL=0.000001) { return fless(a,b, TOL)||fequal(a,b); }
template<typename T>
inline bool fgeq(T a, T b, T TOL=0.000001) { return fgreater(a,b, TOL)||fequal(a,b); }

template<typename T>
inline double min(T a, T b, T TOL=0.000001) { return fless(a, b, TOL)?a:b; }
template<typename T>
inline double max(T a, T b, T TOL=0.000001) { return fless(a, b, TOL)?b:a; }


#endif /* AI_SKIRMISH_INCUMBENT_SRC_UTILS_H_ */
