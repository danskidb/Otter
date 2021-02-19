#pragma once

#include "Otter/Components/Component.h"

namespace RpgGame {

	class CombatComponent : public Otter::Component {
	public:
		int hp = 0;
		int sp = 0;

		CombatComponent() : Otter::Component() {}
		CombatComponent(unsigned int _hp, unsigned int _sp) : Otter::Component(), hp(_hp), sp(_sp) {}

		bool IsAlive() {
			return hp > 0;
		}
	};
}