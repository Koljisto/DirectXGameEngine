#include "KeyboardClass.h"

KeyboardClass::KeyboardClass()
{
	for (bool& key_state : this->key_states)
		key_state = false;
}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode) const
{
	return this->key_states[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty() const
{
	return this->key_buffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty() const
{
	return this->char_buffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
{
	if (this->key_buffer.empty()) // If no keys to be read?
	{
		return {}; // return empty keyboard event
	}
	else
	{
		const KeyboardEvent e = this->key_buffer.front(); // get first keyboard event from queue
		this->key_buffer.pop(); // remove first item from queue
		return e; // returns keyboard event
	}
}

unsigned char KeyboardClass::ReadChar()
{
	if (this->char_buffer.empty()) // If no char to be read?
	{
		return 0u; // return 0 (NULL char)
	}
	else
	{
		const unsigned char e = this->char_buffer.front(); // get first char from queue
		this->char_buffer.pop(); // remove first item from queue
		return e; // returns char
	}
}

void KeyboardClass::OnKeyPressed(const unsigned char key_c)
{
	this->key_states[key_c] = true;
	this->key_buffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key_c));
}

void KeyboardClass::OnKeyReleased(const unsigned char key_c)
{
	this->key_states[key_c] = false;
	this->key_buffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key_c));
}

void KeyboardClass::OnChar(const unsigned char key_c)
{
	this->char_buffer.push(key_c);
}

void KeyboardClass::EnableAutoRepeatKey()
{
	this->auto_repeat_keys = true;
}

void KeyboardClass::DisableAutoRepeatKey()
{
	this->auto_repeat_keys = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
	this->auto_repeat_chars = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
	this->auto_repeat_chars = false;
}

bool KeyboardClass::IsKeyAutoRepeat() const
{
	return this->auto_repeat_keys;
}

bool KeyboardClass::IsCharsAutoRepeat() const
{
	return this->auto_repeat_chars;
}
