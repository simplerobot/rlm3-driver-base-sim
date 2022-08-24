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

TEST_CASE(DebugOutput_HappyCase)
{
	SIM_ExpectDebugOutput("abc");

	RLM3_DebugOutput('a');
	RLM3_DebugOutput('b');
	RLM3_DebugOutput('c');
}

TEST_CASE(DebugOutput_None)
{
	SIM_ExpectDebugOutput("");
}

TEST_CASE(DebugOutput_NonMatching)
{
	auto test = [] {
		SIM_ExpectDebugOutput("abc");

		RLM3_DebugOutput('a');
		RLM3_DebugOutput('d');
		RLM3_DebugOutput('c');
	};

	TestCaseListItem test_case(test, "test", __FILE__, __LINE__);
	ASSERT(!test_case.Run());
}

TEST_CASE(DebugOutput_Missing)
{
	auto test = [] {
		SIM_ExpectDebugOutput("abc");

		RLM3_DebugOutput('a');
		RLM3_DebugOutput('b');
	};

	TestCaseListItem test_case(test, "test", __FILE__, __LINE__);
	ASSERT(!test_case.Run());
}

TEST_CASE(DebugOutput_Extra)
{
	auto test = [] {
		SIM_ExpectDebugOutput("abc");

		RLM3_DebugOutput('a');
		RLM3_DebugOutput('b');
		RLM3_DebugOutput('c');
		RLM3_DebugOutput('d');
	};

	TestCaseListItem test_case(test, "test", __FILE__, __LINE__);
	ASSERT(!test_case.Run());
}




