#include "qt_full_box.hpp"

using biv::QtFullBox;

QtFullBox::QtFullBox(const Coord& top_left, const int width, const int height, UIFactory* factory)
    : FullBox(top_left, width, height, factory), QtUIObjectRectAdapter(top_left, width, height) {}

void QtFullBox::paint(QPainter& painter) const {
    painter.setBrush(QColor(205, 133, 63)); // Peru/tan color
    painter.setPen(Qt::black);
    painter.drawRect(get_left(), get_top(), get_width(), get_height());
}
