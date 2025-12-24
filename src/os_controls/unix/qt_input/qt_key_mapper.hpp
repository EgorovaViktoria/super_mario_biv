#pragma once

#include <optional>
#include <QKeyEvent>
#include "user_input.hpp"

namespace biv {
namespace os {
namespace qt_os {
    std::optional<UserInput> map_qt_key(int key);
}
}
}
