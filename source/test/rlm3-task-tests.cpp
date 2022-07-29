#include "Test.hpp"
#include "rlm3-task.h"
#include "rlm3-sim.hpp"


TEST_CASE(RLM3_Delay_HappyCase)
{
	ASSERT(RLM3_GetCurrentTime() == 0);
	RLM3_Delay(10);
	ASSERT(RLM3_GetCurrentTime() == 10);
}

TEST_CASE(RLM3_DelayUntil_HappyCase)
{
	ASSERT(RLM3_GetCurrentTime() == 0);
	RLM3_DelayUntil(0, 5);
	RLM3_DelayUntil(5, 5);
	ASSERT(RLM3_GetCurrentTime() == 10);
}

TEST_CASE(RLM3_Take_HappyCase)
{
	RLM3_Give(RLM3_GetCurrentTask());

	RLM3_Take();
}

TEST_CASE(RLM3_Take_Delayed)
{
	RLM3_Task current_task = RLM3_GetCurrentTask();

	SIM_AddDelay(5);
	SIM_AddInterrupt([&]() { });
	SIM_AddDelay(15);
	SIM_AddInterrupt([&]() { RLM3_GiveFromISR(current_task); });

	RLM3_Take();
	ASSERT(RLM3_GetCurrentTime() == 20);
}

TEST_CASE(RLM3_TakeWithTimeout_HappyCase)
{
	RLM3_Give(RLM3_GetCurrentTask());

	ASSERT(RLM3_TakeWithTimeout(10));

	ASSERT(RLM3_GetCurrentTime() == 0);
}

TEST_CASE(RLM3_TakeWithTimeout_Timeout)
{
	ASSERT(!RLM3_TakeWithTimeout(10));

	ASSERT(RLM3_GetCurrentTime() == 10);
}

TEST_CASE(RLM3_TakeWithTimeout_Delayed)
{
	RLM3_Task current_task = RLM3_GetCurrentTask();

	SIM_AddDelay(5);
	SIM_AddInterrupt([&]() { });
	SIM_AddDelay(15);
	SIM_AddInterrupt([&]() { RLM3_GiveFromISR(current_task); });

	RLM3_TakeWithTimeout(30);
	ASSERT(RLM3_GetCurrentTime() == 20);
}

TEST_CASE(SIM_Give_HappyCase)
{
	SIM_Give();

	RLM3_Take();
}

TEST_CASE(SIM_Give_Delayed)
{
	SIM_AddDelay(5);
	SIM_AddInterrupt([&]() { });
	SIM_AddDelay(15);
	SIM_AddInterrupt([&]() { SIM_Give(); });

	RLM3_Take();
	ASSERT(RLM3_GetCurrentTime() == 20);
}

