#pragma once

#include <vector>

#include "game_map.hpp"
#include "console_ui_obj.hpp"

namespace biv {
	class ConsoleGameMap : public GameMap {
		private:
			char** map;
			
			std::vector<ConsoleUIObject*> objs;
			
			// Флаг, указывающий, что buffer был обновлён и нужно перерисовать экран.
			// Делается mutable, т.к. show() const, но должно иметь возможность сбрасывать флаг.
			mutable bool needs_draw = true;
			
		public:
			ConsoleGameMap(const int height, const int width);
			~ConsoleGameMap();
			
			void add_obj(ConsoleUIObject*);
			void clear() noexcept override;
			void refresh() noexcept override;
			void remove_obj(ConsoleUIObject*);
			void remove_objs() override;
			void show() const noexcept override;
	};
}
