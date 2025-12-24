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

	// Платформа 1
	ui_factory->create_ship({10, 25}, 30, 2);

	// Платформа 2
	ui_factory->create_ship({60, 22}, 50, 3);

	// Промежуточные коробки/блоки
	ui_factory->create_box({30, 12}, 8, 3);
	ui_factory->create_full_box({70, 12}, 5, 3);
	ui_factory->create_box({80, 12}, 6, 3);

	// Враги/доп. объекты
	ui_factory->create_enemy({20, 5}, 3, 2);
	ui_factory->create_enemy({40, 5}, 3, 2);
	ui_factory->create_enemy({90, 5}, 3, 2);

	// Монетки
	

	// Летающий враг
	ui_factory->create_flying_enemy({75, 6}, 3, 2);

	// Платформа 3 — отдельная, не склеивается с платформой 2 (ставим с небольшим отступом)
	// платформа 2: x in [60, 110), поэтому ставим платформу 3 начиная с 120
	ui_factory->create_ship({120, 22}, 30, 3); // третья платформа (120..150)

	// Платформа 4 (целевая) — создаём последней, чтобы она была static_objs.back()
	ui_factory->create_ship({170, 22}, 20, 3); // четвертая (целевая) платформа (170..190)

	// Прыгающий враг располагаем на ПРЕДПОСЛЕДНЕЙ (третьей) платформе, а не на целевой.
	// Платформа 3 top = 22, высота врага = 2 => top_enemy = 22 - 2 = 20
	// Ставим по центру платформы 3: x = 120 + 30/2 - width/2 = 135 - 1.5 ≈ 134-135
	ui_factory->create_jumping_enemy({135, 20}, 3, 2);
}
