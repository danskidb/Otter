#pragma once

namespace RpgGame {

	struct CombatComponent {
		unsigned int hp;
		unsigned int sp;

		CombatComponent() {}
		CombatComponent(unsigned int _hp, unsigned int _sp) : hp(_hp), sp(_sp) {}
	};
}