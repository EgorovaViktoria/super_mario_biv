#include "qt_game_map.hpp"

namespace biv {
    // Пока пустая реализация, так как вся отрисовка идет через Window/Factory
    // Но методы должны быть определены, чтобы линковщик не ругался (vtable errors).
    
    // Если ты используешь версию класса из qt_ui_factory.hpp (вложенную), то этот файл можно оставить пустым 
    // или вообще убрать из CMake. Но для чистоты лучше определить методы:

    void QtGameMap::clear() noexcept {}
    void QtGameMap::refresh() noexcept {}
    void QtGameMap::remove_objs() {}
    void QtGameMap::show() const noexcept {}
    bool QtGameMap::is_below_map(const int y) const noexcept {
      return y >= height;
    }
}
