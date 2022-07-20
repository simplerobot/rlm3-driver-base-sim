#include "rlm3-base.h"
#include "rlm3-sim.hpp"
#include <cstdio>
#include "Assert.h"


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
