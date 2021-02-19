#pragma once

#include "Data/PersonaData.h"
#include "Otter/Components/Component.h"

namespace RpgGame {

	class PersonaComponent : public Otter::Component, public Persona {};
}