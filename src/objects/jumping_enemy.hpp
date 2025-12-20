#pragma once

#include "enemy.hpp"

namespace biv {
	class JumpingEnemy : public Enemy {
		private:
			int jump_interval = 60; // frames between jumps
			int timer = 0;
		public:
			JumpingEnemy(const Coord& top_left, const int width, const int height);

			void move_horizontally() noexcept override;
			void move_vertically() noexcept override;
	};
}