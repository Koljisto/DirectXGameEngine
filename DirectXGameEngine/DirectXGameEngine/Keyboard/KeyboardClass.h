#pragma once
#include "KeyboardEvent.h"
#include  <queue>

class KeyboardClass
{
public:
	KeyboardClass();
	bool KeyIsPressed(const unsigned char keycode) const;
	bool KeyBufferIsEmpty() const;
	bool CharBufferIsEmpty() const;
	KeyboardEvent ReadKey();
	unsigned char ReadChar();
	void OnKeyPressed(const unsigned char key_c);
	void OnKeyReleased(const unsigned char key_c);
	void OnChar(const unsigned char key_c);
	void EnableAutoRepeatKey();
	void DisableAutoRepeatKey();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();
	bool IsKeyAutoRepeat() const;
	bool IsCharsAutoRepeat() const;

private:
	bool auto_repeat_keys = false;
	bool auto_repeat_chars = false;
	bool key_states[256];
	std::queue<KeyboardEvent> key_buffer;
	std::queue<unsigned char> char_buffer;
};
