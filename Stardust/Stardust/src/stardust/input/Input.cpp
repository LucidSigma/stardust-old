#include "Input.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <utility>

#include "../locale/Locale.h"
#include "../debug/logging/Log.h"
#include "../debug/message_box/MessageBox.h"
#include "../graphics/window/Window.h"

namespace stardust
{
	Input::Keyboard::Keyboard(const uint8_t* currentKeys, const std::uint8_t* previousKeys)
		: m_currentKeys(currentKeys), m_previousKeys(previousKeys)
	{ }

	[[nodiscard]] bool Input::Keyboard::IsKeyDown(const KeyCode key) const
	{
		return m_currentKeys[static_cast<SDL_Scancode>(key)] && !m_previousKeys[static_cast<SDL_Scancode>(key)];
	}
	
	[[nodiscard]] bool Input::Keyboard::IsKeyPressed(const KeyCode key) const
	{
		return m_currentKeys[static_cast<SDL_Scancode>(key)];
	}
	
	[[nodiscard]] bool Input::Keyboard::IsKeyUp(const KeyCode key) const
	{
		return !m_currentKeys[static_cast<SDL_Scancode>(key)] && m_previousKeys[static_cast<SDL_Scancode>(key)];;
	}

	[[nodiscard]] bool Input::Keyboard::IsAnyKeyDown(const std::vector<KeyCode>& keys) const
	{
		return std::any_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
		{
			return IsKeyDown(keycode);
		});
	}

	[[nodiscard]] bool Input::Keyboard::IsAnyKeyPressed(const std::vector<KeyCode>& keys) const
	{
		return std::any_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
		{
			return IsKeyPressed(keycode);
		});
	}

	[[nodiscard]] bool Input::Keyboard::IsAnyKeyUp(const std::vector<KeyCode>& keys) const
	{
		return std::any_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
		{
			return IsKeyUp(keycode);
		});
	}

	[[nodiscard]] bool Input::Keyboard::AreAllKeysDown(const std::vector<KeyCode>& keys) const
	{
		return std::all_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
		{
			return IsKeyDown(keycode);
		});
	}

	[[nodiscard]] bool Input::Keyboard::AreAllKeysPressed(const std::vector<KeyCode>& keys) const
	{
		return std::all_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
		{
			return IsKeyPressed(keycode);
		});
	}

	[[nodiscard]] bool Input::Keyboard::AreAllKeysUp(const std::vector<KeyCode>& keys) const
	{
		return std::all_of(std::cbegin(keys), std::cend(keys), [this](const KeyCode keycode)
		{
			return IsKeyUp(keycode);
		});
	}

	[[nodiscard]] bool Input::Mouse::IsButtonDown(const Button button) const
	{
		const auto buttonValue = static_cast<std::underlying_type_t<Button>>(button);

		return (m_currentButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue) && !((m_previousButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue));
	}

	[[nodiscard]] bool Input::Mouse::IsButtonPressed(const Button button) const
	{
		const auto buttonValue = static_cast<std::underlying_type_t<Button>>(button);

		return (m_currentButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue);
	}

	[[nodiscard]] bool Input::Mouse::IsButtonUp(const Button button) const
	{
		const auto buttonValue = static_cast<std::underlying_type_t<Button>>(button);

		return !((m_currentButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue)) && (m_previousButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue);
	}

	[[nodiscard]] bool Input::Mouse::IsAnyButtonDown(const std::vector<Button>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonDown(button);
		});
	}

	[[nodiscard]] bool Input::Mouse::IsAnyButtonPressed(const std::vector<Button>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonPressed(button);
		});
	}

	[[nodiscard]] bool Input::Mouse::IsAnyButtonUp(const std::vector<Button>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonUp(button);
		});
	}

	[[nodiscard]] bool Input::Mouse::AreAllButtonsDown(const std::vector<Button>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonDown(button);
		});
	}

	[[nodiscard]] bool Input::Mouse::AreAllButtonsPressed(const std::vector<Button>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonPressed(button);
		});
	}

	[[nodiscard]] bool Input::Mouse::AreAllButtonsUp(const std::vector<Button>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonUp(button);
		});
	}

	void Input::GameController::GameControllerDestroyer::operator ()(SDL_GameController* const gameController) const noexcept
	{
		SDL_GameControllerClose(gameController);
	}

	void Input::GameController::HapticDeviceDestroyer::operator ()(SDL_Haptic* const hapticDevice) const noexcept
	{
		SDL_HapticClose(hapticDevice);
	}

	Input::GameController::GameController(const int id)
	{
		m_id = id;
		m_currentButtons = ButtonState{ };
		m_previousButtons = ButtonState{ };
		m_axes = Axes{ };

		m_handle = std::unique_ptr<SDL_GameController, GameControllerDestroyer>(SDL_GameControllerOpen(id));

		if (m_handle != nullptr)
		{
			m_hapticDeviceHandle = std::unique_ptr<SDL_Haptic, HapticDeviceDestroyer>(SDL_HapticOpenFromJoystick(GetJoystickHandle()));

			if (m_hapticDeviceHandle != nullptr)
			{
				if (SDL_HapticRumbleInit(GetRawHapticDeviceHandle()) != 0 || SDL_HapticRumbleSupported(GetRawHapticDeviceHandle()) != SDL_TRUE)
				{
					Log::EngineWarn("Haptic device in controller {} could not be initialised: {}.", id, SDL_GetError());
					m_hapticDeviceHandle = nullptr;
				}
			}
		}
	}

	Input::GameController::GameController(GameController&& other) noexcept
		: m_id(0), m_currentButtons(ButtonState{ }), m_previousButtons(ButtonState{ }), m_axes(Axes{ }), m_handle(nullptr), m_hapticDeviceHandle(nullptr), m_isHapticDevicePaused(false)
	{
		std::swap(m_id, other.m_id);

		std::swap(m_currentButtons, other.m_currentButtons);
		std::swap(m_previousButtons, other.m_previousButtons);
		std::swap(m_axes, other.m_axes);

		std::swap(m_handle, other.m_handle);
		std::swap(m_hapticDeviceHandle, other.m_hapticDeviceHandle);

		std::swap(m_isHapticDevicePaused, other.m_isHapticDevicePaused);
	}

	Input::GameController& Input::GameController::operator =(GameController&& other) noexcept
	{
		m_id = std::exchange(other.m_id, 0);

		m_currentButtons = std::exchange(other.m_currentButtons, ButtonState{ });
		m_previousButtons = std::exchange(other.m_previousButtons, ButtonState{ });
		m_axes = std::exchange(other.m_axes, Axes{ });

		m_handle = std::exchange(other.m_handle, nullptr);
		m_hapticDeviceHandle = std::exchange(other.m_hapticDeviceHandle, nullptr);

		m_isHapticDevicePaused = std::exchange(other.m_isHapticDevicePaused, false);

		return *this;
	}

	[[nodiscard]] bool Input::GameController::IsButtonDown(const Button button) const
	{
		return GetButtonState(button, m_currentButtons) && !GetButtonState(button, m_previousButtons);
	}

	[[nodiscard]] bool Input::GameController::IsButtonPressed(const Button button) const
	{
		return GetButtonState(button, m_currentButtons);
	}

	[[nodiscard]] bool Input::GameController::IsButtonUp(const Button button) const
	{
		return !GetButtonState(button, m_currentButtons) && GetButtonState(button, m_previousButtons);
	}

	[[nodiscard]] bool Input::GameController::IsAnyButtonDown(const std::vector<Button>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonDown(button);
		});
	}

	[[nodiscard]] bool Input::GameController::IsAnyButtonPressed(const std::vector<Button>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonPressed(button);
		});
	}

	[[nodiscard]] bool Input::GameController::IsAnyButtonUp(const std::vector<Button>& buttons) const
	{
		return std::any_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonUp(button);
		});
	}

	[[nodiscard]] bool Input::GameController::AreAllButtonsDown(const std::vector<Button>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonDown(button);
		});
	}

	[[nodiscard]] bool Input::GameController::AreAllButtonsPressed(const std::vector<Button>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonPressed(button);
		});
	}

	[[nodiscard]] bool Input::GameController::AreAllButtonsUp(const std::vector<Button>& buttons) const
	{
		return std::all_of(std::cbegin(buttons), std::cend(buttons), [this](const Button button)
		{
			return IsButtonUp(button);
		});
	}

	void Input::GameController::Rumble(const float strength, const std::uint32_t milliseconds) const
	{
		if (HasHapticDevice())
		{
			SDL_HapticRumblePlay(GetRawHapticDeviceHandle(), std::clamp(strength, 0.0f, 1.0f), milliseconds);
		}
	}

	void Input::GameController::StopRumble() const
	{
		if (HasHapticDevice())
		{
			SDL_HapticRumbleStop(GetRawHapticDeviceHandle());
		}
	}

	void Input::GameController::PauseRumble() const
	{
		if (HasHapticDevice())
		{
			m_isHapticDevicePaused = SDL_HapticPause(GetRawHapticDeviceHandle()) == 0;
		}
	}

	void Input::GameController::ResumeRumble() const
	{
		if (HasHapticDevice())
		{
			m_isHapticDevicePaused = m_isHapticDevicePaused && SDL_HapticUnpause(GetRawHapticDeviceHandle()) != 0;
		}
	}

	[[nodiscard]] SDL_Joystick* const Input::GameController::GetJoystickHandle() const
	{
		return SDL_GameControllerGetJoystick(GetRawHandle());
	}

	const bool Input::GameController::GetButtonState(const Button button, const ButtonState& buttonState) const
	{
		switch (button)
		{
		case Button::DPadUp:
			return buttonState.dPad.up;

		case Button::DPadDown:
			return buttonState.dPad.down;

		case Button::DPadLeft:
			return buttonState.dPad.left;

		case Button::DPadRight:
			return buttonState.dPad.right;

		case Button::A:
			return buttonState.a;

		case Button::B:
			return buttonState.b;

		case Button::X:
			return buttonState.x;

		case Button::Y:
			return buttonState.y;

		case Button::Back:
			return buttonState.back;

		case Button::Guide:
			return buttonState.guide;

		case Button::Start:
			return buttonState.start;

		case Button::LeftStick:
			return buttonState.leftStick;

		case Button::RightStick:
			return buttonState.rightStick;

		case Button::LeftShoulder:
			return buttonState.leftShoulder;

		case Button::RightShoulder:
			return buttonState.rightShoulder;

		default:
			return false;
		}
	}

	bool Input::IsMouseCaptured()
	{
		return s_isMouseCaptured;
	}

	void Input::CaptureMouse(const bool captureMouse)
	{
		s_isMouseInRelativeMode = SDL_CaptureMouse(static_cast<SDL_bool>(captureMouse)) == 0;
	}

	bool Input::IsMouseInRelativeMode()
	{
		return s_isMouseInRelativeMode;
	}

	void Input::SetRelativeMouseMode(const bool isMouseRelative)
	{
		s_isMouseInRelativeMode = SDL_SetRelativeMouseMode(static_cast<SDL_bool>(isMouseRelative)) == 0;

		if (s_isMouseInRelativeMode)
		{
			ClearRelativeMouseState();
		}
	}

	void Input::ClearRelativeMouseState()
	{
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}

	bool Input::IsCursorShown()
	{
		return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
	}

	void Input::ShowCursor(const bool showCursor)
	{
		SDL_ShowCursor(showCursor ? SDL_ENABLE : SDL_DISABLE);
	}

	void Input::WarpMouse(const Window& window, const int x, const int y)
	{
		SDL_WarpMouseInWindow(window.GetRawHandle(), x, y);
	}

	unsigned int Input::GetMaxGameControllers() noexcept
	{
		return s_maxGameControllers;
	}

	void Input::SetMaxGameControllers(const unsigned int maxGameControllers) noexcept
	{
		s_maxGameControllers = std::max(1u, maxGameControllers);
	}

	unsigned int Input::GetGameControllerDeadzone() noexcept
	{
		return s_gameControllerDeadzone;
	}

	void Input::SetGameControllerDeadzone(const unsigned int gameControllerDeadzone) noexcept
	{
		s_gameControllerDeadzone = gameControllerDeadzone;
	}

	Input::GameController* Input::AddGameController(const int id, const Locale& locale)
	{
		if (GetGameControllerCount() == s_maxGameControllers)
		{
			return nullptr;
		}

		GameController gameController(id);
		const SDL_JoystickID instanceID = SDL_JoystickInstanceID(gameController.GetJoystickHandle());

		s_gameControllers.emplace(instanceID, std::move(gameController));

		if (s_gameControllers.at(instanceID).GetRawHandle() == nullptr)
		{
			message_box::Show(locale["warnings"]["titles"]["controller"], locale["warnings"]["bodies"]["controller"], message_box::Type::Warning);
			Log::EngineWarn("Could not add new game controller: {}.", SDL_GetError());

			s_gameControllers.erase(instanceID);

			return nullptr;
		}
		else
		{
			Log::EngineTrace("Controller {} added (Instance ID {}).", id, instanceID);

			return &s_gameControllers.at(instanceID);
		}
	}

	void Input::RemoveGameController(const int instanceID)
	{
		if (s_gameControllers.contains(instanceID))
		{
			s_gameControllers.erase(instanceID);
			Log::EngineTrace("Controller removed: {}", instanceID);
		}
	}

	void Input::RemoveAllGameControllers()
	{
		s_gameControllers.clear();
	}

	std::size_t Input::GetGameControllerCount()
	{
		return s_gameControllers.size();
	}

	void Input::UpdateKeyboardState()
	{
		s_previousKeys = std::move(s_currentKeys);
		s_currentKeys = std::move(std::vector<std::uint8_t>(SDL_GetKeyboardState(nullptr), SDL_GetKeyboardState(nullptr) + SDL_NUM_SCANCODES));

		s_keyboardState = Keyboard(s_currentKeys.data(), s_previousKeys.data());
	}

	void Input::UpdateMouseState()
	{
		int x = 0;
		int y = 0;
		s_previousMouseButtonStates = s_currentMouseButtonStates;
		s_currentMouseButtonStates = SDL_GetMouseState(&x, &y);

		int relativeX = 0;
		int relativeY = 0;
		SDL_GetRelativeMouseState(&relativeX, &relativeY);

		s_mouseState.m_currentButtonStates = s_currentMouseButtonStates;
		s_mouseState.m_previousButtonStates = s_previousMouseButtonStates;
		s_mouseState.m_x = static_cast<float>(x);
		s_mouseState.m_y = static_cast<float>(y);
		s_mouseState.m_relativeX = static_cast<float>(relativeX);
		s_mouseState.m_relativeY = static_cast<float>(relativeY);
	}

	void Input::ResetScrollState() noexcept
	{
		s_yScrollAmount = 0;
	}

	void Input::UpdateScrollState(const std::int32_t scrollAmount) noexcept
	{
		s_yScrollAmount = scrollAmount;
	}

	void Input::UpdateGameControllers()
	{
		for (auto& [id, gameController] : s_gameControllers)
		{
			gameController.m_previousButtons = gameController.m_currentButtons;

			gameController.m_currentButtons.dPad.up = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_UP) == 1u;
			gameController.m_currentButtons.dPad.down = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1u;
			gameController.m_currentButtons.dPad.left = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1u;
			gameController.m_currentButtons.dPad.right = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1u;

			gameController.m_currentButtons.a = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_A) == 1u;
			gameController.m_currentButtons.b = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_B) == 1u;
			gameController.m_currentButtons.x = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_X) == 1u;
			gameController.m_currentButtons.y = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_Y) == 1u;
				
			gameController.m_currentButtons.back = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_BACK) == 1u;
			gameController.m_currentButtons.guide = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_GUIDE) == 1u;
			gameController.m_currentButtons.start = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_START) == 1u;
				
			gameController.m_currentButtons.leftStick = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_LEFTSTICK) == 1u;
			gameController.m_currentButtons.rightStick = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_RIGHTSTICK) == 1u;
				
			gameController.m_currentButtons.leftShoulder = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == 1u;
			gameController.m_currentButtons.rightShoulder = SDL_GameControllerGetButton(gameController.GetRawHandle(), SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == 1u;

			gameController.m_axes.left.x = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_LEFTX);
			gameController.m_axes.left.y = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_LEFTY);
			gameController.m_axes.right.x = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_RIGHTX);
			gameController.m_axes.right.y = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_RIGHTY);
				
			gameController.m_axes.leftTrigger = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_TRIGGERLEFT);
			gameController.m_axes.rightTrigger = SDL_GameControllerGetAxis(gameController.GetRawHandle(), SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

			const std::initializer_list<std::reference_wrapper<std::int16_t>> axes{
				std::ref(gameController.m_axes.left.x),
				std::ref(gameController.m_axes.left.y),
				std::ref(gameController.m_axes.right.x),
				std::ref(gameController.m_axes.right.y),
				std::ref(gameController.m_axes.leftTrigger),
				std::ref(gameController.m_axes.rightTrigger),
			};

			for (auto& axis : axes)
			{
				if (static_cast<unsigned int>(std::abs(axis.get())) < s_gameControllerDeadzone)
				{
					axis.get() = 0;
				}
			}
		}
	}

	const Input::Keyboard& Input::GetKeyboardState()
	{
		return s_keyboardState;
	}

	const Input::Mouse& Input::GetMouseState()
	{
		return s_mouseState;
	}

	std::int32_t Input::GetScrollState()
	{
		return s_yScrollAmount;
	}

	[[nodiscard]] bool Input::DoesGameControllerExist(const SDL_JoystickID instanceID)
	{
		return s_gameControllers.contains(instanceID);
	}

	[[nodiscard]] Input::GameController* Input::GetGameController(const SDL_JoystickID instanceID)
	{
		return DoesGameControllerExist(instanceID) ? &s_gameControllers.at(instanceID) : nullptr;
	}

	[[nodiscard]] Input::GameController* Input::GetGameControllerByPlayerIndex(const unsigned int playerIndex)
	{
		for (auto& [id, gameController] : s_gameControllers)
		{
			if (gameController.m_playerIndex == playerIndex)
			{
				return &gameController;
			}
		}

		return nullptr;
	}

	[[nodiscard]] const std::unordered_map<SDL_JoystickID, Input::GameController>& Input::GetGameControllers()
	{
		return s_gameControllers;
	}
}