#include "qt_ui_obj_rect_adapter.hpp"

using biv::QtUIObjectRectAdapter;

QtUIObjectRectAdapter::QtUIObjectRectAdapter(
	const Coord& top_left, const int width, const int height
) : Rect(top_left, width, height) {}
