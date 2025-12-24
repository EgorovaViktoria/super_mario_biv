#include "qt_game_window.hpp"
#include "qt_ui_factory.hpp"
#include <QDebug>
#include <QApplication>

namespace biv {

    QtGameWindow::QtGameWindow(QWidget *parent) : QWidget(parent) {
        setFixedSize(1200, 600);
        setWindowTitle("Super Mario Qt6");
        
        QPalette pal = palette();
        pal.setColor(QPalette::Window, Qt::black);
        setAutoFillBackground(true);
        setPalette(pal);

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &QtGameWindow::game_loop);
        
        // ВАЖНО: 30 мс = ~33 FPS. Это скорость оригинальной игры.
        // При такой скорости физика работает корректно и Марио не проваливается.
        timer->start(30); 
    }

    QtGameWindow::~QtGameWindow() {
        if (timer->isActive()) timer->stop();
    }

    void QtGameWindow::set_factory(QtUIFactory *f) { this->factory = f; }
    void QtGameWindow::set_game(Game *g) { this->game = g; }
    void QtGameWindow::set_level(GameLevel *l) { this->current_level = l; }

    void QtGameWindow::keyPressEvent(QKeyEvent *event) {
        switch (event->key()) {
            case Qt::Key_Left: key_left = true; break;
            case Qt::Key_Right: key_right = true; break;
            case Qt::Key_Space: 
            case Qt::Key_Up:    key_jump = true; break;
            case Qt::Key_Escape: QApplication::quit(); break;
        }
    }

    void QtGameWindow::keyReleaseEvent(QKeyEvent *event) {
        switch (event->key()) {
            case Qt::Key_Left: key_left = false; break;
            case Qt::Key_Right: key_right = false; break;
            case Qt::Key_Space: 
            case Qt::Key_Up:    key_jump = false; break;
        }
    }

    void QtGameWindow::game_loop() {
    if (!game || !factory) return;
    auto mario = factory->get_mario();
    if (!mario) return;

    // 1. Управление - только установка направления
    if (key_jump && mario->is_on_ground()) {
        mario->jump();
    }
    
    // Устанавливаем направление движения
    if (key_left) {
        mario->set_direction_left();
    } else if (key_right) {
        mario->set_direction_right();
    } else {
        mario->stop_horizontal();
    }

    // 2. Физика - ВАЖЕН ПОРЯДОК!
    
    // Сначала применяем гравитацию
    game->apply_gravity();
    
    // Проверяем вертикальные коллизии (земля/потолок)
    game->check_vertically_static_collisions();
    
    // Затем горизонтальное движение
    game->move_objs_horizontally();
    
    // Проверяем горизонтальные коллизии
    game->check_horizontally_static_collisions();
    
    // Проверяем коллизии с врагами
    game->check_mario_collision();
    
    // Обновляем состояние Марио (на земле ли)
    game->update_mario_ground_state();

    // 3. Движение карты относительно Марио
    // Карта должна двигаться, когда Марио достигает определенной позиции
    int mario_x = mario->get_center_x();
    int screen_center_x = width() / 2;
    
    if (mario_x > screen_center_x && mario->is_moving_right()) {
        int move_by = mario_x - screen_center_x;
        game->move_map_left(move_by);
    } else if (mario_x < screen_center_x && mario->is_moving_left()) {
        int move_by = screen_center_x - mario_x;
        game->move_map_right(move_by);
    }

    // 4. Проверка смерти
    if (factory->get_game_map()->is_below_map(mario->get_bottom()) || !mario->is_active()) {
        if (current_level) {
            current_level->restart();
            return;
        }
    }
    
    if (game->is_level_end()) {
        game->finish();
    }

    // 5. Отрисовка
    update();
}
        
        if (game->is_level_end()) {
             game->finish();
        }

        // 4. Отрисовка
        update(); 
    }

    void QtGameWindow::paintEvent(QPaintEvent *) {
        QPainter painter(this);
        if (factory) {
            factory->draw_all(painter);
        }
    }
}
