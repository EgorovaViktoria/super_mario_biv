#pragma once

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>

// Используем относительные пути для надежности
#include "../../controller/game.hpp"
#include "../../levels/game_level.hpp"

namespace biv {

    class QtUIFactory; // Forward declaration

    class QtGameWindow : public QWidget {
        Q_OBJECT

    public:
        explicit QtGameWindow(QWidget *parent = nullptr);
        ~QtGameWindow();

        void set_factory(QtUIFactory* factory);
        void set_game(Game* game);
        
        // Добавили отсутствующий метод
        void set_level(GameLevel* level);

    protected:
        void paintEvent(QPaintEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;

    private slots:
        void game_loop();

    private:
        Game* game = nullptr;
        QtUIFactory* factory = nullptr;
        GameLevel* current_level = nullptr;
        QTimer* timer;
        
        bool key_left = false;
        bool key_right = false;
        bool key_jump = false;
    };
}
