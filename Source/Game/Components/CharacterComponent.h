#pragma once

#include <vector>
#include "Data/PersonaData.h"
#include "Otter/Components/Component.h"

namespace RpgGame {
	
	class CharacterComponent : public Otter::Component, public Character {};
}