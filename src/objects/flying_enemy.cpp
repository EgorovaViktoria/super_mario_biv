#include "flying_enemy.hpp"

using biv::FlyingEnemy;

static inline bool rects_collide(const biv::Rect& a, const biv::Rect& b) noexcept {
	return (
		a.get_right() > b.get_left() &&
		a.get_left()  < b.get_right() &&
		a.get_bottom() > b.get_top() &&
		a.get_top()   < b.get_bottom()
	);
}

FlyingEnemy::FlyingEnemy(const Coord& top_left, const int width, const int height)
	: RectMapMovableAdapter(top_left, width, height) {
	base_y = top_left.y;
	amplitude = 2.0f;
	freq = 0.12f;
	phase = 0.0f;
	hspeed = 0.5f;
	vspeed = 0.0f;
	on_ground = false;
	ground_left = -1;
	ground_right = -1;
}

biv::Rect FlyingEnemy::get_rect() const noexcept {
	return {top_left, width, height};
}

biv::Speed FlyingEnemy::get_speed() const noexcept {
	return {vspeed, hspeed};
}

void FlyingEnemy::move_horizontally() noexcept {
	// Если знаем платформу, патрулируем строго над ней
	if (on_ground && ground_left >= 0 && ground_right > ground_left) {
		// цельная следующая позиция
		float next_x = top_left.x + hspeed;
		float left_after = next_x;
		float right_after = next_x + width;

		// Если платформа уже меньше по ширине, не даём врагу выйти за платформу:
		int available_width = ground_right - ground_left;
		if (available_width <= width) {
			// ставим врага на центр или на левую границу и разворачиваемся
			if (top_left.x < ground_left) top_left.x = ground_left;
			if (top_left.x + width > ground_right) top_left.x = ground_right - width;
			hspeed = -hspeed;
			return;
		}

		// Если шаг выведет за левую границу — зафиксировать на границе и развернуться
		if (left_after < ground_left) {
			top_left.x = ground_left;
			hspeed = -hspeed;
			return;
		}

		// Если шаг выведет за правую границу — зафиксировать и развернуться
		if (right_after > ground_right) {
			top_left.x = ground_right - width;
			hspeed = -hspeed;
			return;
		}

		// Иначе движение безопасно
		top_left.x = next_x;
	} else {
		// fallback: обычное движение, двигаться и полагаться на вертикальные проверки
		Movable::move_horizontally();
	}

	// защита по краям карты
	if (top_left.x < 0) top_left.x = 0;
	if (top_left.x + width > MAP_WIDTH) top_left.x = MAP_WIDTH - width;
}

void FlyingEnemy::move_vertically() noexcept {
	phase += freq;
	float desired_y = base_y + amplitude * std::sin(phase);

	float dy = desired_y - top_left.y;
	if (dy > MAX_VERTICAL_STEP) dy = MAX_VERTICAL_STEP;
	if (dy < -MAX_VERTICAL_STEP) dy = -MAX_VERTICAL_STEP;

	// перемещаем относительно
	move_vertical_offset(dy);

	// защита от ухода за верхнюю границу
	if (top_left.y < 0.0f) {
		top_left.y = 0.0f;
		base_y = top_left.y - amplitude * std::sin(phase);
	}

	// защита от ухода за низ карты
	if (top_left.y + height > MAP_HEIGHT) {
		top_left.y = MAP_HEIGHT - height;
	}
}

void FlyingEnemy::process_horizontal_static_collision(Rect* obj) noexcept {
	hspeed = -hspeed;
	// делаем шаг назад/в сторону, чтобы не врезаться в объект
	move_horizontally();
}

void FlyingEnemy::process_mario_collision(Collisionable* mario) noexcept {
	// Аналогично Enemy: если Марио падает сверху — враг умирает, иначе Марио умирает.
	if (mario->get_speed().v > 0 && mario->get_speed().v != V_ACCELERATION) {
		kill();
		// безопасно попытаться вызвать jump(), если объект также Movable (проверка через dynamic_cast)
		if (auto m = dynamic_cast<Movable*>(mario)) {
			m->jump();
		}
	} else {
		mario->kill();
	}
}

void FlyingEnemy::process_vertical_static_collision(Rect* obj) noexcept {
	// Проверка "края платформы" и корректировка как в Enemy
	top_left.x += hspeed;
	if (!rects_collide(get_rect(), *obj)) {
		process_horizontal_static_collision(obj);
	} else {
		top_left.x -= hspeed;
	}

	if (vspeed > 0) {
		top_left.y -= vspeed;
		vspeed = 0;
	}

	// Запоминаем границы платформы, чтобы патрулировать над ней
	ground_left = obj->get_left();
	ground_right = obj->get_right();
	on_ground = true;

	// Привязываем базовую высоту полёта к фактической позиции
	base_y = top_left.y;
}

void FlyingEnemy::move_map_left() noexcept {
	RectMapMovableAdapter::move_map_left();
	// сдвигаем границы платформы вместе с картой
	if (ground_left >= 0) {
		ground_left -= MapMovable::MAP_STEP;
		ground_right -= MapMovable::MAP_STEP;
	}
}

void FlyingEnemy::move_map_right() noexcept {
	RectMapMovableAdapter::move_map_right();
	if (ground_left >= 0) {
		ground_left += MapMovable::MAP_STEP;
		ground_right += MapMovable::MAP_STEP;
	}
}
