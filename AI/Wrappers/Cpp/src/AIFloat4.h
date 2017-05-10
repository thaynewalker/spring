/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef _CPPWRAPPER_AI_FLOAT_4_H
#define _CPPWRAPPER_AI_FLOAT_4_H

#include <string>

#include "System/float4.h"

namespace springai {

/**
 * Represents a position on the map.
 */
class AIFloat4 : public float4 {
public:

	AIFloat4();
	AIFloat4(float x, float y, float z, float a);
	AIFloat4(float* xyz);
	AIFloat4(const AIFloat4& other);
	AIFloat4(const float4& f4);

	void LoadInto(float* xyz) const;

	virtual std::string ToString() const;
//	virtual int HashCode() const;
//	virtual bool Equals(const void* obj) const;

	static const AIFloat4 NULL_VALUE;
}; // class AIFloat4

}  // namespace springai

#endif // _CPPWRAPPER_AI_FLOAT_4_H
