#pragma once
#include "../../model/game_map.hpp"

namespace biv {
    class QtGameMap : public GameMap {
    public:
        QtGameMap(int h, int w) : GameMap(h, w) {}
        
        void clear() noexcept override;
        void refresh() noexcept override;
        void remove_objs() override;
        void show() const noexcept override;
        bool is_below_map(const int y) const noexcept;
    };
}
