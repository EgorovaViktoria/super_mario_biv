#include "jumping_enemy.hpp"

using biv::JumpingEnemy;

JumpingEnemy::JumpingEnemy(const Coord& top_left, const int width, const int height)
	: Enemy(top_left, width, height) {
	jump_interval = 60;
	timer = jump_interval / 2; // begin with half delay
	hspeed = 0.2f;
	vspeed = 0.0f;
}

void JumpingEnemy::move_horizontally() noexcept {
	top_left.x += hspeed;
}

void JumpingEnemy::move_vertically() noexcept {
	// gravity handled in Movable::move_vertically, but trigger periodic jump
	if (timer <= 0) {
		// perform jump
		jump();
		timer = jump_interval;
	} else {
		timer--;
	}
	// then apply vertical movement (gravity)
	Movable::move_vertically();
}