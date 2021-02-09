#pragma once

#include "Types.h"
#include <set>

namespace Otter {

	class System
	{
	public: 
		std::set<EntityId> entities;
		virtual void OnRegistered() = 0;
	};
}