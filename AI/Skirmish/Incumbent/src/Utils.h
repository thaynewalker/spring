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

inline bool fless(double a, double b) { return (a < b - TOLERANCE); }
inline bool fgreater(double a, double b) { return (a > b + TOLERANCE); }
inline bool fequal(double a, double b)
{ return (a >= b - TOLERANCE) && (a <= b+TOLERANCE); }
inline bool fleq(double a, double b) { return fless(a,b)||fequal(a,b); }
inline bool fgeq(double a, double b) { return fgreater(a,b)||fequal(a,b); }

inline double min(double a, double b) { return fless(a, b)?a:b; }
inline double max(double a, double b) { return fless(a, b)?b:a; }


#endif /* AI_SKIRMISH_INCUMBENT_SRC_UTILS_H_ */
