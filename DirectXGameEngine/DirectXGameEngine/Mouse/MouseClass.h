#pragma once
#include "MouseEvent.h"
#include <queue>

class MouseClass
{
public:
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);

	bool IsLeftDown() const;
	bool IsRightDown() const;
	bool IsMiddleDown() const;

	int GetPosX() const;
	int GetPosY() const;
	MousePoint GetPos() const;

	bool EventBufferIsEmpty() const;
	MouseEvent ReadEvent();
private:
	std::queue<MouseEvent> event_buffer_;
	bool is_left_down_ = false;
	bool is_right_down_ = false;
	bool is_middle_down_ = false;
	int x_ = 0;
	int y_ = 0;

};
