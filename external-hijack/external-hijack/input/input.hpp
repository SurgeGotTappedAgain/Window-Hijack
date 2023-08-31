#pragma once

class Input
{
private:

	HHOOK hook;
	KBDLLHOOKSTRUCT hook_struct;

public:

	static LRESULT CALLBACK KeyboardCallback(int nCode,  WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MouseCallback(int nCode,  WPARAM wParam, LPARAM lParam);

	void SetHook();
	void ReleaseHook();
};