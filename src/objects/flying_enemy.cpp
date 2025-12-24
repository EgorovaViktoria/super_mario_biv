#include "flying_enemy.hpp"
#include <cmath>

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
	
	hspeed = 0.1f; 
	vspeed = 0.0f;

	left_bound = top_left.x - 40.0f;
	right_bound = top_left.x + 40.0f;
}

biv::Rect FlyingEnemy::get_rect() const noexcept {
	return {top_left, width, height};
}

biv::Speed FlyingEnemy::get_speed() const noexcept {
	return {vspeed, hspeed};
}

void FlyingEnemy::move_horizontally() noexcept {
	if (!is_active()) {
		top_left.x = -10000;
		return;
	}

	float next_x = top_left.x + hspeed;
	
	if (next_x < left_bound) {
		top_left.x = left_bound;
		hspeed = -hspeed; 
	} else if (next_x + width > right_bound) {
		top_left.x = right_bound - width;
		hspeed = -hspeed; 
	} else {
		top_left.x = next_x;
	}

	if (top_left.x < 0) top_left.x = 0;
	if (top_left.x + width > MAP_WIDTH) top_left.x = MAP_WIDTH - width;
}

void FlyingEnemy::move_vertically() noexcept {
	if (!is_active()) {
		top_left.y = -10000;
		return;
	}

	phase += freq;
	float desired_y = base_y + amplitude * std::sin(phase);

	float dy = desired_y - top_left.y;
	if (dy > MAX_VERTICAL_STEP) dy = MAX_VERTICAL_STEP;
	if (dy < -MAX_VERTICAL_STEP) dy = -MAX_VERTICAL_STEP;

	vspeed = dy;

	move_vertical_offset(dy);

	if (top_left.y < 0.0f) {
		top_left.y = 0.0f;
		base_y = top_left.y - amplitude * std::sin(phase);
	}

	if (top_left.y + height > MAP_HEIGHT) {
		top_left.y = MAP_HEIGHT - height;
	}
}

void FlyingEnemy::process_horizontal_static_collision(Rect* obj) noexcept {
	hspeed = -hspeed;
	move_horizontally();
}

void FlyingEnemy::process_mario_collision(Collisionable* mario) noexcept {
	if (mario->get_speed().v > 0 && mario->get_speed().v != V_ACCELERATION) {
		kill();
		if (auto m = dynamic_cast<Movable*>(mario)) {
			m->jump();
		}
	} else {
		mario->kill();
	}
}

void FlyingEnemy::process_vertical_static_collision(Rect* obj) noexcept {
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

	base_y = top_left.y;
}

void FlyingEnemy::move_map_left() noexcept {
	RectMapMovableAdapter::move_map_left();
	left_bound -= MapMovable::MAP_STEP;
	right_bound -= MapMovable::MAP_STEP;
}

void FlyingEnemy::move_map_right() noexcept {
	RectMapMovableAdapter::move_map_right();
	left_bound += MapMovable::MAP_STEP;
	right_bound += MapMovable::MAP_STEP;
}
