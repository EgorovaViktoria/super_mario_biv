#pragma once

#include "qt_ui_obj.hpp"
#include "rect.hpp"

namespace biv {
	class QtUIObjectRectAdapter : virtual public Rect, public QtUIObject {
		public:
			QtUIObjectRectAdapter() = default;
			QtUIObjectRectAdapter(
				const Coord& top_left, const int width, const int height
			);
	};
}
