#include "jumping_enemy.hpp"
#include <cstdlib>

using biv::JumpingEnemy;

static inline bool rects_collide(const biv::Rect& a, const biv::Rect& b) noexcept {
	return (
		a.get_right() > b.get_left() &&
		a.get_left()  < b.get_right() &&
		a.get_bottom() > b.get_top() &&
		a.get_top()   < b.get_bottom()
	);
}

JumpingEnemy::JumpingEnemy(const Coord& top_left, const int width, const int height)
	: RectMapMovableAdapter(top_left, width, height) {
	jump_interval = 60;
	timer = jump_interval / 2;
	hspeed = 0.2f;
	vspeed = 0.0f;
	on_ground = false;
	ground_left = -1;
	ground_right = -1;
}

biv::Rect JumpingEnemy::get_rect() const noexcept {
	return {top_left, width, height};
}

biv::Speed JumpingEnemy::get_speed() const noexcept {
	return {vspeed, hspeed};
}

void JumpingEnemy::move_horizontally() noexcept {
	if (!is_active()) {
		top_left.x = -10000;
		return;
	}

	float next_x = top_left.x + hspeed;
	if (on_ground) {
		float left_after = next_x;
		float right_after = next_x + width;
		if (left_after < ground_left || right_after > ground_right) {
			hspeed = -hspeed;
			return;
		}
		top_left.x = next_x;
	} else {
		Movable::move_horizontally();
	}

	if (top_left.x < 0) top_left.x = 0;
	if (top_left.x + width > MAP_WIDTH) top_left.x = MAP_WIDTH - width;
}

void JumpingEnemy::move_vertically() noexcept {
	if (!is_active()) {
		top_left.y = -10000;
		return;
	}

	if (timer <= 0) {
		vspeed = JUMP_SPEED_ENEMY;
		timer = jump_interval;
	} else {
		timer--;
	}
	Movable::move_vertically();

	if (top_left.y < 0) top_left.y = 0;
	if (top_left.y + height > MAP_HEIGHT) top_left.y = MAP_HEIGHT - height;
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
	top_left.x += hspeed;
	if (!rects_collide(get_rect(), *obj)) {
		process_horizontal_static_collision(obj);
	} else {
		top_left.x -= hspeed;
	}

	if (vspeed > 0) { 
		top_left.y = obj->get_top() - height;
		vspeed = 0;
	}

	ground_left = obj->get_left();
	ground_right = obj->get_right();
	on_ground = true;
}
