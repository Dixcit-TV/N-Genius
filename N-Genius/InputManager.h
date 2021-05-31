#pragma once
#include <map>
#include <SDL_gamecontroller.h>
#include <XInput.h>

#include "Singleton.h"
#include "Input.h"
#include "Command.h"

namespace ngenius
{
	struct InputBinding
	{
		explicit InputBinding(const std::string& bindingName, ICommand* pCommand, std::initializer_list<Input> inpuList)
			: bindingName(bindingName), inputs(inpuList), pCommand(pCommand), prevState(InputState::NONE)
		{}

		InputBinding(const InputBinding& rhs) = delete;
		InputBinding(InputBinding&& rhs) noexcept = delete;
		InputBinding& operator=(const InputBinding& rhs) = delete;
		InputBinding& operator=(InputBinding&& rhs) noexcept = delete;

		~InputBinding()
		{
			delete pCommand;
		}

		std::string bindingName;
		std::vector<Input> inputs;
		ICommand* pCommand;
		InputState prevState;
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		using ControllerMap = std::map<DWORD, XINPUT_STATE>;
		using InputBindings = std::map<std::string, InputBinding>;
		
		~InputManager() override{ delete[] m_KeyboardState; };
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;
		
		bool ProcessInput();

		void BindInput(const std::string& bindingName, ICommand* pCommand, std::initializer_list<Input> inpuList);
		int RegisterGamepad(int id = -1);

		static glm::ivec2 GetMousePosition();
	
	private:
		friend class Singleton<InputManager>;
		explicit InputManager() = default;

		BYTE* m_KeyboardState = new BYTE[256];
		ControllerMap m_ControllerMap;
		InputBindings m_InputBindings;

		void ProcessInput(InputBinding& binding) const;
		glm::vec2 GetAxisData(const Input& input) const;
		InputState GetButtonStateChange(const Input& input, InputState prevState) const;
	};

}
