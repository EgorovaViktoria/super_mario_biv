#include "jumping_enemy.hpp"

using biv::JumpingEnemy;

JumpingEnemy::JumpingEnemy(const Coord& top_left, const int width, const int height)
	: RectMapMovableAdapter(top_left, width, height) {
	jump_interval = 60;
	timer = jump_interval / 2; // начать с половины интервала
	hspeed = 0.2f;
	vspeed = 0.0f;
}

biv::Rect JumpingEnemy::get_rect() const noexcept {
	return {top_left, width, height};
}

biv::Speed JumpingEnemy::get_speed() const noexcept {
	return {vspeed, hspeed};
}

void JumpingEnemy::move_horizontally() noexcept {
	top_left.x += hspeed;
}

void JumpingEnemy::move_vertically() noexcept {
	// таймер прыжков: когда наступает момент — триггерим jump()
	if (timer <= 0) {
		jump();
		timer = jump_interval;
	} else {
		timer--;
	}
	// затем применяем базовую физику вертикального движения
	Movable::move_vertically();
}

void JumpingEnemy::process_horizontal_static_collision(Rect* obj) noexcept {
	hspeed = -hspeed;
	move_horizontally();
}

void JumpingEnemy::process_mario_collision(Collisionable* mario) noexcept {
	if (mario->get_speed().v > 0 && mario->get_speed().v != V_ACCELERATION) {
		kill();
	} else {
		mario->kill();
	}
}

void JumpingEnemy::process_vertical_static_collision(Rect* obj) noexcept {
	// Как у Enemy: проверка края платформы и корректировка vertical speed
	top_left.x += hspeed;
	if (!has_collision(obj)) {
		process_horizontal_static_collision(obj);
	} else {
		top_left.x -= hspeed;
	}

	if (vspeed > 0) {
		top_left.y -= vspeed;
		vspeed = 0;
	}
}
