#include "rlm3-task.h"
#include "rlm3-sim.hpp"
#include "Test.hpp"


static RLM3_Time g_current_time = 0;
static bool g_is_task_active = false;

static const RLM3_Task k_task_id = (RLM3_Task)0xDECADE; // We only have one task, just use a known token


extern RLM3_Time RLM3_GetCurrentTime()
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	return g_current_time;
}

extern RLM3_Time RLM3_GetCurrentTimeFromISR()
{
	ASSERT(SIM_RLM3_Is_IRQ());
	return g_current_time;
}

extern void RLM3_Yield()
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	// Nothing to do since we don't support threads in the simulator.
}

extern void RLM3_Delay(RLM3_Time delay_ms)
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	g_current_time += delay_ms;
}

extern void RLM3_DelayUntil(RLM3_Time start_time, RLM3_Time delay_ms)
{
	ASSERT(g_current_time >= start_time);
	if (g_current_time - start_time < delay_ms)
		RLM3_Delay(delay_ms + start_time - g_current_time);
}

extern RLM3_Task RLM3_GetCurrentTask()
{
	return k_task_id;
}

extern void RLM3_Give(RLM3_Task task)
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	if (task == nullptr)
		return;
	ASSERT(task == k_task_id);
	g_is_task_active = true;
}

extern void RLM3_GiveFromISR(RLM3_Task task)
{
	ASSERT(SIM_RLM3_Is_IRQ());
	if (task == nullptr)
		return;
	ASSERT(task == k_task_id);
	g_is_task_active = true;
}

extern void RLM3_Take()
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	while (!g_is_task_active)
	{
		if (!SIM_HasNextInterrupt())
			FAIL("Test thread sleeping with no more interrupts available to wake it up.");
		g_current_time = SIM_GetNextInterruptTime();
		SIM_RunNextInterrupt();
	}
	g_is_task_active = false;
}

extern bool RLM3_TakeWithTimeout(RLM3_Time timeout_ms)
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	RLM3_Time end_time = g_current_time + timeout_ms;
	while (!g_is_task_active)
	{
		if (!SIM_HasNextInterrupt())
		{
			g_current_time = end_time;
			return false;
		}
		RLM3_Time next_time = SIM_GetNextInterruptTime();
		if (next_time > end_time)
		{
			g_current_time = end_time;
			return false;
		}
		g_current_time = next_time;
		SIM_RunNextInterrupt();
	}
	g_is_task_active = false;
	return true;
}

extern bool RLM3_TakeUntil(RLM3_Time start_time, RLM3_Time delay_ms)
{
	ASSERT(g_current_time >= start_time);
	if (g_current_time - start_time >= delay_ms)
		return false;
	return RLM3_TakeWithTimeout(delay_ms + start_time - g_current_time);
}

TEST_SETUP(TASK_SETUP)
{
	g_current_time = 0;
	g_is_task_active = false;
}
