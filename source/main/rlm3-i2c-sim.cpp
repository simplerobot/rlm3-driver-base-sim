#include "rlm3-i2c.h"
#include "rlm3-sim.hpp"
#include "Test.hpp"
#include <sstream>
#include "Mock.hpp"


static uint32_t g_active_devices_i2c1 = 0;


extern void RLM3_I2C1_Init(RLM3_I2C1_DEVICE device)
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	uint32_t mask = 1 << device;
	ASSERT(device < RLM3_I2C1_DEVICE_COUNT);
	ASSERT((g_active_devices_i2c1 & mask) == 0);
	g_active_devices_i2c1 |= mask;
}

extern void RLM3_I2C1_Deinit(RLM3_I2C1_DEVICE device)
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	uint32_t mask = 1 << device;
	ASSERT(device < RLM3_I2C1_DEVICE_COUNT);
	ASSERT((g_active_devices_i2c1 & mask) != 0);
	g_active_devices_i2c1 &= ~mask;
}

extern bool RLM3_I2C1_IsInit(RLM3_I2C1_DEVICE device)
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	uint32_t mask = 1 << device;
	ASSERT(device < RLM3_I2C1_DEVICE_COUNT);
	return ((g_active_devices_i2c1 & mask) != 0);
}

extern bool RLM3_I2C1_Transmit(uint32_t addr, const uint8_t* data, size_t size)
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	MOCK_CALL(addr, MockData(data, size));
	MOCK_RETURN(bool);
}

extern bool RLM3_I2C1_Receive(uint32_t addr, uint8_t* data, size_t size)
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	MOCK_CALL(addr, MockData(data, size));
	MOCK_RETURN(bool);
}

extern bool RLM3_I2C1_TransmitReceive(uint32_t addr, const uint8_t* tx_data, size_t tx_size, uint8_t* rx_data, size_t rx_size)
{
	ASSERT(!SIM_RLM3_Is_IRQ());
	MockData tx(tx_data, tx_size);
	MockData rx(rx_data, rx_size);
	MOCK_CALL(addr, tx);
	MOCK_OUTPUT(rx);
	MOCK_RETURN(bool);
}
