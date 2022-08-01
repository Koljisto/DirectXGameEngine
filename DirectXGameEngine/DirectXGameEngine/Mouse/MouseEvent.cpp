#include "MouseEvent.h"
#include "MouseEvent.h"

MouseEvent::MouseEvent()
	:
	type_event_(EventType::Invalid),
	x_(0),
	y_(0)
{
}

MouseEvent::MouseEvent(const EventType type_event, const int x, const int y)
	:
	type_event_(type_event),
	x_(x),
	y_(y)
{
}

bool MouseEvent::IsValid() const
{
	return this->type_event_ != EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const
{
	return this->type_event_;
}

MousePoint MouseEvent::GetPos() const
{
	return { this->x_, this->y_ };
}

int MouseEvent::GetPosX() const
{
	return this->x_;
}

int MouseEvent::GetPosY() const
{
	return this->y_;
}
