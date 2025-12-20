#include "third_level.hpp"

using biv::ThirdLevel;

ThirdLevel::ThirdLevel(UIFactory* ui_factory) : GameLevel(ui_factory) {
	init_data();
}

biv::GameLevel* ThirdLevel::get_next() {
	// No next level yet
	return nullptr;
}

bool ThirdLevel::is_final() const noexcept {
	// Третий уровень — финальный (можно вернуть false, если планируете добавлять дальше)
	return true;
}

void ThirdLevel::init_data() {
	// Minimal safe content so level isn't empty
	ui_factory->create_mario({39, 10}, 3, 3);
	ui_factory->create_ship({10, 25}, 30, 2);
}
