#pragma once
#include <XInput.h>
#include "Singleton.h"

namespace ngenius
{
	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		~InputManager() override = default;
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;
		
		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;
	private:
		friend class Singleton<InputManager>;
		explicit InputManager() = default;

		XINPUT_STATE m_CurrentState{};
	};

}
