#pragma once

namespace PK2::MapUtil {
	constexpr int TILE_SIZE = 32;

	int alignToGrid(int position) {
		return (position / TILE_SIZE) * TILE_SIZE;
	}

	/*
		Turns a map position into a screen position.

		Example:
			X Position on map:		15
			X Position on screen:	480
	*/
	int mapPositionToScreen(int position) {
		return position * TILE_SIZE;
	}

	int screenPositionToMap(int position) {
		return position / TILE_SIZE;
	}
}