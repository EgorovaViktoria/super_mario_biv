#pragma once

#include "collisionable.hpp"
#include "movable.hpp"
#include "rect.hpp"
#include "rect_map_movable_adapter.hpp"
#include "speed.hpp"
#include "coord.hpp"

namespace biv {
	class JumpingEnemy : public RectMapMovableAdapter, public Movable, public Collisionable {
		private:
			int jump_interval = 60; 
			int timer = 0;

			bool on_ground = false;
			int ground_left = -1;
			int ground_right = -1;

			static constexpr int MAP_WIDTH = 200;
			static constexpr int MAP_HEIGHT = 30;

			static constexpr float JUMP_SPEED_ENEMY = -0.7f;

		public:
			JumpingEnemy(const Coord& top_left, const int width, const int height);

			biv::Rect get_rect() const noexcept override;
			biv::Speed get_speed() const noexcept override;

			void move_horizontally() noexcept override;
			void move_vertically() noexcept override;

			void process_horizontal_static_collision(Rect* obj) noexcept override;
			void process_vertical_static_collision(Rect* obj) noexcept override;
			void process_mario_collision(Collisionable* mario) noexcept override;
	};
}
