#include "third_level.hpp"

using biv::ThirdLevel;

ThirdLevel::ThirdLevel(UIFactory* ui_factory) : GameLevel(ui_factory) {
	init_data();
}

biv::GameLevel* ThirdLevel::get_next() {
	return nullptr;
}

bool ThirdLevel::is_final() const noexcept {
	return true;
}

void ThirdLevel::init_data() {
	ui_factory->create_mario({20, 10}, 3, 3);

	ui_factory->create_ship({10, 25}, 30, 2);

	ui_factory->create_ship({50, 20}, 20, 2);
	ui_factory->create_enemy({60, 15}, 3, 2); 

	ui_factory->create_box({80, 15}, 5, 3);
	ui_factory->create_box({90, 10}, 5, 3);    
	ui_factory->create_box({100, 15}, 5, 3);

	ui_factory->create_flying_enemy({90, 5}, 3, 2);

	ui_factory->create_ship({120, 22}, 40, 3); 
	
	ui_factory->create_jumping_enemy({140, 15}, 3, 2);
	
	ui_factory->create_full_box({130, 10}, 5, 3);
	ui_factory->create_full_box({150, 10}, 5, 3);

	ui_factory->create_ship({180, 25}, 20, 5); 
	
	ui_factory->create_enemy({185, 20}, 3, 2);
}
