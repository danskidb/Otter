#pragma once

#include <cstdint>
#include <bitset>

#include "Types/OArray.h"
#include "Types/OString.h"

namespace Otter {

	using EntityId = std::uint32_t;
	const EntityId MAX_ENTITIES = 5000;

	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;

	// ID to track which components an entity has. 
	// With this we can make simple bitwise comparisons to check whether an entity has X components.
	using Signature = std::bitset<MAX_COMPONENTS>;
}