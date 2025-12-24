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
		// NB: не фильтруем объекты по is_active() здесь,
		// чтобы объекты, которые логически "опустошены" (FullBox -> '-') продолжали отрисовываться.
		// (Ранее здесь стояла dynamic_cast + is_active() проверка, из-за чего блоки исчезали).

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
// (Остальная логика show() оставлена прежней: определение терминала/viewport, omit final newline и т.д.)
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

	int rows_to_print = (term_rows > 0) ? std::min(term_rows, height) : height;
	int cols_to_print = (term_cols > 0) ? std::min(term_cols, width) : width;

	// Найдём Марио, центрируем viewport по нему, как раньше (если нужно)
	int mario_top = -1, mario_left = -1;
	for (ConsoleUIObject* o : objs) {
		if (o->get_brush() == '@') {
			if (auto coll = dynamic_cast<biv::Collisionable*>(o)) {
				if (!coll->is_active()) continue;
			}
			mario_top = o->get_top();
			mario_left = o->get_left();
			break;
		}
	}

	int row_start = 0;
	int col_start = 0;
	if (mario_top >= 0) {
		row_start = mario_top - rows_to_print / 2;
		if (row_start < 0) row_start = 0;
		if (row_start + rows_to_print > height) row_start = std::max(0, height - rows_to_print);
	}
	if (mario_left >= 0) {
		col_start = mario_left - cols_to_print / 2;
		if (col_start < 0) col_start = 0;
		if (col_start + cols_to_print > width) col_start = std::max(0, width - cols_to_print);
	}

	bool omit_final_newline = (term_rows > 0 && rows_to_print == term_rows);

	std::string out;
	out.reserve(static_cast<size_t>(rows_to_print) * (static_cast<size_t>(cols_to_print) + 1) + 1);

	for (int r = 0; r < rows_to_print; ++r) {
		int map_row = row_start + r;
		out.append(map[map_row] + col_start, static_cast<size_t>(cols_to_print));
		if (!omit_final_newline || (r + 1 < rows_to_print)) {
			out.push_back('\n');
		}
	}

	std::fwrite(out.data(), 1, out.size(), stdout);
	std::fflush(stdout);
}
