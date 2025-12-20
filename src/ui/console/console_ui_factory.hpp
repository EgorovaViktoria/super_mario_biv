#pragma once

#include <vector>

#include "ui_factory.hpp"
#include "console_ui_obj_rect_adapter.hpp"
#include "ui_objects/console_box.hpp"
#include "ui_objects/console_full_box.hpp"
#include "ui_objects/console_ship.hpp"
#include "ui_objects/console_enemy.hpp"
#include "ui_objects/console_mario.hpp"
#include "ui_objects/console_money.hpp"
#include "ui_objects/console_flying_enemy.hpp"
#include "ui_objects/console_jumping_enemy.hpp"
#include "console_game_map.hpp"

#include "ui_objects/console_flying_enemy.hpp"
#include "ui_objects/console_jumping_enemy.hpp"

namespace biv {

	class ConsoleUIFactory : public UIFactory {
		public:
			ConsoleUIFactory(Game* game);
			virtual void clear_data() override;

			virtual void create_box(const Coord&, const int width, const int height) override;
			virtual void create_enemy(const Coord&, const int width, const int height) override;
			virtual void create_full_box(const Coord&, const int width, const int height) override;
			virtual void create_mario(const Coord&, const int width, const int height) override;
			virtual void create_money(const Coord&, const int width, const int height) override;
			virtual void create_ship(const Coord&, const int width, const int height) override;

			// Доп. виды врагов
			virtual void create_flying_enemy(const Coord&, const int width, const int height) override;
			virtual void create_jumping_enemy(const Coord&, const int width, const int height) override;

			biv::GameMap* get_game_map() override;
			biv::Mario* get_mario() override;

		private:
			// Указатель на реализацию карты для консольного UI
			ConsoleGameMap* game_map = nullptr;

			// хранилища UI-объектов
			std::vector<ConsoleBox*> boxes;
			std::vector<ConsoleFullBox*> full_boxes;
			std::vector<ConsoleShip*> ships;
			std::vector<ConsoleEnemy*> enemies;
			std::vector<ConsoleMoney*> moneys;

			// хранилища для новых типов врагов
			std::vector<ConsoleFlyingEnemy*> flying_enemies;
			std::vector<ConsoleJumpingEnemy*> jumping_enemies;

			// текущий Mario (консольный)
			ConsoleMario* mario = nullptr;

			void create_game_map();
	};

} // namespace biv
