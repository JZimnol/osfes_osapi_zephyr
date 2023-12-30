#include "osapi.h"

namespace osapi
{

unsigned int getSystemTime()
{
	return k_uptime_get_32();
}

} // namespace osapi
