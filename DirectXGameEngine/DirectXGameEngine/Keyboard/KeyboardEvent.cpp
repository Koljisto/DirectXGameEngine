#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent()
	:
	type_event_(EventType::Invalid),
	key_c_(0u)
{
}

KeyboardEvent::KeyboardEvent(const EventType type_event, const unsigned char key_c)
	:
	type_event_(type_event),
	key_c_(key_c)
{
}

bool KeyboardEvent::IsPress() const
{
	return this->type_event_ == EventType::Press;
}

bool KeyboardEvent::IsRelease() const
{
	return this->type_event_ == EventType::Release;
}

bool KeyboardEvent::IsValid() const
{
	return this->type_event_ != EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
	return this->key_c_;
}
