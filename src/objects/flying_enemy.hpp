#pragma once

#include "enemy.hpp"
#include <cmath>

namespace biv {
	class FlyingEnemy : public Enemy {
		private:
			float base_y = 0.0f;
			float amplitude = 2.0f;
			float phase = 0.0f;
			float freq = 0.12f; // phase increment per tick

		public:
			FlyingEnemy(const Coord& top_left, const int width, const int height);

			// override movement
			void move_horizontally() noexcept override;
			void move_vertically() noexcept override;
	};
}