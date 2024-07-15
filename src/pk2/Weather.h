#pragma once
namespace PK2 {
	/*
		Note: The use of enum instead of enum class is intentional here.

		This way it's easier to convert the stored weather index to the enum.
		See LegacyMapFileHandler for an example.
	*/
	enum Weather {
		NORMAL = 0,
		RAIN,
		LEAVES,
		RAIN_AND_LEAVES,
		SNOW
	};
}