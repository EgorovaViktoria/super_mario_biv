#pragma once

#include <optional>
#include <QKeyEvent>
#include "user_input.hpp"

namespace biv::os::unix {
    std::optional<UserInput> map_qt_key(int key);
}
