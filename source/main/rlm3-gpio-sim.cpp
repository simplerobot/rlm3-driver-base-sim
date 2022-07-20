#include "rlm3-gpio.h"
#include "rlm3-sim.hpp"
#include "Test.hpp"


struct GPIO_Pin_Config
{
	GPIO_Mode mode;
	GPIO_Pull pull;
	GPIO_Speed speed;
	GPIO_AlternateFunction alternate;
};

struct GPIO_Port_State
{
	bool clock_enabled;
	uint16_t enabled_pins;
	uint16_t output_pins;
	uint16_t input_pins;
	uint16_t pin_values;
	GPIO_Pin_Config pin_configs[16];
};

static GPIO_Port_State g_port_states[GPIO_COUNT];


void GPIO_CLOCK_ENABLE(GPIO_Port port)
{
	ASSERT(port < GPIO_COUNT);
	g_port_states[port].clock_enabled = true;
}

void GPIO_CLOCK_DISABLE(GPIO_Port port)
{
	ASSERT(port < GPIO_COUNT);
	g_port_states[port].clock_enabled = false;
}

extern void HAL_GPIO_Init(GPIO_Port port, GPIO_InitTypeDef* init)
{
	ASSERT(port < GPIO_COUNT);
	ASSERT(init != nullptr);

	// Validate we only are setting valid pins.
	ASSERT(init->Pin != 0);
	ASSERT((init->Pin & ~0xFFFF) == 0);

	// We currently only support digital input and output pins.  NOT alternate functions.
	ASSERT(init->Mode == GPIO_MODE_INPUT || init->Mode == GPIO_MODE_OUTPUT_PP || init->Mode == GPIO_MODE_OUTPUT_OD);

	ASSERT(init->Pull == GPIO_NOPULL || init->Pull == GPIO_PULLUP || init->Pull == GPIO_PULLDOWN);

	ASSERT(init->Speed == GPIO_SPEED_FREQ_LOW || init->Speed == GPIO_SPEED_FREQ_MEDIUM || init->Speed == GPIO_SPEED_FREQ_HIGH || init->Speed == GPIO_SPEED_FREQ_VERY_HIGH);

	ASSERT(init->Alternate == GPIO_AF_UNINITIALIZED);

	GPIO_Port_State& port_state = g_port_states[port];

	// Make sure the port clock is enabled.
	ASSERT(port_state.clock_enabled);

	// Make sure these pins are not already enabled.
	ASSERT((port_state.enabled_pins & init->Pin) == 0);

	// Enable these pins.
	port_state.enabled_pins |= init->Pin;
	if (init->Mode == GPIO_MODE_INPUT)
		port_state.input_pins |= init->Pin;
	else
		port_state.output_pins |= init->Pin;

	// Save the configuration for each affected pin.
	for (size_t pin_index = 0; pin_index < 16; pin_index++)
	{
		uint32_t pin = 1 << pin_index;
		if ((init->Pin & pin) != 0)
		{
			GPIO_Pin_Config& pin_state = port_state.pin_configs[pin_index];
			pin_state.mode = init->Mode;
			pin_state.pull = init->Pull;
			pin_state.speed = init->Speed;
			pin_state.alternate = init->Alternate;
		}
	}
}

extern void HAL_GPIO_DeInit(GPIO_Port port, uint32_t pin)
{
	ASSERT(port < GPIO_COUNT);

	// Validate we only are updating valid pins.
	ASSERT(pin != 0);
	ASSERT((pin & ~0xFFFF) == 0);

	// Make sure these pins are actually enabled.
	GPIO_Port_State& port_state = g_port_states[port];
	ASSERT(port_state.clock_enabled == true);
	ASSERT((pin & ~port_state.enabled_pins) == 0);

	// Clear all the enabled bits for these pins.
	port_state.enabled_pins &= ~pin;
	port_state.output_pins &= ~pin;
	port_state.input_pins &= ~pin;
}

extern GPIO_PinState HAL_GPIO_ReadPin(GPIO_Port port, uint32_t pin)
{
	ASSERT(port < GPIO_COUNT);

	// Validate we only are reading a valid pin.
	ASSERT(pin != 0);
	ASSERT((pin & ~0xFFFF) == 0);
	ASSERT(__builtin_popcount(pin) == 1);

	// Make sure this pin is actually enabled for input.
	GPIO_Port_State& port_state = g_port_states[port];
	ASSERT(port_state.clock_enabled == true);
	ASSERT((pin & ~port_state.enabled_pins) == 0);
	ASSERT((pin & ~port_state.input_pins) == 0);

	if ((pin & port_state.pin_values) != 0)
		return GPIO_PIN_SET;
	else
		return GPIO_PIN_RESET;
}

void HAL_GPIO_WritePin(GPIO_Port port, uint32_t pin, GPIO_PinState state)
{
	ASSERT(port < GPIO_COUNT);
	ASSERT(state == GPIO_PIN_SET || state == GPIO_PIN_RESET);

	// Validate we only are writing valid pins.
	ASSERT(pin != 0);
	ASSERT((pin & ~0xFFFF) == 0);

	// We actually can write to a pin before it has been enabled, but I can't think of a valid use case of writing to an input pin.
	GPIO_Port_State& port_state = g_port_states[port];
	ASSERT((pin & port_state.input_pins) == 0);

	if (state == GPIO_PIN_SET)
		port_state.pin_values |= pin;
	else
		port_state.pin_values &= ~pin;
}

const char* ToString(GPIO_Port port)
{
	switch (port)
	{
	case GPIOA: return "GPIOA";
	case GPIOB: return "GPIOB";
	case GPIOC: return "GPIOC";
	case GPIOD: return "GPIOD";
	case GPIOE: return "GPIOE";
	case GPIOF: return "GPIOF";
	case GPIOG: return "GPIOG";
	case GPIOH: return "GPIOH";
	case GPIOI: return "GPIOI";
	case GPIOJ: return "GPIOJ";
	case GPIOK: return "GPIOK";
	default: return "<invalid>";
	}
}

const char* ToString(GPIO_Mode mode)
{
	switch (mode)
	{
	case GPIO_MODE_UNINITIALIZED:	return "GPIO_MODE_UNINITIALIZED";
	case GPIO_MODE_INPUT:	return "GPIO_MODE_INPUT";
	case GPIO_MODE_OUTPUT_PP:	return "GPIO_MODE_OUTPUT_PP";
	case GPIO_MODE_OUTPUT_OD:	return "GPIO_MODE_OUTPUT_OD";
	case GPIO_MODE_AF_PP:	return "GPIO_MODE_AF_PP";
	case GPIO_MODE_AF_OD:	return "GPIO_MODE_AF_OD";
	case GPIO_MODE_ANALOG:	return "GPIO_MODE_ANALOG";
	case GPIO_MODE_IT_RISING:	return "GPIO_MODE_IT_RISING";
	case GPIO_MODE_IT_FALLING:	return "GPIO_MODE_IT_FALLING";
	case GPIO_MODE_IT_RISING_FALLING:	return "GPIO_MODE_IT_RISING_FALLING";
	case GPIO_MODE_EVT_RISING:	return "GPIO_MODE_EVT_RISING";
	case GPIO_MODE_EVT_FALLING:	return "GPIO_MODE_EVT_FALLING";
	case GPIO_MODE_EVT_RISING_FALLING:	return "GPIO_MODE_EVT_RISING_FALLING";
	default: return "<invalid>";
	}
}

const char* ToString(GPIO_Pull pull)
{
	switch (pull)
	{
	case GPIO_PULL_UNINITIALIZED:	return "GPIO_PULL_UNINITIALIZED";
	case GPIO_NOPULL:	return "GPIO_NOPULL";
	case GPIO_PULLUP:	return "GPIO_PULLUP";
	case GPIO_PULLDOWN:	return "GPIO_PULLDOWN";
	default: return "<invalid>";
	}
}

const char* ToString(GPIO_Speed speed)
{
	switch (speed)
	{
	case GPIO_SPEED_UNINITIALIZED:	return "GPIO_SPEED_UNINITIALIZED";
	case GPIO_SPEED_FREQ_LOW:	return "GPIO_SPEED_FREQ_LOW";
	case GPIO_SPEED_FREQ_MEDIUM:	return "GPIO_SPEED_FREQ_MEDIUM";
	case GPIO_SPEED_FREQ_HIGH:	return "GPIO_SPEED_FREQ_HIGH";
	case GPIO_SPEED_FREQ_VERY_HIGH:	return "GPIO_SPEED_FREQ_VERY_HIGH";
	default: return "<invalid>";
	}
}

const char* ToString(GPIO_AlternateFunction alternate)
{
	switch (alternate)
	{
	case GPIO_AF_UNINITIALIZED:	return "GPIO_AF_UNINITIALIZED";
	case GPIO_AF0_RTC_50Hz:	return "GPIO_AF0_RTC_50Hz";
	case GPIO_AF0_MCO:	return "GPIO_AF0_MCO";
	case GPIO_AF0_TAMPER:	return "GPIO_AF0_TAMPER";
	case GPIO_AF0_SWJ:	return "GPIO_AF0_SWJ";
	case GPIO_AF0_TRACE:	return "GPIO_AF0_TRACE";
	case GPIO_AF1_TIM1:	return "GPIO_AF1_TIM1";
	case GPIO_AF1_TIM2:	return "GPIO_AF1_TIM2";
	case GPIO_AF2_TIM3:	return "GPIO_AF2_TIM3";
	case GPIO_AF2_TIM4:	return "GPIO_AF2_TIM4";
	case GPIO_AF2_TIM5:	return "GPIO_AF2_TIM5";
	case GPIO_AF3_TIM8:	return "GPIO_AF3_TIM8";
	case GPIO_AF3_TIM9:	return "GPIO_AF3_TIM9";
	case GPIO_AF3_TIM10:	return "GPIO_AF3_TIM10";
	case GPIO_AF3_TIM11:	return "GPIO_AF3_TIM11";
	case GPIO_AF4_I2C1:	return "GPIO_AF4_I2C1";
	case GPIO_AF4_I2C2:	return "GPIO_AF4_I2C2";
	case GPIO_AF4_I2C3:	return "GPIO_AF4_I2C3";
	case GPIO_AF5_SPI1:	return "GPIO_AF5_SPI1";
	case GPIO_AF5_SPI2:	return "GPIO_AF5_SPI2";
	case GPIO_AF5_SPI3:	return "GPIO_AF5_SPI3";
	case GPIO_AF5_SPI4:	return "GPIO_AF5_SPI4";
	case GPIO_AF5_SPI5:	return "GPIO_AF5_SPI5";
	case GPIO_AF5_SPI6:	return "GPIO_AF5_SPI6";
	case GPIO_AF6_SPI3:	return "GPIO_AF6_SPI3";
	case GPIO_AF6_SAI1:	return "GPIO_AF6_SAI1";
	case GPIO_AF7_USART1:	return "GPIO_AF7_USART1";
	case GPIO_AF7_USART2:	return "GPIO_AF7_USART2";
	case GPIO_AF7_USART3:	return "GPIO_AF7_USART3";
	case GPIO_AF8_UART4:	return "GPIO_AF8_UART4";
	case GPIO_AF8_UART5:	return "GPIO_AF8_UART5";
	case GPIO_AF8_USART6:	return "GPIO_AF8_USART6";
	case GPIO_AF8_UART7:	return "GPIO_AF8_UART7";
	case GPIO_AF8_UART8:	return "GPIO_AF8_UART8";
	case GPIO_AF9_CAN1:	return "GPIO_AF9_CAN1";
	case GPIO_AF9_CAN2:	return "GPIO_AF9_CAN2";
	case GPIO_AF9_TIM12:	return "GPIO_AF9_TIM12";
	case GPIO_AF9_TIM13:	return "GPIO_AF9_TIM13";
	case GPIO_AF9_TIM14:	return "GPIO_AF9_TIM14";
	case GPIO_AF10_OTG_FS:	return "GPIO_AF10_OTG_FS";
	case GPIO_AF10_OTG_HS:	return "GPIO_AF10_OTG_HS";
	case GPIO_AF11_ETH:	return "GPIO_AF11_ETH";
	case GPIO_AF12_FMC:	return "GPIO_AF12_FMC";
	case GPIO_AF12_OTG_HS_FS:	return "GPIO_AF12_OTG_HS_FS";
	case GPIO_AF12_SDIO:	return "GPIO_AF12_SDIO";
	case GPIO_AF13_DCMI:	return "GPIO_AF13_DCMI";
	case GPIO_AF15_EVENTOUT:	return "GPIO_AF15_EVENTOUT";
	default: return "<invalid>";
	}
}

const char* ToString(GPIO_PinState state)
{
	switch (state)
	{
	case GPIO_PIN_RESET:	return "GPIO_PIN_RESET";
	case GPIO_PIN_SET:	return "GPIO_PIN_SET";
	default: return "<invalid>";
	}
}

std::ostream& operator<<(std::ostream& out, GPIO_Port port)
{
	out << ToString(port);
	return out;
}

std::ostream& operator<<(std::ostream& out, GPIO_Mode mode)
{
	out << ToString(mode);
	return out;
}

std::ostream& operator<<(std::ostream& out, GPIO_Pull pull)
{
	out << ToString(pull);
	return out;
}

std::ostream& operator<<(std::ostream& out, GPIO_Speed speed)
{
	out << ToString(speed);
	return out;
}

std::ostream& operator<<(std::ostream& out, GPIO_AlternateFunction alternate)
{
	out << ToString(alternate);
	return out;
}

std::ostream& operator<<(std::ostream& out, GPIO_PinState state)
{
	out << ToString(state);
	return out;
}
