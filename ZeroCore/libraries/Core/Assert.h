#pragma once
#include <Core/Logger.h>

//#ifdef ZERO_ENABLE_ASSERTS
#define ZERO_ASSERT(condition, message) { \
    if (!(condition)) { \
        ZERO_CORE_LOG_CRITICAL("[{}:{}] " message, __FILE__, __LINE__); \
        exit(-1); \
    } \
}

//#endif // ZERO_ENAsBLE_ASSERTS
