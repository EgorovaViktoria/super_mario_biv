#include "second_level.hpp"

#include "third_level.hpp"

using biv::SecondLevel;

SecondLevel::SecondLevel(UIFactory* ui_factory) : GameLevel(ui_factory) {
	init_data();
}

biv::GameLevel* SecondLevel::get_next() {
	if (!next) {
		clear_data();
		next = new biv::ThirdLevel(ui_factory);
	}
	return next;
}

bool SecondLevel::is_final() const noexcept {
	return false;
}

// ----------------------------------------------------------------------------
// 									PROTECTED
// ----------------------------------------------------------------------------
void SecondLevel::init_data() {
	ui_factory->create_mario({39, 10}, 3, 3);

	ui_factory->create_ship({10, 25}, 30, 2);
	ui_factory->create_ship({60, 22}, 50, 3);
	ui_factory->create_ship({140, 25}, 40, 2);

	ui_factory->create_box({30, 12}, 8, 3);
	ui_factory->create_full_box({50, 12}, 5, 3);
	ui_factory->create_box({80, 12}, 6, 3);

	ui_factory->create_enemy({20, 5}, 3, 2);
	ui_factory->create_enemy({40, 5}, 3, 2);
	ui_factory->create_enemy({90, 5}, 3, 2);

	// additional objects
	ui_factory->create_money({35, 11}, 1, 1);
	ui_factory->create_money({95, 11}, 1, 1);

	// new enemy types
	ui_factory->create_flying_enemy({75, 6}, 3, 2);
	ui_factory->create_jumping_enemy({160, 9}, 3, 2);
}
