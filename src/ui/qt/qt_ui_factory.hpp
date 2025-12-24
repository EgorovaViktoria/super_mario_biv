#pragma once

#include <vector>
#include <QPainter>
#include "../../model/ui_factory.hpp"
#include "qt_game_map.hpp"
#include "../../model/rect.hpp" // Чтобы dynamic_cast работал корректно

namespace biv {

    struct RenderObject {
        Rect* rect;
        QColor color;
    };

    class QtUIFactory : public UIFactory {
    public:
        QtUIFactory(Game* game);
        ~QtUIFactory(); // Объявили
        
        void clear_data() override;
        
        void create_box(const Coord&, const int, const int) override;
        void create_enemy(const Coord&, const int, const int) override;
        void create_full_box(const Coord&, const int, const int) override;
        void create_mario(const Coord&, const int, const int) override;
        void create_money(const Coord&, const int, const int) override;
        void create_ship(const Coord&, const int, const int) override;
        void create_flying_enemy(const Coord&, const int, const int) override;
        void create_jumping_enemy(const Coord&, const int, const int) override;

        GameMap* get_game_map() override;
        Mario* get_mario() override;
        
        void draw_all(QPainter& p);

    protected:
        void create_game_map() override;

    private:
        QtGameMap* map = nullptr;
        Mario* mario = nullptr;
        std::vector<RenderObject> render_list;
        float scale = 20.0f; 

        // Реализация шаблона прямо в хедере
        template<typename T>
        T* create_base(const Coord& tl, int w, int h, QColor c) {
            T* obj = new T(tl, w, h);
            // Явное приведение к Rect*
            render_list.push_back(RenderObject{dynamic_cast<Rect*>(obj), c});
            return obj;
        }
    };
}
