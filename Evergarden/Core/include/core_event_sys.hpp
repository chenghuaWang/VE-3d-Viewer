/*!
 * @author  chenghua.wang
 * @file    Evergarden/Core/include/core_event_sys.hpp
 * @brief   A basic event system class for Layer event
 * 
 * @details The Most Event code here is depend on Hazel
 * Event system. We add more application event to this
 * Event-sys for layer infomation transfer convenience
 * 
 * You can check https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Events/
 * for more infomation.
 */

#ifndef H_CORE_EVENT_SYS
#define H_CORE_EVENT_SYS

#ifdef WIN32
#pragma once
#endif //! WIN32

#include "core_base.hpp"
#include "core_event_codes.hpp"

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

///< The basic class for Event
namespace evergarden {

    enum class EventType {
		None = 0,
		WindowClose, 
        WindowResize, 
        WindowFocus, 
        WindowLostFocus, 
        WindowMoved,
		AppTick, 
        AppUpdate, 
        AppRender,
		KeyPressed, 
        KeyReleased, 
        KeyTyped,
		MouseButtonPressed, 
        MouseButtonReleased, 
        MouseMoved, 
        MouseScrolled
        // TODO Add more layer info
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication    = BIT(0),
		EventCategoryInput          = BIT(1),
		EventCategoryKeyboard       = BIT(2),
		EventCategoryMouse          = BIT(3),
		EventCategoryMouseButton    = BIT(4)
        // TODO Add more Layer info 
	};


    /*!
     * @brief class provide blocking event system.
     * This system is referenced from 'Hazel' engine
     * 
     * @details Why we need m_Handled?
     * m_Handled is a flag that means if this event is
     * done or not. Due to blocking event system, this
     * flag is uesd to tell other class using this event
     * if this event should pass to others or not.
     */
    class Event {
    public:
        virtual ~Event() = default;

	public:
        virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); } ///< ToString function here for Log and debug porpose.

    public:
		bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

    public:
        bool m_Handled = false; ///< member of handled. check if this event is handled or not.
    };

    /*!
     * @brief class for event process.
     *
     * @details This class will be used in onEvent functions in every layer.
     * we will build a dispather every time a event is send to a layer. To
     * Mind that, this approach is quite slow in some situation. The broad cast
     * method to all layers is not a really good way to build a event system.
     * Also, you can't manage the event-handle flag will in very complex layer
     * systems.
     */
    class EventDispatcher {
	public:
		EventDispatcher(Event& event): m_Event(event) {}

    public:		
		template<typename T, typename F>
		bool Dispatch(const F& func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};
} //! namespace evergarden.


///< The KeyCodes Event inhirent from Event
namespace evergarden {

    class KeyEvent: public Event {
	public:
		KeyCodes GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(const KeyCodes keycode): m_KeyCode(keycode) {}

		KeyCodes m_KeyCode;
	};

    class KeyPressedEvent: public KeyEvent {
	public:
		KeyPressedEvent(const KeyCodes keycode, const uint16_t repeatCount): KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		uint16_t GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	
    private:
		uint16_t m_RepeatCount;
	};

    class KeyReleasedEvent: public KeyEvent {
	public:
		KeyReleasedEvent(const KeyCodes keycode): KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent: public KeyEvent {
	public:
		KeyTypedEvent(const KeyCodes keycode): KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

} //! namespace evergarden.


///< The MouseCodes Event inhirent from Event
namespace evergarden {

    class MouseMovedEvent: public Event {
	public:
		MouseMovedEvent(const float x, const float y): m_MouseX(x), m_MouseY(y) {}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	
    private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent: public Event {
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset): m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent: public Event {
	public:
		MouseCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	
    protected:
		MouseButtonEvent(const MouseCode button): m_Button(button) {}

		MouseCode m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(const MouseCode button): MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent: public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(const MouseCode button): MouseButtonEvent(button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}


///< The Application Event inhirent from Event
namespace evergarden {

    class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height): m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:

		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent: public Event{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event {
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event {
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}


#endif //! H_CORE_EVENT_SYS