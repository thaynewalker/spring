/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "AIExport.h"

#include "ExternalAI/Interface/AISEvents.h"
// generated by the C++ Wrapper scripts
#include "OOAICallback.h"
#include "WrappOOAICallback.h"

#include "Incumbent.h"

#include <stdexcept>
#include <map>

static std::map<int, incumbent::Incumbent*> myAIs;
static std::map<int, springai::OOAICallback*>  myAICallbacks;

const static int ERROR_SHIFT = 100;

#define CATCH_CPP_AI_EXCEPTION(RETURN_VAR)					\
	catch (int err) {										\
		RETURN_VAR = err;									\
	} catch (const std::exception& e) {						\
		RETURN_VAR = ERROR_SHIFT + 2;						\
	} catch (const std::string& s) {						\
		RETURN_VAR = ERROR_SHIFT + 3;						\
	} catch (const char* s) {								\
		RETURN_VAR = ERROR_SHIFT + 4;						\
	} catch (...) {											\
		RETURN_VAR = ERROR_SHIFT + 5;						\
	}

EXPORT(int) init(int skirmishAIId, const struct SSkirmishAICallback* innerCallback) {

	int ret = ERROR_SHIFT + 1;

	try {
		springai::OOAICallback* clb = springai::WrappOOAICallback::GetInstance(innerCallback, skirmishAIId);
		incumbent::Incumbent* ai = new incumbent::Incumbent(clb);

		myAIs[skirmishAIId] = ai;
		myAICallbacks[skirmishAIId] = clb;

		ret = 0;
	} CATCH_CPP_AI_EXCEPTION(ret);

	return ret; // (ret != 0) => error
}

EXPORT(int) release(int skirmishAIId) {

	int ret = ERROR_SHIFT + 1;

	try {
		springai::OOAICallback* clb = myAICallbacks[skirmishAIId];
		myAICallbacks.erase(skirmishAIId);

		incumbent::Incumbent* ai = myAIs[skirmishAIId];
		myAIs.erase(skirmishAIId);

		delete ai;
		delete clb;

		ret = 0;
	} CATCH_CPP_AI_EXCEPTION(ret);

	return ret; // (ret != 0) => error
}

EXPORT(int) handleEvent(int skirmishAIId, int topic, const void* data) {

	int ret = ERROR_SHIFT + 1;

	try {
		ret = myAIs[skirmishAIId]->HandleEvent(topic, data);
	} CATCH_CPP_AI_EXCEPTION(ret);

	return ret; // (ret != 0) => error
}
