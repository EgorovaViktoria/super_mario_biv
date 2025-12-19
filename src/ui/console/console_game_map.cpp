#include "console_game_map.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace biv {

ConsoleGameMap::ConsoleGameMap(const int height, const int width) 
	: GameMap(height, width) {

	map = new char*[height];
	for (int i = 0; i < height; i++) {
		map[i] = new char[width + 1];
	}
		
	clear();
	needs_draw = true;
}

ConsoleGameMap::~ConsoleGameMap() {
	for (int i = 0; i < height; i++) {
		delete [] map[i];
	}
	delete [] map;
}

void ConsoleGameMap::add_obj(ConsoleUIObject* obj) {
	objs.push_back(obj);
}

void ConsoleGameMap::clear() noexcept {
	// Воздух
	for (int i = 0; i < width; i++) {
		map[0][i] = ' ';
	}
	map[0][width] = '\0';
	
	for (int i = 1; i < height - 3; i++) {
		std::strcpy(map[i], map[0]);
	}
	
	// Вода
	for (int i = 0; i < width; i++) {
		map[height - 3][i] = '~';
	}
	map[height - 3][width] = '\0';
	
	for (int i = height - 2; i < height; i++) {
		std::strcpy(map[i], map[height - 3]);
	}
}

void ConsoleGameMap::refresh() noexcept {
	clear();
	
	for (ConsoleUIObject* obj: objs) {
		int left = obj->get_left();
		int top = obj->get_top();
		int right = obj->get_right();
		int bottom = obj->get_bottom();
		
		char brush = obj->get_brush();
		
		for (int i = left; i < right; i++) {
			for (int j = top; j < bottom; j++) {
				if (is_on_map(i, j)) {
					map[j][i] = brush;
				}
			}
		}
	}

	// Буфер обновлён — нужно перерисовать при следующем show()
	needs_draw = true;
}

void ConsoleGameMap::remove_obj(ConsoleUIObject* obj) {
	objs.erase(std::remove(objs.begin(), objs.end(), obj), objs.end());
}

void ConsoleGameMap::remove_objs() {
	objs.clear();
}

void ConsoleGameMap::show() const noexcept {
	// Если буфер не изменился с последней отрисовки, ничего не печатаем.
	if (!needs_draw) return;

	// Очистить экран и переместить курсор в начало (полная перерисовка).
	std::cout << "\x1b[2J\x1b[H";

	// Соберём весь экран в один буфер и выведем единой операцией (меньше мерцания).
	std::string buffer;
	buffer.reserve(static_cast<size_t>(height) * (width + 1));

	for (int i = 0; i < height; ++i) {
		buffer.append(map[i], static_cast<size_t>(width));
		buffer.push_back('\n');
	}

	std::cout << buffer << std::flush;

	// Сброс флага — повторные вызовы show() без предварительного refresh() больше не будут печатать.
	needs_draw = false;
}

} // namespace biv
