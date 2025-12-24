#include "qt_game_map.hpp"
#include "qt_key_mapper.hpp"
#include <QPainter>
#include <QApplication>
#include <algorithm>

using biv::QtGameMap;

QtGameMap::QtGameMap(const int height, const int width, QWidget* parent) 
	: QWidget(parent), GameMap(height, width) {
    setFixedSize(width * CELL_SIZE, height * CELL_SIZE);
}

QtGameMap::~QtGameMap() {
}

void QtGameMap::add_obj(QtUIObject* obj) {
	objs.push_back(obj);
}

void QtGameMap::clear() noexcept {
}

void QtGameMap::refresh() noexcept {
    update();
}

void QtGameMap::remove_obj(QtUIObject* obj) {
	objs.erase(std::remove(objs.begin(), objs.end(), obj), objs.end());
}

void QtGameMap::remove_objs() {
	objs.clear();
}

void QtGameMap::show() const noexcept {
    const_cast<QtGameMap*>(this)->setVisible(true);
    const_cast<QtGameMap*>(this)->update();
    QApplication::processEvents();
}

biv::os::UserInput QtGameMap::get_input() {
    return current_input;
}

void QtGameMap::keyPressEvent(QKeyEvent* event) {
    if (auto input = biv::os::unix::map_qt_key(event->key())) {
        current_input = *input;
    }
}

void QtGameMap::keyReleaseEvent(QKeyEvent* event) {
    if (auto input = biv::os::unix::map_qt_key(event->key())) {
        if (current_input == *input) {
            current_input = biv::os::UserInput::NO_INPUT;
        }
    }
}

void QtGameMap::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::cyan); // Sky background
    
    painter.scale(CELL_SIZE, CELL_SIZE);
    
    // Draw sea/ground (bottom 3 rows)
    painter.fillRect(0, GameMap::height - 3, GameMap::width, 3, Qt::blue);
    
    for (QtUIObject* obj : objs) {
        if (obj->is_active()) {
            painter.save();
            obj->paint(painter);
            painter.restore();
        }
    }
}
