#include "MouseClass.h"

void MouseClass::OnLeftPressed(const int x, const int y)
{
	this->is_left_down_ = true;
	const MouseEvent em(MouseEvent::EventType::LPress, x, y);
	this->event_buffer_.push(em);
}

void MouseClass::OnLeftReleased(const int x, const int y)
{
	this->is_left_down_ = false;
	this->event_buffer_.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void MouseClass::OnRightPressed(const int x, const int y)
{
	this->is_right_down_ = true;
	const MouseEvent em(MouseEvent::EventType::RPress, x, y);
	this->event_buffer_.push(em);
}

void MouseClass::OnRightReleased(const int x, const int y)
{
	this->is_right_down_ = false;
	this->event_buffer_.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void MouseClass::OnMiddlePressed(const int x, const int y)
{
	this->is_right_down_ = true;
	const MouseEvent em(MouseEvent::EventType::MPress, x, y);
	this->event_buffer_.push(em);
}

void MouseClass::OnMiddleReleased(const int x, const int y)
{
	this->is_middle_down_ = false;
	this->event_buffer_.push(MouseEvent(MouseEvent::EventType::MRelease, x, y));
}

void MouseClass::OnWheelUp(const int x, const int y)
{
	this->event_buffer_.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseClass::OnWheelDown(const int x, const int y)
{
	this->event_buffer_.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void MouseClass::OnMouseMove(const int x, const int y)
{
	this->x_ = x;
	this->y_ = y;
	this->event_buffer_.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	this->event_buffer_.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}

bool MouseClass::IsLeftDown() const
{
	return this->is_left_down_;
}

bool MouseClass::IsRightDown() const
{
	return this->is_right_down_;
}

bool MouseClass::IsMiddleDown() const
{
	return this->is_middle_down_;
}

int MouseClass::GetPosX() const
{
	return this->x_;
}

int MouseClass::GetPosY() const
{
	return this->y_;
}

MousePoint MouseClass::GetPos() const
{
	return { this->x_, this->y_ };
}

bool MouseClass::EventBufferIsEmpty() const
{
	return this->event_buffer_.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	if (this->event_buffer_.empty())
	{
		return {};
	}
	else
	{
		const MouseEvent e = this->event_buffer_.front();
		this->event_buffer_.pop();
		return e;
	}
}
