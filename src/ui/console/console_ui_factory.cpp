#include "console_ui_factory.hpp"

using biv::ConsoleUIFactory;

ConsoleUIFactory::ConsoleUIFactory(Game* game) : UIFactory(game) {
	create_game_map();
}

void ConsoleUIFactory::clear_data() {
	game->remove_objs();
	if (game_map) game_map->remove_objs();
	if (mario != nullptr) {
		game->remove_collisionable(mario);
		game->remove_movable(mario);
		game->remove_mario();
		if (game_map) game_map->remove_obj(mario);
		delete mario;
		mario = nullptr;
	}
	// Удаляем и освобождаем все контейнеры врагов и объектов
	for (auto b : boxes) delete b;
	boxes.clear();
	for (auto fb : full_boxes) delete fb;
	full_boxes.clear();
	for (auto s : ships) delete s;
	ships.clear();
	for (auto e : enemies) delete e;
	enemies.clear();
	for (auto m : moneys) delete m;
	moneys.clear();

	// Новые типы врагов
	for (auto fe : flying_enemies) delete fe;
	flying_enemies.clear();
	for (auto je : jumping_enemies) delete je;
	jumping_enemies.clear();
}

void ConsoleUIFactory::create_box(
	const Coord& top_left, const int width, const int height
) {
	ConsoleBox* box = new ConsoleBox(top_left, width, height);
	boxes.push_back(box);
	game->add_map_movable(box);
	game->add_static_obj(box);
	if (game_map) game_map->add_obj(box);
}

void ConsoleUIFactory::create_enemy(
	const Coord& top_left, const int width, const int height
) {
	ConsoleEnemy* enemy = new ConsoleEnemy(top_left, width, height);
	enemies.push_back(enemy);
	game->add_map_movable(enemy);
	game->add_movable(enemy);
	game->add_collisionable(enemy);
	if (game_map) game_map->add_obj(enemy);
}

void ConsoleUIFactory::create_full_box(
	const Coord& top_left, const int width, const int height
) {
	ConsoleFullBox* full_box = new ConsoleFullBox(top_left, width, height, this);
	full_boxes.push_back(full_box);
	game->add_collisionable(full_box);
	game->add_map_movable(full_box);
	game->add_static_obj(full_box);
	if (game_map) game_map->add_obj(full_box);
}

void ConsoleUIFactory::create_mario(
	const Coord& top_left, const int width, const int height
) {
	// Удаляем старого Марио, только если он реально существует.
	if (mario != nullptr) {
		game->remove_collisionable(mario);
		game->remove_movable(mario);
		game->remove_mario();
		if (game_map) game_map->remove_obj(mario);
		delete mario;
		mario = nullptr;
	}

	mario = new ConsoleMario(top_left, width, height);

	game->add_collisionable(mario);
	game->add_movable(mario);
	game->add_mario(mario);

	if (game_map) {
		game_map->add_obj(mario);
	}
}

void ConsoleUIFactory::create_money(
	const Coord& top_left, const int width, const int height
) {
	ConsoleMoney* money = new ConsoleMoney(top_left, width, height);
	moneys.push_back(money);
	game->add_map_movable(money);
	game->add_movable(money);
	game->add_collisionable(money);
	if (game_map) game_map->add_obj(money);
}

void ConsoleUIFactory::create_ship(
	const Coord& top_left, const int width, const int height
) {
	ConsoleShip* ship = new ConsoleShip(top_left, width, height);
	ships.push_back(ship);
	game->add_map_movable(ship);
	game->add_static_obj(ship);
	if (game_map) game_map->add_obj(ship);
}

void ConsoleUIFactory::create_flying_enemy(
	const Coord& top_left, const int width, const int height
) {
	ConsoleFlyingEnemy* fe = new ConsoleFlyingEnemy(top_left, width, height);
	flying_enemies.push_back(fe);
	game->add_map_movable(fe);
	game->add_movable(fe);
	game->add_collisionable(fe);
	if (game_map) game_map->add_obj(fe);
}

// Создаем прыгающего врага и регистрируем точно так же, как обычного врага,
// чтобы он участвовал в проверках столкновений и не проваливался сквозь платформы.
void ConsoleUIFactory::create_jumping_enemy(
	const Coord& top_left, const int width, const int height
) {
	ConsoleJumpingEnemy* je = new ConsoleJumpingEnemy(top_left, width, height);
	jumping_enemies.push_back(je);
	game->add_map_movable(je);
	game->add_movable(je);
	game->add_collisionable(je);
	if (game_map) game_map->add_obj(je);
}

biv::GameMap* ConsoleUIFactory::get_game_map() {
	return game_map;
}

biv::Mario* ConsoleUIFactory::get_mario() {
	return mario;
}

// ----------------------------------------------------------------------------
// 									PRIVATE
// ----------------------------------------------------------------------------
void ConsoleUIFactory::create_game_map() {
	game_map = new ConsoleGameMap(30, 200);
}
