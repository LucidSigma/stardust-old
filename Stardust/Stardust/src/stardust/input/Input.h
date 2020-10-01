#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "../utility/interfaces/INoncopyable.h"
#include "../utility/interfaces/INonmovable.h"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <unordered_map>
#include <vector>

#include <magic_enum/magic_enum.hpp>
#include <SDL2/SDL.h>

#include "../graphics/renderer/Renderer.h"
#include "KeyCode.h"

namespace stardust
{
	class Input
		: private INoncopyable, private INonmovable
	{
	public:
		class Keyboard
		{
		private:
			const std::uint8_t* m_currentKeys;
			const std::uint8_t* m_previousKeys;

		public:
			Keyboard(const uint8_t* currentKeys, const std::uint8_t* previousKeys);
			~Keyboard() noexcept = default;

			[[nodiscard]] bool IsKeyDown(const KeyCode key) const;
			[[nodiscard]] bool IsKeyPressed(const KeyCode key) const;
			[[nodiscard]] bool IsKeyUp(const KeyCode key) const;

			[[nodiscard]] bool IsAnyKeyDown(const std::vector<KeyCode>& keys) const;
			[[nodiscard]] bool IsAnyKeyPressed(const std::vector<KeyCode>& keys) const;
			[[nodiscard]] bool IsAnyKeyUp(const std::vector<KeyCode>& keys) const;

			[[nodiscard]] bool AreAllKeysDown(const std::vector<KeyCode>& keys) const;
			[[nodiscard]] bool AreAllKeysPressed(const std::vector<KeyCode>& keys) const;
			[[nodiscard]] bool AreAllKeysUp(const std::vector<KeyCode>& keys) const;
		};

		class Mouse
		{
		public:
			enum class Button
				: decltype(SDL_BUTTON_LEFT)
			{
				Left = SDL_BUTTON_LEFT,
				Middle = SDL_BUTTON_MIDDLE,
				Right = SDL_BUTTON_RIGHT,
				Thumb1 = SDL_BUTTON_X1,
				Thumb2 = SDL_BUTTON_X2
			};

		private:
			static constexpr std::size_t s_ButtonCount = magic_enum::enum_count<Button>();

			std::uint32_t m_currentButtonStates;
			std::uint32_t m_previousButtonStates;

			float m_x;
			float m_y;

			float m_relativeX;
			float m_relativeY;

		public:
			friend class Input;

			inline static constexpr std::size_t GetButtonCount() { return s_ButtonCount; }

			Mouse() = default;
			~Mouse() noexcept = default;

			[[nodiscard]] inline glm::vec2 GetCoordinates() const noexcept { return glm::vec2{ m_x, m_y }; }
			[[nodiscard]] inline glm::vec2 GetProportionalCoordinates(const Renderer& renderer) const noexcept { return glm::vec2{ m_x, m_y } * renderer.GetLogicalSizeScale(); }

			[[nodiscard]] inline glm::vec2 GetRelativeCoordinates() const noexcept { return glm::vec2{ m_relativeX, m_relativeY }; }
			[[nodiscard]] inline glm::vec2 GetProportionalRelativeCoordinates(const Renderer& renderer) const noexcept { return glm::vec2{ m_relativeX, m_relativeY } * renderer.GetLogicalSizeScale(); }

			[[nodiscard]] bool IsButtonDown(const Button button) const;
			[[nodiscard]] bool IsButtonPressed(const Button button) const;
			[[nodiscard]] bool IsButtonUp(const Button button) const;

			[[nodiscard]] bool IsAnyButtonDown(const std::vector<Button>& buttons) const;
			[[nodiscard]] bool IsAnyButtonPressed(const std::vector<Button>& buttons) const;
			[[nodiscard]] bool IsAnyButtonUp(const std::vector<Button>& buttons) const;

			[[nodiscard]] bool AreAllButtonsDown(const std::vector<Button>& buttons) const;
			[[nodiscard]] bool AreAllButtonsPressed(const std::vector<Button>& buttons) const;
			[[nodiscard]] bool AreAllButtonsUp(const std::vector<Button>& buttons) const;
		};

		class GameController
			: private INoncopyable
		{
		public:
			enum class Button
			{
				DPadUp,
				DPadDown,
				DPadLeft,
				DPadRight,
				A,
				B,
				X,
				Y,
				Back,
				Guide,
				Start,
				LeftStick,
				RightStick,
				LeftShoulder,
				RightShoulder,
			};

			struct ButtonState
			{
				struct DPad
				{
					bool up;
					bool down;
					bool left;
					bool right;
				} dPad;

				bool a;
				bool b;
				bool x;
				bool y;

				bool back;
				bool guide;
				bool start;

				bool leftStick;
				bool rightStick;

				bool leftShoulder;
				bool rightShoulder;
			};

			struct Axes
			{
				struct Thumbstick
				{
					std::int16_t x;
					std::int16_t y;
				};

				Thumbstick left;
				Thumbstick right;

				std::int16_t leftTrigger;
				std::int16_t rightTrigger;
			};

		private:
			struct GameControllerDestroyer
			{
				void operator ()(SDL_GameController* const gameController) const noexcept;
			};

			struct HapticDeviceDestroyer
			{
				void operator ()(SDL_Haptic* const hapticDevice) const noexcept;
			};

			static constexpr std::size_t s_ButtonCount = magic_enum::enum_count<Button>();

			int m_id = 0;
			unsigned int m_playerIndex = 0u;

			ButtonState m_currentButtons;
			ButtonState m_previousButtons;
			Axes m_axes;

			std::unique_ptr<SDL_GameController, GameControllerDestroyer> m_handle = nullptr;
			std::unique_ptr<SDL_Haptic, HapticDeviceDestroyer> m_hapticDeviceHandle = nullptr;

			mutable bool m_isHapticDevicePaused = false;

		public:
			friend class Input;

			inline static constexpr std::size_t GetButtonCount() { return s_ButtonCount; }

			explicit GameController(const int id);
			GameController(GameController&& other) noexcept;
			GameController& operator =(GameController&& other) noexcept;
			~GameController() = default;

			[[nodiscard]] bool IsButtonDown(const Button button) const;
			[[nodiscard]] bool IsButtonPressed(const Button button) const;
			[[nodiscard]] bool IsButtonUp(const Button button) const;

			[[nodiscard]] bool IsAnyButtonDown(const std::vector<Button>& buttons) const;
			[[nodiscard]] bool IsAnyButtonPressed(const std::vector<Button>& buttons) const;
			[[nodiscard]] bool IsAnyButtonUp(const std::vector<Button>& buttons) const;

			[[nodiscard]] bool AreAllButtonsDown(const std::vector<Button>& buttons) const;
			[[nodiscard]] bool AreAllButtonsPressed(const std::vector<Button>& buttons) const;
			[[nodiscard]] bool AreAllButtonsUp(const std::vector<Button>& buttons) const;

			void Rumble(const float strength, const std::uint32_t milliseconds) const;
			void StopRumble() const;
			void PauseRumble() const;
			void ResumeRumble() const;
			inline bool IsRumblingPaused() const noexcept { return m_isHapticDevicePaused; }

			[[nodiscard]] inline bool HasHapticDevice() const noexcept { return m_hapticDeviceHandle != nullptr; }

			inline int GetID() const noexcept { return m_id; }
			inline const Axes& GetAxes() const noexcept { return m_axes; }

			inline unsigned int GetPlayerIndex() const noexcept { return m_playerIndex; }
			inline void SetPlayerIndex(const unsigned int playerIndex) noexcept { m_playerIndex = playerIndex; }

			[[nodiscard]] inline SDL_GameController* const GetRawHandle() const noexcept { return m_handle.get(); }
			[[nodiscard]] inline SDL_Haptic* const GetRawHapticDeviceHandle() const noexcept { return m_hapticDeviceHandle.get(); }
			[[nodiscard]] SDL_Joystick* const GetJoystickHandle() const;

		private:
			const bool GetButtonState(const Button button, const ButtonState& buttonState) const;
		};

	private:
		inline static std::vector<std::uint8_t> s_currentKeys{ std::vector<std::uint8_t>(SDL_NUM_SCANCODES, SDL_FALSE) };
		inline static std::vector<std::uint8_t> s_previousKeys{ std::vector<std::uint8_t>(SDL_NUM_SCANCODES, SDL_FALSE) };

		inline static std::uint32_t s_currentMouseButtonStates = 0;
		inline static std::uint32_t s_previousMouseButtonStates = 0;

		inline static Keyboard s_keyboardState{ s_currentKeys.data(), s_previousKeys.data() };
		inline static Mouse s_mouseState{ };
		inline static std::int32_t s_yScrollAmount = 0;

		inline static unsigned int s_maxGameControllers = std::numeric_limits<unsigned int>::max();
		inline static std::unordered_map<SDL_JoystickID, GameController> s_gameControllers{ };
		inline static unsigned int s_gameControllerDeadzone = 0u;

		inline static bool s_isMouseCaptured = false;
		inline static bool s_isMouseInRelativeMode = false;

	public:
		static bool IsMouseCaptured();
		static void CaptureMouse(const bool captureMouse);
		
		static bool IsMouseInRelativeMode();
		static void SetRelativeMouseMode(const bool isMouseRelative);
		static void ClearRelativeMouseState();

		static bool IsCursorShown();
		static void ShowCursor(const bool showCursor);
		static void WarpMouse(const class Window& window, const int x, const int y);

		static unsigned int GetMaxGameControllers() noexcept;
		static void SetMaxGameControllers(const unsigned int maxGameControllers) noexcept;
		static unsigned int GetGameControllerDeadzone() noexcept;
		static void SetGameControllerDeadzone(const unsigned int gameControllerDeadzone) noexcept;

		static GameController* AddGameController(const int id, const class Locale& locale);
		static void RemoveGameController(const int instanceID);
		static void RemoveAllGameControllers();
		static std::size_t GetGameControllerCount();

		static void UpdateKeyboardState();
		static void UpdateMouseState();
		static void ResetScrollState() noexcept;
		static void UpdateScrollState(const std::int32_t scrollAmount) noexcept;
		static void UpdateGameControllers();

		static const Keyboard& GetKeyboardState();
		static const Mouse& GetMouseState();
		static std::int32_t GetScrollState();

		[[nodiscard]] static bool DoesGameControllerExist(const SDL_JoystickID instanceID);
		[[nodiscard]] static GameController* GetGameController(const SDL_JoystickID instanceID);
		[[nodiscard]] static GameController* GetGameControllerByPlayerIndex(const unsigned int playerIndex);
		[[nodiscard]] static const std::unordered_map<SDL_JoystickID, GameController>& GetGameControllers();

		Input() = delete;
		~Input() noexcept = delete;
	};
}

#endif