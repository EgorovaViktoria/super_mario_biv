#include "qt_mario.hpp"

using biv::QtMario;

QtMario::QtMario(const Coord& top_left, const int width, const int height)
    : Mario(top_left, width, height), QtUIObjectRectAdapter(top_left, width, height) {}

void QtMario::paint(QPainter& painter) const {
    painter.setBrush(Qt::red);
    painter.setPen(Qt::black);
    painter.drawRect(get_left(), get_top(), get_width(), get_height());
}
