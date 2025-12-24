#pragma once

#include "jumping_enemy.hpp"
#include "qt_ui_obj_rect_adapter.hpp"

namespace biv {
    class QtJumpingEnemy : public JumpingEnemy, public QtUIObjectRectAdapter {
        public:
            QtJumpingEnemy(const Coord& top_left, const int width, const int height);
            void paint(QPainter& painter) const override;
    };
}
