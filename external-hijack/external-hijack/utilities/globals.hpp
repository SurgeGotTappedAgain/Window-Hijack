#pragma once

class ExceptionManger : public std::exception
{
public:
	ExceptionManger(const std::string& message) : message(message) {}

	virtual const char* what() const noexcept override
	{
		return message.c_str();
	}

	static void Error(const std::string& errorMessage)
	{
		throw ExceptionManger(errorMessage);
	}

private:
	std::string message;
};

class InputManager
{
public:

	bool toggle_menu, left_click, right_click;
	ImVec2 mouse_position;

} input_manager;