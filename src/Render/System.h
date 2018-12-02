#pragma once

#include "RenderForward.h"

namespace RED_LILIUM_NAMESPACE
{
namespace Render
{

class System : public RedLiliumObject, public std::enable_shared_from_this<System>
{
public:
	System();
	~System() override;

private:

};

}
} // namespace RED_LILIUM_NAMESPACE
