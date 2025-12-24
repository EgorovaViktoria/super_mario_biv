#pragma once

#include "collisionable.hpp"
#include "movable.hpp"
#include "rect.hpp"
#include "rect_map_movable_adapter.hpp"
#include "speed.hpp"
#include "coord.hpp"
#include <cmath>

namespace biv {
	class FlyingEnemy : public RectMapMovableAdapter, public Movable, public Collisionable {
		private:
			float base_y = 0.0f;
			float amplitude = 2.0f;
			float phase = 0.0f;
			float freq = 0.12f;

			static constexpr float MAX_VERTICAL_STEP = 0.7f;

			float left_bound = 0.0f;
			float right_bound = 0.0f;
			
			static constexpr int MAP_WIDTH = 200;
			static constexpr int MAP_HEIGHT = 30;

		public:
			FlyingEnemy(const Coord& top_left, const int width, const int height);

			biv::Rect get_rect() const noexcept override;
			biv::Speed get_speed() const noexcept override;

			void move_horizontally() noexcept override;
			void move_vertically() noexcept override;

			void process_horizontal_static_collision(Rect* obj) noexcept override;
			void process_vertical_static_collision(Rect* obj) noexcept override;
			void process_mario_collision(Collisionable* mario) noexcept override;

			void move_map_left() noexcept override;
			void move_map_right() noexcept override;
	};
}
