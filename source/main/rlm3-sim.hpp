#pragma once

#include "rlm3-base.h"
#include "rlm3-task.h"

#ifndef __cplusplus
#error Sim definitions use C++ features.
#endif

#ifndef TEST
#error Sim definitions only work in test environments.
#endif

#include <functional>


extern bool SIM_RLM3_Is_IRQ();

extern void SIM_AddInterrupt(std::function<void()> interrupt);
extern void SIM_AddDelay(RLM3_Time delay);

extern bool SIM_HasNextInterrupt();
extern RLM3_Time SIM_GetNextInterruptTime();
extern void SIM_RunNextInterrupt();



