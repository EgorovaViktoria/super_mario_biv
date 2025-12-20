#include "flying_enemy.hpp"

using biv::FlyingEnemy;

FlyingEnemy::FlyingEnemy(const Coord& top_left, const int width, const int height)
	: Enemy(top_left, width, height) {
	// reasonable defaults
	base_y = top_left.y;
	amplitude = 2.0f;
	freq = 0.12f;
	phase = 0.0f;
	hspeed = 0.5f; // gentle horizontal speed
	vspeed = 0.0f;
}

void FlyingEnemy::move_horizontally() noexcept {
	top_left.x += hspeed;
}

void FlyingEnemy::move_vertically() noexcept {
	phase += freq;
	top_left.y = base_y + amplitude * std::sin(phase);
}