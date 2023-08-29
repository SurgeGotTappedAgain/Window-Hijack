#pragma once

class Overlay
{
private:

	HWND window_handle;
	WNDCLASSEX window_class;

	ID3D11Device* d3d_device;
	ID3D11DeviceContext* device_context;
	IDXGISwapChain* swap_chain;
	ID3D11RenderTargetView* render_target_view;
	ImDrawList* draw_list;

	int			game_screen_width, game_screen_height;
	bool		menu = false;

public:

	bool InitDevice();
	void DestroyDevice();

	void InitImGui();
	void DestroyImGui();
	void InitRenderTarget();
	void DestroyRenderTarget();

	void DestroyWindow();

	const void Render(float width, float height);

	void InputHandler();
	bool MessageLoop();

	Overlay();
	~Overlay();
};

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