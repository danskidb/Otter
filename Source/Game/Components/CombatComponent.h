#pragma once

namespace RpgGame {

	struct CombatComponent {
		int hp = 0;
		int sp = 0;

		CombatComponent() {}
		CombatComponent(unsigned int _hp, unsigned int _sp) : hp(_hp), sp(_sp) {}

		bool IsAlive() {
			return hp > 0;
		}
	};
}