#include "qt_flying_enemy.hpp"

using biv::QtFlyingEnemy;

QtFlyingEnemy::QtFlyingEnemy(const Coord& top_left, const int width, const int height, Game* game)
    : FlyingEnemy(top_left, width, height, game), QtUIObjectRectAdapter(top_left, width, height) {}

void QtFlyingEnemy::paint(QPainter& painter) const {
    painter.setBrush(Qt::magenta);
    painter.setPen(Qt::black);
    painter.drawRect(get_left(), get_top(), get_width(), get_height());
}
