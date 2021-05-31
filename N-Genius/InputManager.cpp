#include "PCH.h"
#include "InputManager.h"
#include <SDL.h>

bool ngenius::InputManager::ProcessInput()
{
	std::for_each(std::begin(m_ControllerMap), std::end(m_ControllerMap), [](ControllerMap::value_type& cPair)
	{
		ZeroMemory(&cPair.second, sizeof(XINPUT_STATE));
		XInputGetState(cPair.first, &cPair.second);
	});

	[[maybe_unused]] auto a = GetKeyboardState(m_KeyboardState);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {

		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
	}

	std::for_each(std::begin(m_InputBindings), std::end(m_InputBindings), [this](InputBindings::value_type& bindingPair)
		{
			ProcessInput(bindingPair.second);
		});

	return true;
}

void ngenius::InputManager::ProcessInput(InputBinding& binding) const
{
	bool axisTrigger{ false };
	InputData inputData{ InputState::NONE, {} };
	for (const Input& input : binding.inputs)
	{
		switch(input.type)
		{
		case InputType::BUTTON:
			binding.prevState = GetButtonStateChange(input, binding.prevState);
			if (binding.prevState != InputState::NONE)
			{
				inputData.state = binding.prevState;
				binding.pCommand->Execute(inputData);
				return;
			}
			break;
		case InputType::AXIS:
			glm::vec2 newAxisValue{ GetAxisData(input) };

			inputData.value.x = abs(inputData.value.x) > abs(newAxisValue.x) ? inputData.value.x : newAxisValue.x;
			inputData.value.y = abs(inputData.value.y) > abs(newAxisValue.y) ? inputData.value.y : newAxisValue.y;
			
			axisTrigger = true;
			break;
		}
	}

	if (axisTrigger)
	{
		binding.pCommand->Execute(inputData);
	}
}

glm::vec2 ngenius::InputManager::GetAxisData(const Input& input) const
{
	glm::vec2 newAxisValue{};
	if (input.source == InputSource::GAMEPAD)
	{
		const auto controllerIt{ m_ControllerMap.find(input.gamePadId) };
		if (controllerIt != std::end(m_ControllerMap))
		{
			const XINPUT_STATE& controllerState{ controllerIt->second };

			if (input.key & int(GamepadAxis::GAMEPAD_AXIS_THUMBLEFT))
			{
				newAxisValue.x = abs(controllerState.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? controllerState.Gamepad.sThumbLX / GAMEPAD_MAX_THUMB_VALUE : 0.f;
				newAxisValue.y = abs(controllerState.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? -controllerState.Gamepad.sThumbLY / GAMEPAD_MAX_THUMB_VALUE : 0.f;
			}
			else if (input.key & int(GamepadAxis::GAMEPAD_AXIS_THUMBRIGHT))
			{
				newAxisValue.x = abs(controllerState.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? controllerState.Gamepad.sThumbRX / GAMEPAD_MAX_THUMB_VALUE : 0.f;
				newAxisValue.y = abs(controllerState.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? -controllerState.Gamepad.sThumbRY / GAMEPAD_MAX_THUMB_VALUE : 0.f;
			}
			else if (input.key & int(GamepadAxis::GAMEPAD_AXIS_TRIGGERLEFT))
			{
				newAxisValue.x = newAxisValue.y = controllerState.Gamepad.bLeftTrigger / GAMEPAD_MAX_TRIGGER_VALUE;
			}
			else if (input.key & int(GamepadAxis::GAMEPAD_AXIS_TRIGGERRIGHT))
			{
				newAxisValue.x = newAxisValue.y = controllerState.Gamepad.bRightTrigger / GAMEPAD_MAX_TRIGGER_VALUE;
			}
		}
	}
	else if (input.source == InputSource::MOUSE)
	{
		int x, y;
		SDL_GetRelativeMouseState(&x, &y);
		newAxisValue.x = float(x);
		newAxisValue.y = float(y);
	}

	return newAxisValue;
}

ngenius::InputState ngenius::InputManager::GetButtonStateChange(const Input& input, InputState prevState) const
{
	bool isPressed{};
	if (input.source == InputSource::GAMEPAD)
	{
		auto it{ m_ControllerMap.find(input.gamePadId) };
		isPressed = it != std::end(m_ControllerMap) && it->second.Gamepad.wButtons & input.key;
	}
	else if (input.source == InputSource::KEYBOARD || input.source == InputSource::MOUSE)
	{
		isPressed = m_KeyboardState[input.key] & 0xF0;
	}

	if (isPressed && (prevState == InputState::NONE || prevState == InputState::RELEASED))
		return InputState::PRESSED;

	if (isPressed && prevState == InputState::PRESSED)
		return InputState::HELD;

	if (!isPressed && (prevState == InputState::PRESSED || prevState == InputState::HELD))
		return InputState::RELEASED;

	if (!isPressed && prevState == InputState::RELEASED)
		return InputState::NONE;

	return prevState;
}

void ngenius::InputManager::BindInput(const std::string& bindingName, ICommand* pCommand, std::initializer_list<Input> inpuList)
{
	auto emplacePair{ m_InputBindings.try_emplace(bindingName, bindingName, pCommand, inpuList) };
	if (!emplacePair.second)
	{
		std::cout << (emplacePair.first != m_InputBindings.end() ? bindingName + " input already exists !" : " Could not add input " + bindingName) << std::endl;
	}
}

int ngenius::InputManager::RegisterGamepad(int id)
{
	if (id < 0)
		id = static_cast<int>(m_ControllerMap.size());
	
	if (!m_ControllerMap.try_emplace(id, XINPUT_STATE{}).second)
	{
		std::cout << "Controller with ID " << id << " was already registered";
	}

	return id;
}

glm::ivec2 ngenius::InputManager::GetMousePosition()
{
	glm::ivec2 mousePos{};
	SDL_GetMouseState(&mousePos.x, &mousePos.y);
	return mousePos;
}