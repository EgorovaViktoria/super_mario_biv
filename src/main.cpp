#include <QApplication>
#include "ui/qt/qt_game_window.hpp"
#include "ui/qt/qt_ui_factory.hpp"
#include "levels/first_level.hpp"
#include "controller/game.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Инициализация логики игры
    biv::Game game;
    
    // Создаем Qt-фабрику (она будет создавать визуальные объекты)
    auto* ui_factory = new biv::QtUIFactory(&game);
    
    // Загружаем первый уровень
    // Уровень сам наполнит игру объектами через фабрику
    auto* level = new biv::FirstLevel(ui_factory); 

    // Создаем и показываем главное окно
    biv::QtGameWindow window;
    window.set_factory(ui_factory);
    window.set_game(&game);
    window.set_level(level); // Передаем уровень, чтобы управлять рестартом/сменой
    
    window.show();

    return app.exec();
}
