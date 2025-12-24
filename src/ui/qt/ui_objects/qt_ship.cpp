#include "qt_ship.hpp"

using biv::QtShip;

QtShip::QtShip(const Coord& top_left, const int width, const int height)
    : Ship(top_left, width, height), QtUIObjectRectAdapter(top_left, width, height) {}

void QtShip::paint(QPainter& painter) const {
    painter.setBrush(Qt::darkGreen);
    painter.setPen(Qt::black);
    painter.drawRect(get_left(), get_top(), get_width(), get_height());
}
