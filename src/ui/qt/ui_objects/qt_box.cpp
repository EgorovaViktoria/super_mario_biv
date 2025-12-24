#include "qt_box.hpp"

using biv::QtBox;

QtBox::QtBox(const Coord& top_left, const int width, const int height)
    : Box(top_left, width, height), QtUIObjectRectAdapter(top_left, width, height) {}

void QtBox::paint(QPainter& painter) const {
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::black);
    painter.drawRect(get_left(), get_top(), get_width(), get_height());
}
