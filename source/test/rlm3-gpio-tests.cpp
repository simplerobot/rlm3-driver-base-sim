#include "Test.hpp"
#include "rlm3-gpio.h"


TEST_CASE(GPIO_ClockEnable_HappyCase)
{
	ASSERT(!SIM_GPIO_IsClockEnabled(GPIOB));
	__HAL_RCC_GPIOB_CLK_ENABLE();
	ASSERT(SIM_GPIO_IsClockEnabled(GPIOB));
	__HAL_RCC_GPIOB_CLK_DISABLE();
	ASSERT(!SIM_GPIO_IsClockEnabled(GPIOB));
}

TEST_CASE(GPIO_ClockEnable_AlreadyEnabled)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	ASSERT_ASSERTS(__HAL_RCC_GPIOB_CLK_ENABLE());
}

TEST_CASE(GPIO_ClockEnable_AlreadyDisabled)
{
	ASSERT_ASSERTS(__HAL_RCC_GPIOB_CLK_DISABLE());
}

TEST_CASE(HAL_GPIO_Init_HappyCase)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef init = {};
	init.Pin = GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_14;
	init.Mode = GPIO_MODE_OUTPUT_PP;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOB, &init);

	ASSERT(SIM_GPIO_IsEnabled(GPIOB, GPIO_PIN_0));
	ASSERT(SIM_GPIO_IsEnabled(GPIOB, GPIO_PIN_6));
	ASSERT(SIM_GPIO_IsEnabled(GPIOB, GPIO_PIN_14));
	ASSERT(!SIM_GPIO_IsEnabled(GPIOB, GPIO_PIN_1));
	ASSERT(!SIM_GPIO_IsEnabled(GPIOB, GPIO_PIN_8));
	ASSERT(!SIM_GPIO_IsEnabled(GPIOB, GPIO_PIN_15));
	ASSERT(SIM_GPIO_GetMode(GPIOB, GPIO_PIN_0) == GPIO_MODE_OUTPUT_PP);
	ASSERT(SIM_GPIO_GetMode(GPIOB, GPIO_PIN_6) == GPIO_MODE_OUTPUT_PP);
	ASSERT(SIM_GPIO_GetMode(GPIOB, GPIO_PIN_14) == GPIO_MODE_OUTPUT_PP);
	ASSERT(SIM_GPIO_GetMode(GPIOB, GPIO_PIN_1) == GPIO_MODE_DISABLED);
	ASSERT(SIM_GPIO_GetMode(GPIOB, GPIO_PIN_8) == GPIO_MODE_DISABLED);
	ASSERT(SIM_GPIO_GetMode(GPIOB, GPIO_PIN_15) == GPIO_MODE_DISABLED);
	ASSERT(SIM_GPIO_GetPull(GPIOB, GPIO_PIN_0) == GPIO_NOPULL);
	ASSERT(SIM_GPIO_GetPull(GPIOB, GPIO_PIN_6) == GPIO_NOPULL);
	ASSERT(SIM_GPIO_GetPull(GPIOB, GPIO_PIN_14) == GPIO_NOPULL);
	ASSERT(SIM_GPIO_GetPull(GPIOB, GPIO_PIN_1) == GPIO_PULL_DISABLED);
	ASSERT(SIM_GPIO_GetPull(GPIOB, GPIO_PIN_8) == GPIO_PULL_DISABLED);
	ASSERT(SIM_GPIO_GetPull(GPIOB, GPIO_PIN_15) == GPIO_PULL_DISABLED);
	ASSERT(SIM_GPIO_GetSpeed(GPIOB, GPIO_PIN_0) == GPIO_SPEED_FREQ_HIGH);
	ASSERT(SIM_GPIO_GetSpeed(GPIOB, GPIO_PIN_6) == GPIO_SPEED_FREQ_HIGH);
	ASSERT(SIM_GPIO_GetSpeed(GPIOB, GPIO_PIN_14) == GPIO_SPEED_FREQ_HIGH);
	ASSERT(SIM_GPIO_GetSpeed(GPIOB, GPIO_PIN_1) == GPIO_SPEED_DISABLED);
	ASSERT(SIM_GPIO_GetSpeed(GPIOB, GPIO_PIN_8) == GPIO_SPEED_DISABLED);
	ASSERT(SIM_GPIO_GetSpeed(GPIOB, GPIO_PIN_15) == GPIO_SPEED_DISABLED);
	ASSERT(SIM_GPIO_GetAlt(GPIOB, GPIO_PIN_0) == GPIO_AF_DISABLED);
	ASSERT(SIM_GPIO_GetAlt(GPIOB, GPIO_PIN_6) == GPIO_AF_DISABLED);
	ASSERT(SIM_GPIO_GetAlt(GPIOB, GPIO_PIN_14) == GPIO_AF_DISABLED);
	ASSERT(SIM_GPIO_GetAlt(GPIOB, GPIO_PIN_1) == GPIO_AF_DISABLED);
	ASSERT(SIM_GPIO_GetAlt(GPIOB, GPIO_PIN_8) == GPIO_AF_DISABLED);
	ASSERT(SIM_GPIO_GetAlt(GPIOB, GPIO_PIN_15) == GPIO_AF_DISABLED);
}

TEST_CASE(HAL_GPIO_Init_ClockOff)
{
	GPIO_InitTypeDef init = {};
	init.Pin = GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_14;
	init.Mode = GPIO_MODE_OUTPUT_PP;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_FREQ_HIGH;

	ASSERT_ASSERTS(HAL_GPIO_Init(GPIOB, &init));
}

TEST_CASE(HAL_GPIO_Init_NULL)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	ASSERT_ASSERTS(HAL_GPIO_Init(GPIOB, nullptr));
}

TEST_CASE(HAL_GPIO_Init_Twice)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef init = {};
	init.Pin = GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_14;
	init.Mode = GPIO_MODE_OUTPUT_PP;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOB, &init);
	ASSERT_ASSERTS(HAL_GPIO_Init(GPIOB, &init));
}

TEST_CASE(HAL_GPIO_DeInit_HappyCase)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef init = {};
	init.Pin = GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_14;
	init.Mode = GPIO_MODE_OUTPUT_PP;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &init);

	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_14);

	ASSERT(!SIM_GPIO_IsEnabled(GPIOB, GPIO_PIN_0));
	ASSERT(SIM_GPIO_IsEnabled(GPIOB, GPIO_PIN_6));
	ASSERT(!SIM_GPIO_IsEnabled(GPIOB, GPIO_PIN_14));
	ASSERT(SIM_GPIO_GetMode(GPIOB, GPIO_PIN_0) == GPIO_MODE_DISABLED);
	ASSERT(SIM_GPIO_GetMode(GPIOB, GPIO_PIN_6) == GPIO_MODE_OUTPUT_PP);
	ASSERT(SIM_GPIO_GetMode(GPIOB, GPIO_PIN_14) == GPIO_MODE_DISABLED);
	ASSERT(SIM_GPIO_GetPull(GPIOB, GPIO_PIN_0) == GPIO_PULL_DISABLED);
	ASSERT(SIM_GPIO_GetPull(GPIOB, GPIO_PIN_6) == GPIO_NOPULL);
	ASSERT(SIM_GPIO_GetPull(GPIOB, GPIO_PIN_14) == GPIO_PULL_DISABLED);
	ASSERT(SIM_GPIO_GetSpeed(GPIOB, GPIO_PIN_0) == GPIO_SPEED_DISABLED);
	ASSERT(SIM_GPIO_GetSpeed(GPIOB, GPIO_PIN_6) == GPIO_SPEED_FREQ_HIGH);
	ASSERT(SIM_GPIO_GetSpeed(GPIOB, GPIO_PIN_14) == GPIO_SPEED_DISABLED);
	ASSERT(SIM_GPIO_GetAlt(GPIOB, GPIO_PIN_0) == GPIO_AF_DISABLED);
	ASSERT(SIM_GPIO_GetAlt(GPIOB, GPIO_PIN_6) == GPIO_AF_DISABLED);
	ASSERT(SIM_GPIO_GetAlt(GPIOB, GPIO_PIN_14) == GPIO_AF_DISABLED);
}

TEST_CASE(HAL_GPIO_DeInit_NoClock)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef init = {};
	init.Pin = GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_14;
	init.Mode = GPIO_MODE_OUTPUT_PP;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &init);
	__HAL_RCC_GPIOB_CLK_DISABLE();

	ASSERT_ASSERTS(HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_14));
}

TEST_CASE(HAL_GPIO_DeInit_DisabledPin)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	ASSERT_ASSERTS(HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_14));
}

TEST_CASE(HAL_GPIO_WritePin_HappyCase)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef init = {};
	init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_14;
	init.Mode = GPIO_MODE_OUTPUT_PP;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &init);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_14, GPIO_PIN_SET);
	ASSERT(SIM_GPIO_Read(GPIOB, GPIO_PIN_0));
	ASSERT(!SIM_GPIO_Read(GPIOB, GPIO_PIN_1));
	ASSERT(SIM_GPIO_Read(GPIOB, GPIO_PIN_14));

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_6, GPIO_PIN_RESET);
	ASSERT(!SIM_GPIO_Read(GPIOB, GPIO_PIN_0));
	ASSERT(!SIM_GPIO_Read(GPIOB, GPIO_PIN_1));
	ASSERT(SIM_GPIO_Read(GPIOB, GPIO_PIN_14));
}

TEST_CASE(HAL_GPIO_WritePin_DisabledClock)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef init = {};
	init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_14;
	init.Mode = GPIO_MODE_INPUT;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &init);
	__HAL_RCC_GPIOB_CLK_DISABLE();

	ASSERT_ASSERTS(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_14, GPIO_PIN_SET));
}

TEST_CASE(HAL_GPIO_ReadPin_HappyCase)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef init = {};
	init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_14;
	init.Mode = GPIO_MODE_INPUT;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &init);

	SIM_GPIO_Write(GPIOB, GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_14, true);
	ASSERT(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET);
	ASSERT(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET);
	ASSERT(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_SET);

	SIM_GPIO_Write(GPIOB, GPIO_PIN_0 | GPIO_PIN_6, false);
	ASSERT(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET);
	ASSERT(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET);
	ASSERT(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_SET);
}

TEST_CASE(HAL_GPIO_ReadPin_DisabledClock)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef init = {};
	init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_14;
	init.Mode = GPIO_MODE_INPUT;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &init);
	__HAL_RCC_GPIOB_CLK_DISABLE();

	ASSERT_ASSERTS(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0));
}

TEST_CASE(HAL_GPIO_ReadPin_DisabledPin)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	ASSERT_ASSERTS(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6));
}

TEST_CASE(HAL_GPIO_ReadPin_MultiplePins)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef init = {};
	init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_14;
	init.Mode = GPIO_MODE_INPUT;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &init);

	ASSERT_ASSERTS(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1));
}

