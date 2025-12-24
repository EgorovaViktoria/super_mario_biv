#pragma once

#include <QPainter>

namespace biv {
	class QtUIObject {
		public:
			virtual void paint(QPainter& painter) const = 0;
			virtual bool is_active() const noexcept { return true; }
	};
}
