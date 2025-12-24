#include "qt_key_mapper.hpp"

namespace biv::os::unix {
    std::optional<UserInput> map_qt_key(int key) {
        switch (key) {
            case Qt::Key_A:
            case Qt::Key_Left:
                return UserInput::MAP_LEFT;
            case Qt::Key_D:
            case Qt::Key_Right:
                return UserInput::MAP_RIGHT;
            case Qt::Key_Space:
            case Qt::Key_Up:
                return UserInput::MARIO_JUMP;
            case Qt::Key_Escape:
                return UserInput::EXIT;
            default:
                return std::nullopt;
        }
    }
}
