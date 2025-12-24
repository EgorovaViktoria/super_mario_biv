/**
	Игровой цикл - это обощённое представление потока событий, происходящих в игре.

	Игровой цикл:
		1. Установка параметров игры
		2. Получение пользовательского ввода
		3. Обновление внутреннего состояния игры
		4. Обновление изображения на экране
		5. Проверка того, не окончена ли игра

		*** Если игра не окончена, то возвращаемся в пункт № 2.

		6. Завершение
*/
#include <thread>
#include <QApplication>
#include <QTimer>

#include "qt_ui_factory.hpp"
#include "qt_game_map.hpp"
#include "first_level.hpp"
#include "game.hpp"
#include "game_level.hpp"
#include "game_map.hpp"
#include "mario.hpp"
#include "os_control_settings.hpp"
#include "ui_factory.hpp"
#include "user_input.hpp"

int main(int argc, char *argv[]) {
	using namespace std::chrono_literals;
	biv::os::init_settings();
	
	QApplication app(argc, argv);
	
	biv::Game game;
	biv::UIFactory* ui_factory = new biv::QtUIFactory(&game);
	biv::GameMap* game_map = ui_factory->get_game_map();
	biv::GameLevel* game_level = new biv::FirstLevel(ui_factory);
	biv::Mario* mario = ui_factory->get_mario();
	
	QTimer* timer = new QTimer();
	QObject::connect(timer, &QTimer::timeout, [&]() {
		biv::os::UserInput user_input = dynamic_cast<biv::QtGameMap*>(game_map)->get_input();
		
		switch (user_input) {
			case biv::os::UserInput::MAP_LEFT:
				mario->move_map_left();
				if (!game.check_static_collisions(mario)) {
					game.move_map_left();
				}
				mario->move_map_right();
				break;
			case biv::os::UserInput::MAP_RIGHT:
				mario->move_map_right();
				if (!game.check_static_collisions(mario)) {
					game.move_map_right();
				}
				mario->move_map_left();
				break;
			case biv::os::UserInput::MARIO_JUMP:
				mario->jump();
				break;
			case biv::os::UserInput::EXIT:
				game.finish();
				break;
			default:
				break;
		}
		
		game.move_objs_horizontally();
		game.check_horizontally_static_collisions();
		
		game.move_objs_vertically();
		game.check_mario_collision();
		game.check_vertically_static_collisions();
		
		if (
			game_map->is_below_map(mario->get_top())
			|| !mario->is_active()
		) {
			game_level->restart();
			game.start_level();
			mario = ui_factory->get_mario();
		}
		
		if (game.is_level_end()) {
			if (!game_level->is_final()) {
				game_level = game_level->get_next();
				mario = ui_factory->get_mario();
				game.start_level();
			} else {
				game.finish();
			}
		}
		
		game_map->refresh();
		
		if (game.is_finished()) {
			QApplication::quit();
		}
	});
	
	timer->start(16); // ~60 FPS
	game_map->show();
	
	return app.exec();
}
