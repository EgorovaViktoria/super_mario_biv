#include "qt_enemy.hpp"

using biv::QtEnemy;

QtEnemy::QtEnemy(const Coord& top_left, const int width, const int height)
    : Enemy(top_left, width, height), QtUIObjectRectAdapter(top_left, width, height) {}

void QtEnemy::paint(QPainter& painter) const {
    painter.setBrush(QColor(139, 69, 19)); // Brown
    painter.setPen(Qt::black);
    painter.drawRect(get_left(), get_top(), get_width(), get_height());
}
