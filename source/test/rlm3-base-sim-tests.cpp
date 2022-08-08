#include "Test.hpp"
#include "rlm3-base.h"


TEST_CASE(GetUniqueDeviceId_HappyCase)
{
	uint8_t id[12];
	RLM3_GetUniqueDeviceId(id);
}

TEST_CASE(SetUniqueDeviceId_HappyCase)
{
	uint8_t id_in[12];
	for (size_t i = 0; i < 12; i++)
		id_in[i] = i;
	SIM_SetUniqueDeviceId(id_in);

	uint8_t id_out[12];
	RLM3_GetUniqueDeviceId(id_out);
	for (size_t i = 0; i < 12; i++)
		ASSERT(id_out[i] == i);
}

