#include "CharacterComponent.h"
#include "Otter/Log.h"

namespace RpgGame {

	void CharacterComponent::AddPersona(EntityId personaEntityId)
	{
		instantiatedPersona.push_back(personaEntityId);

		if (activePersona < 0)
			activePersona = 0;
	}

	void CharacterComponent::ActivatePersona(unsigned int toActivate)
	{
		if (toActivate < instantiatedPersona.size())
			activePersona = toActivate;
	}

	Otter::EntityId CharacterComponent::GetActivePersona()
	{
		OT_ASSERT(activePersona >= 0, "Character " + name + " wanted to return active persona but this was invalid!");
		return instantiatedPersona[activePersona];
	}

}