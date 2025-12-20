#pragma once

#include "game_level.hpp"

namespace biv {
	class ThirdLevel : public GameLevel {
		public:
			ThirdLevel(UIFactory* ui_factory);
			GameLevel* get_next() override;

			// Соответствует сигнатуре в GameLevel (noexcept)
			bool is_final() const noexcept override;

		private:
			void init_data() override;
	};
}
