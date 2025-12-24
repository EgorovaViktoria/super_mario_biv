#include "qt_ui_factory.hpp"
#include "../../objects/mario.hpp"
#include "../../objects/enemy.hpp"
#include "../../objects/flying_enemy.hpp"
#include "../../objects/jumping_enemy.hpp"
#include "../../objects/box.hpp"
#include "../../objects/full_box.hpp"
#include "../../objects/money.hpp"
#include "../../objects/ship.hpp"

namespace biv {

    QtUIFactory::QtUIFactory(Game* game) : UIFactory(game) {
        create_game_map();
    }
    
    // Реализация деструктора
    QtUIFactory::~QtUIFactory() {
        if (map) delete map;
        // Объекты рендера не удаляем здесь, так как ими владеет Game (обычно)
        // Или если Factory владеет, нужно пройтись и удалит
ь.
        // В текущей архитектуре ownership не совсем ясен, оставим как есть.
    }

    void QtUIFactory::create_game_map() {
        if (!map) map = new QtGameMap(10, 100); 
    }

    GameMap* QtUIFactory::get_game_map() { return map; }
    Mario* QtUIFactory::get_mario() { return mario; }

    void QtUIFactory::clear_data() {
        render_list.clear();
        game->remove_objs();
        mario = nullptr;
    }

    void QtUIFactory::draw_all(QPainter& p) {
        for (const auto& obj : render_list) {
            if (!obj.rect) continue; // Защита

            float w = obj.rect->get_right() - obj.rect->get_left();
            float h = obj.rect->get_bottom() - obj.rect->get_top();
            
            QRectF r(
                obj.rect->get_left() * scale, 
                obj.rect->get_top() * scale,
                w * scale,
                h * scale
            );
            
            p.fillRect(r, obj.color);
            p.setPen(QColor(0, 0, 0, 100));
            p.drawRect(r);
        }
    }

    void QtUIFactory::create_mario(const Coord& tl, int w, int h) {
        mario = new Mario(tl, w, h);
        render_list.push_back(RenderObject{dynamic_cast<Rect*>(mario), Qt::red});
        game->add_mario(mario);
        game->add_movable(mario);
    }

    void QtUIFactory::create_enemy(const Coord& tl, int w, int h) {
        auto* obj = create_base<Enemy>(tl, w, h, QColor(139, 69, 19)); 
        game->add_collisionable(obj);
        game->add_movable(obj);
        game->add_map_movable(obj);
    }

    void QtUIFactory::create_ship(const Coord& tl, int w, int h) {
        // Создаем через create_base (он добавит в render_list)
        auto* obj = create_base<Ship>(tl, w, h, Qt::darkGreen);
        
        // ВАЖНО: Добавляем в список статических объектов для коллизий
        game->add_static_obj(obj);
        
        // Добавляем, чтобы он двигался вместе с картой
        game->add_map_movable(obj);
    }

    void QtUIFactory::create_box(const Coord& tl, int w, int h) {
        auto* obj = create_base<Box>(tl, w, h, Qt::yellow);
        game->add_static_obj(obj);
        game->add_map_movable(obj);
    }

    void QtUIFactory::create_full_box(const Coord& tl, int w, int h) {
        auto* obj = new FullBox(tl, w, h, this);
        render_list.push_back(RenderObject{dynamic_cast<Rect*>(obj), QColor(205, 133, 63)});
        game->add_static_obj(obj);
        game->add_map_movable(obj);
    }
    
    void QtUIFactory::create_money(const Coord& tl, int w, int h) {
        auto* obj = new Money(tl, w, h);
        render_list.push_back(RenderObject{dynamic_cast<Rect*>(obj), QColor(255, 215, 0)});
        game->add_map_movable(obj);
    }

    void QtUIFactory::create_flying_enemy(const Coord& tl, int w, int h) {
        auto* obj = create_base<FlyingEnemy>(tl, w, h, Qt::magenta);
        game->add_collisionable(obj);
        game->add_movable(obj);
        game->add_map_movable(obj);
    }

    void QtUIFactory::create_jumping_enemy(const Coord& tl, int w, int h) {
        auto* obj = create_base<JumpingEnemy>(tl, w, h, Qt::darkRed);
        game->add_collisionable(obj);
        game->add_movable(obj);
        game->add_map_movable(obj);
    }
}
