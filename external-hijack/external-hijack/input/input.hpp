#pragma once

class Input
{
private:

	HHOOK keyboard_hook;
	HHOOK mouse_hook;

public:

	static LRESULT CALLBACK KeyboardCallback(int nCode,  WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MouseCallback(int nCode,  WPARAM wParam, LPARAM lParam);

	void SetHook();
	void ReleaseHook();
};