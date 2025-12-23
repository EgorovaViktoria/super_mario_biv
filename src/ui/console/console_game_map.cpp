#include "console_game_map.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <sys/ioctl.h>
#include <unistd.h>

#include "collisionable.hpp"

using biv::ConsoleGameMap;

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

// add_obj: защита от дублирования, без логов
void ConsoleGameMap::add_obj(ConsoleUIObject* obj) {
	if (std::find(objs.begin(), objs.end(), obj) == objs.end()) {
		objs.push_back(obj);
	}
}

// Быстрая и корректная очистка карты
void ConsoleGameMap::clear() noexcept {
	for (int r = 0; r < height; ++r) {
		std::memset(map[r], ' ', width);
		map[r][width] = '\0';
	}

	// Заполняем "воду" внизу
	for (int i = 0; i < width; i++) {
		map[height - 3][i] = '~';
	}
	map[height - 3][width] = '\0';
	for (int i = height - 2; i < height; i++) {
		std::memcpy(map[i], map[height - 3], width + 1);
	}
}

void ConsoleGameMap::refresh() noexcept {
	clear();
	
	for (ConsoleUIObject* obj: objs) {
		// Если объект также реализует Collisionable и он не активен — пропускаем.
		if (auto coll = dynamic_cast<biv::Collisionable*>(obj)) {
			if (!coll->is_active()) continue;
		}

		int left = obj->get_left();
		int top = obj->get_top();
		int right = obj->get_right();
		int bottom = obj->get_bottom();
		
		// защита от ненаходящихся в карте объектов / неверных координат
		if (left >= right || top >= bottom) continue;
		if (right <= 0 || bottom <= 0) continue; // полностью вне слева/сверху
		if (left >= width || top >= height) continue; // полностью вне справа/снизу

		// корректируем диапазоны под карту
		int cl = std::max(0, left);
		int ct = std::max(0, top);
		int cr = std::min(width, right);
		int cb = std::min(height, bottom);

		char brush = obj->get_brush();
		
		for (int i = cl; i < cr; i++) {
			for (int j = ct; j < cb; j++) {
				if (is_on_map(i, j)) {
					map[j][i] = brush;
				}
			}
		}
	}

	needs_draw = true;
}

void ConsoleGameMap::remove_obj(ConsoleUIObject* obj) {
	objs.erase(std::remove(objs.begin(), objs.end(), obj), objs.end());
}

void ConsoleGameMap::remove_objs() {
	objs.clear();
}

// Отрисовываем экран единым буфером — уменьшаем мерцание.
// Автоматически определяем размер терминала и подрезаем вывод по строкам/колонкам.
// Если число печатаемых строк совпадает с высотой терминала, не добавляем завершающий '\n',
// чтобы избежать прокрутки терминала.
void ConsoleGameMap::show() const noexcept {
	if (!needs_draw) return;

	int term_rows = -1;
	int term_cols = -1;

	// Определяем размер терминала (только если stdout привязан к tty)
	if (isatty(STDOUT_FILENO)) {
		struct winsize w;
		if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
			term_rows = static_cast<int>(w.ws_row);
			term_cols = static_cast<int>(w.ws_col);
		}
	}

	// Если ioctl не сработал, используем полные размеры карты
	int rows_to_print = (term_rows > 0) ? std::min(term_rows, height) : height;
	int cols_to_print = (term_cols > 0) ? std::min(term_cols, width) : width;

	// Если терминальная высота совпадает с полной высотой карты (rows_to_print == height),
	// то мы не добавляем завершающий '\n' после последней строки, чтобы не вызвать scroll.
	bool omit_final_newline = (term_rows > 0 && term_rows == height);

	std::string out;
	out.reserve(static_cast<size_t>(rows_to_print) * (static_cast<size_t>(cols_to_print) + 1) + 1);

	for (int i = 0; i < rows_to_print; ++i) {
		// append только cols_to_print символов из map[i]
		out.append(map[i], static_cast<size_t>(cols_to_print));
		// добавляем '\n' для всех строк, кроме последней при omit_final_newline
		if (!omit_final_newline || (i + 1 < rows_to_print)) {
			out.push_back('\n');
		}
	}

	std::fwrite(out.data(), 1, out.size(), stdout);
	std::fflush(stdout);
}
