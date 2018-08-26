#include "pch.h"

#include <Core/SmartPointers.h>
#include "Tests.h"

using namespace RED_LILIUM_NAMESPACE;

struct TestBaseClass : RedLiliumObject
{
	u32 m_baseValue;
};

struct TestClass : TestBaseClass
{
	u32 m_childValue;
};

TEST(RedLiliumWeakPointer, PointerConstructor_1)
{
	uptr<TestClass> u = std::make_unique<TestClass>();
	ptr<TestClass> p = u.get();
}

TEST(RedLiliumWeakPointer, ConstPointerConstructor_1)
{
	uptr<TestClass> u = std::make_unique<TestClass>();
	const ptr<const TestClass> p = u.get();
}

TEST(RedLiliumWeakPointer, PointerConstructor_2)
{
	uptr<TestClass> u = std::make_unique<TestClass>();
	ptr<TestBaseClass> p = u.get();
}

TEST(RedLiliumWeakPointer, ConstPointerConstructor_2)
{
	uptr<TestClass> u = std::make_unique<TestClass>();
	const ptr<const TestBaseClass> p = u.get();
}

TEST(RedLiliumWeakPointer, PointerConstructor_3)
{
	TestClass u;
	{
		ptr<TestClass> p = &u;
	}
}

TEST(RedLiliumWeakPointer, ConstPointerConstructor_3)
{
	TestClass u;
	{
		const ptr<const TestClass> p = &u;
	}
}

TEST(RedLiliumWeakPointer, PointerConstructor_4)
{
	TestClass u;
	{
		ptr<TestBaseClass> p = &u;
	}
}

TEST(RedLiliumWeakPointer, ConstPointerConstructor_4)
{
	TestClass u;
	{
		const ptr<const TestBaseClass> p = &u;
	}
}

TEST(RedLiliumWeakPointer, NullptrConstructor)
{
	ptr<TestClass> p = nullptr;
}

TEST(RedLiliumWeakPointer, ConstNullptrConstructor)
{
	const ptr<const TestClass> p = nullptr;
}

TEST(RedLiliumWeakPointer, CopyConstructor_1)
{
	TestClass u;
	{
		ptr<TestClass> p1 = &u;
		ptr<TestClass> p2(p1);
		ASSERT_TRUE(p1 == p2);
	}
}

TEST(RedLiliumWeakPointer, CopyConstructor_2)
{
	TestClass u;
	{
		ptr<TestClass> p1 = &u;
		const ptr<const TestClass> p2(p1);
		ASSERT_TRUE(p1 == p2);
	}
}

TEST(RedLiliumWeakPointer, CopyConstructor_3)
{
	TestClass u;
	{
		const ptr<const TestClass> p1 = &u;
		const ptr<const TestClass> p2(p1);
		ASSERT_TRUE(p1 == p2);
	}
}

TEST(RedLiliumWeakPointer, CopyConstructor_4)
{
	TestClass u;
	{
		ptr<TestClass> p1 = &u;
		ptr<TestBaseClass> p2(p1);
		ASSERT_TRUE(p1 == p2);
	}
}

TEST(RedLiliumWeakPointer, CopyConstructor_5)
{
	TestClass u;
	{
		ptr<TestClass> p1 = &u;
		const ptr<const TestBaseClass> p2(p1);
		ASSERT_TRUE(p1 == p2);
	}
}

TEST(RedLiliumWeakPointer, CopyConstructor_6)
{
	TestClass u;
	{
		const ptr<const TestClass> p1 = &u;
		const ptr<const TestBaseClass> p2(p1);
		ASSERT_TRUE(p1 == p2);
	}
}

TEST(RedLiliumWeakPointer, Multithreading_1)
{
	TestClass o1, o2, o3;

	const u32 instancesCount = 1000;
	const u32 threadsCount = 4;

	std::vector<std::vector<ptr<TestClass>>> data;
	for (u32 j = 0; j < threadsCount; j++)
	{
		data.push_back(std::vector<ptr<TestClass>>());
		for (u32 i = 0; i < instancesCount; i++)
		{
			data[j].push_back(nullptr);
		}
	}

	std::vector<std::function<void()>> threadFuncs;
	std::atomic<u32> runnedThreads(threadsCount);

	for (u32 thread = 0; thread < threadsCount; thread++)
	{
		auto threadFund = [&, thread]()
		{
			const u32 triesCount = 1000;
			for (u32 tryIndex = 0; tryIndex < triesCount; tryIndex++)
			{
				const u32 vecSize = 500;
				std::vector<ptr<TestClass>> vec;
				for (u32 i = 0; i < vecSize; i++)
				{
					// construct
					vec.push_back(ptr<TestClass>(&o1));
					vec.push_back(ptr<TestClass>(&o2));
					vec.push_back(ptr<TestClass>(&o3));

					ptr<TestClass> p1 = &o1;
					ptr<TestClass> p2 = &o2;
					ptr<TestClass> p3 = &o3;

					// copy
					auto it = vec.begin() + vec.size() - 3;
					*it = &o3; it++;
					*it = &o1; it++;
					*it = &o2; it++;

					// move
					ptr<TestClass> c1 = std::move(p1);
					ptr<TestClass> c2 = std::move(p2);
					ptr<TestClass> c3 = std::move(p3);

					// destructors
					{
						ptr<TestClass> d1 = &o1;
						ptr<TestClass> d2 = &o2;
						ptr<TestClass> d3 = &o3;
					}
				}

				// swap
				std::sort(vec.begin(), vec.end());
			}

			runnedThreads--;
		};
		threadFuncs.push_back(threadFund);
	}

	std::vector<std::thread> threads;
	for (u32 thread = 0; thread < threadsCount; thread++)
	{
		threads.push_back(std::thread(threadFuncs[thread]));
		threads.back().detach();
	}

	while (true)
	{
		if (runnedThreads == 0)
		{
			break;
		}
	}
}
