#pragma once

#include <QWidget>
#include <vector>
#include <QKeyEvent>
#include "game_map.hpp"
#include "qt_ui_obj.hpp"
#include "user_input.hpp"

namespace biv {
	class QtGameMap : public QWidget, public GameMap {
		Q_OBJECT
		
		private:
			std::vector<QtUIObject*> objs;
            static constexpr int CELL_SIZE = 20;
            biv::os::UserInput current_input = biv::os::UserInput::NO_INPUT;

		public:
			QtGameMap(const int height, const int width, QWidget* parent = nullptr);
			~QtGameMap();

			void add_obj(QtUIObject* obj);
			
			void clear() noexcept override;
			void refresh() noexcept override;
			void remove_objs() override;
			void remove_obj(QtUIObject* obj);
			
			void show() const noexcept override; 
            
            biv::os::UserInput get_input();

        private:
            void paintEvent(QPaintEvent* event) override;
            void keyPressEvent(QKeyEvent* event) override;
            void keyReleaseEvent(QKeyEvent* event) override;
	};
}
