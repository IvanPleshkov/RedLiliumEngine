#include "pch.h"

#include "gtest/gtest.h"

using namespace RED_LILIUM_NAMESPACE;

TEST(Test, Test)
{
	vec2 v;
	v.x = 5;
	v.y = 6;
	ASSERT_TRUE(v.x + v.y == 11);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
