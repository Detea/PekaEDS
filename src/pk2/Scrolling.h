#pragma once
namespace PK2 {
	// The use of enum instead of enum class is intentional. For more details see Weather.h
	enum Scrolling {
		STATIC = 0,
		PARALLAX_VERTICAL,
		PARALLAX_HORIZONTAL,
		PARALLAX_VERTICAL_AND_HORIZONTAL
	};
}