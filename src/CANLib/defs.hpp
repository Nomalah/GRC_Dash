#pragma once

#if __linux__
#include <linux/can.h>
#else

#endif

enum class CanFormat : uint8_t { Standard, Extended };
enum class RetCode { Success, ReadErr, OpenErr, Timeout, SocketErr, WriteErr };
