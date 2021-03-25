#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

namespace ngenius
{
	constexpr float GAMEPAD_MAX_THUMB_VALUE = 32768.f;
	constexpr float GAMEPAD_MAX_TRIGGER_VALUE = 255.f;

	enum class GamepadAxis
	{
		GAMEPAD_AXIS_THUMBRIGHT = 1
		, GAMEPAD_AXIS_THUMBLEFT = 2
		, GAMEPAD_AXIS_TRIGGERRIGHT = 4
		, GAMEPAD_AXIS_TRIGGERLEFT = 8
	};

	enum class InputSource : uint8_t
	{
		KEYBOARD, MOUSE, GAMEPAD
	};

	enum class InputState : uint8_t
	{
		NONE, PRESSED, RELEASED, HELD
	};

	enum class InputType : uint8_t
	{
		BUTTON, AXIS
	};

	struct Input
	{
		explicit Input(unsigned int key, unsigned int negativeKey, InputType type, InputSource source, int gamePadId = -1)
			: key(key), negativeKey(negativeKey), source(source), type(type), gamePadId(gamePadId)
		{}

		unsigned int key;
		unsigned int negativeKey;
		InputSource source;
		InputType type;
		int gamePadId;
	};

	struct InputData
	{
		InputState state;
		glm::vec2 value;
	};
}
