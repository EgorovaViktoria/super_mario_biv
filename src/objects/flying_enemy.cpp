#include "flying_enemy.hpp"

using biv::FlyingEnemy;

FlyingEnemy::FlyingEnemy(const Coord& top_left, const int width, const int height)
	: RectMapMovableAdapter(top_left, width, height) {
	// Инициализация параметров полёта
	base_y = top_left.y;
	amplitude = 2.0f;
	freq = 0.12f;
	phase = 0.0f;
	hspeed = 0.5f; // мягкая горизонтальная скорость
	vspeed = 0.0f;
}

biv::Rect FlyingEnemy::get_rect() const noexcept {
	return {top_left, width, height};
}

biv::Speed FlyingEnemy::get_speed() const noexcept {
	return {vspeed, hspeed};
}

void FlyingEnemy::move_horizontally() noexcept {
	top_left.x += hspeed;
}

void FlyingEnemy::move_vertically() noexcept {
	phase += freq;
	float desired_y = base_y + amplitude * std::sin(phase);

	// смещение от текущего положения
	float dy = desired_y - top_left.y;

	// ограничиваем шаг по Y
	if (dy > MAX_VERTICAL_STEP) dy = MAX_VERTICAL_STEP;
	if (dy < -MAX_VERTICAL_STEP) dy = -MAX_VERTICAL_STEP;

	// перемещаем относительно, а не перезаписываем y
	move_vertical_offset(dy);

	// защита от ухода за верхнюю границу
	if (top_left.y < 0.0f) {
		top_left.y = 0.0f;
		base_y = top_left.y - amplitude * std::sin(phase);
	}
}

void FlyingEnemy::process_horizontal_static_collision(Rect* obj) noexcept {
	// При горизонтальном столкновении просто меняем направление и двигаемся,
	// чтобы не "залазить" в объект.
	hspeed = -hspeed;
	move_horizontally();
}

void FlyingEnemy::process_mario_collision(Collisionable* mario) noexcept {
	// Аналогично Enemy: если Марио падает сверху — враг умирает, иначе Марио умирает.
	if (mario->get_speed().v > 0 && mario->get_speed().v != V_ACCELERATION) {
		kill();
	} else {
		mario->kill();
	}
}

void FlyingEnemy::process_vertical_static_collision(Rect* obj) noexcept {
	// Поведение, идентичное Enemy::process_vertical_static_collision:
	// Проверка: не свалился ли враг с корабля (край платформы).
	top_left.x += hspeed;
	if (!has_collision(obj)) {
		// если при сдвиге по X коллизии нет — переворачиваем направление
		process_horizontal_static_collision(obj);
	} else {
		// иначе возвращаем обратно
		top_left.x -= hspeed;
	}

	// Если есть положительная вертикальная скорость (вниз) — корректируем Y как у Enemy.
	if (vspeed > 0) {
		top_left.y -= vspeed;
		vspeed = 0;
	}

	// После корректировки привязываем базу полёта к фактической позиции,
	// чтобы не "уходить" далеко вверх после столкновения.
	base_y = top_left.y;
}
