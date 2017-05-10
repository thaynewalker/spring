/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "AIFloat4.h"

#include <string>

springai::AIFloat4::AIFloat4()
	: float4(0.0f, 0.0f, 0.0f, 0.0f)
{
}
springai::AIFloat4::AIFloat4(float x, float y, float z, float a)
	: float4(x, y, z, a)
{
}
springai::AIFloat4::AIFloat4(float* xyz)
	: float4(xyz)
{
}
springai::AIFloat4::AIFloat4(const springai::AIFloat4& other)
	: float4(other)
{
}
springai::AIFloat4::AIFloat4(const float4& f4)
	: float4(f4)
{
}



void springai::AIFloat4::LoadInto(float* xyz) const {
	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;
}

std::string springai::AIFloat4::ToString() const {

	char resBuff[128];
	SNPRINTF(resBuff, sizeof(resBuff), "(%f, %f, %f, %f)", x, y, z, a);
	return std::string(resBuff);
}


/*int springai::AIFloat4::HashCode() const {
	static const int prime = 41;

	int result = 0;

	result = prime * result + ToIntBits(x);
	result = prime * result + ToIntBits(y);
	result = prime * result + ToIntBits(z);

	return result;
}*/

/*bool springai::AIFloat4::Equals(const void* obj) const {
	if (this == obj) {
		return true;
	} else if (obj == NULL) {
		return false;
	//} else if (!super.equals(obj)) {
	//	return false;
	//} else if (sizeof(*this) != sizeof(*obj)) {
	//	return false;
	}

	const AIFloat4& other = *((AIFloat4*)obj);
	if (ToIntBits(x) != ToIntBits(other.x)) {
		return false;
	} else if (ToIntBits(y) != ToIntBits(other.y)) {
		return false;
	} else if (ToIntBits(z) != ToIntBits(other.z)) {
		return false;
	} else {
		return true;
	}
}*/

const springai::AIFloat4 springai::AIFloat4::NULL_VALUE;

