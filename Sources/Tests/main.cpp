#include "pch.h"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Failing test", "[failing]")
{
	REQUIRE(1 == 1);
}

//#include <Core/Common.h>
//
//using namespace RED_LILIUM_NAMESPACE;
//
//TEST_CASE("Test", "Test")
//{
//	vec2 v;
//	v.x = 5;
//	v.y = 6;
//	REQUIRE(v.x + v.y == 11);
//}
