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
			// Параметры полёта
			float base_y = 0.0f;
			float amplitude = 2.0f;
			float phase = 0.0f;
			float freq = 0.12f; // прирост фазы за тик

			// Ограничение шага по Y, чтобы избежать резких "вспархиваний"
			static constexpr float MAX_VERTICAL_STEP = 0.7f;

			// Флаг: стоим ли мы на платформе
			bool on_ground = false;
			// Границы последней платформы (в координатах X)
			int ground_left = -1;
			int ground_right = -1;

			// Защита по размерам карты (при необходимости подправь)
			static constexpr int MAP_WIDTH = 200;
			static constexpr int MAP_HEIGHT = 30;

		public:
			FlyingEnemy(const Coord& top_left, const int width, const int height);

			// Интерфейсы Collisionable/Movable/Rect
			biv::Rect get_rect() const noexcept override;
			biv::Speed get_speed() const noexcept override;

			// Передвижение
			void move_horizontally() noexcept override;
			void move_vertically() noexcept override;

			// Обработка коллизий
			void process_horizontal_static_collision(Rect* obj) noexcept override;
			void process_vertical_static_collision(Rect* obj) noexcept override;
			void process_mario_collision(Collisionable* mario) noexcept override;

			// Обновление при сдвиге карты — сдвигаем границы платформы синхронно
			void move_map_left() noexcept override;
			void move_map_right() noexcept override;
	};
}
