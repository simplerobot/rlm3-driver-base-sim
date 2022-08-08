#include "rlm3-base.h"
#include "rlm3-sim.hpp"
#include <cstdio>
#include "Test.hpp"
#include <cstring>


static const uint8_t g_default_device_id[12] = { 0x39, 0x00, 0x3d, 0x00, 0x11, 0x51, 0x36, 0x30, 0x34, 0x38, 0x37, 0x30 };
static uint8_t g_device_id[12];


extern bool RLM3_IsIRQ()
{
	return SIM_RLM3_Is_IRQ();
}

extern bool RLM3_IsSchedulerRunning()
{
	return true;
}

extern void RLM3_DebugOutput(uint8_t c)
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	std::putchar(c);
}

extern bool RLM3_DebugOutputFromISR(uint8_t c)
{
	ASSERT(SIM_RLM3_Is_IRQ());
	std::putchar(c);
	return true;
}

extern void RLM3_GetUniqueDeviceId(uint8_t id_out[12])
{
	::memcpy(id_out, g_device_id, sizeof(g_device_id));
}

extern void SIM_SetUniqueDeviceId(const uint8_t id[12])
{
	::memcpy(g_device_id, id, sizeof(g_device_id));
}

TEST_SETUP(SIM_BASE)
{
	::memcpy(g_device_id, g_default_device_id, sizeof(g_device_id));
}
