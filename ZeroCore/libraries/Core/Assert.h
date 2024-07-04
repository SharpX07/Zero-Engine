#pragma once
#include <Core/Logger.h>

//#ifdef ZERO_ENABLE_ASSERTS
#define ZERO_ASSERT(type,condition, message) {if(!condition){ZERO_CORE_LOG_##type(message);exit(-1);}}
//#endif // ZERO_ENAsBLE_ASSERTS
