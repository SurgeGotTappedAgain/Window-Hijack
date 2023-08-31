#include "includes.hpp"

bool Overlay::InitDevice()
{
	DXGI_SWAP_CHAIN_DESC swap_chain_description;
	ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));
	swap_chain_description.BufferCount = 2;
	swap_chain_description.BufferDesc.Width = 0;
	swap_chain_description.BufferDesc.Height = 0;
	swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.OutputWindow = window_handle;
	swap_chain_description.SampleDesc.Count = 1;
	swap_chain_description.SampleDesc.Quality = 0;
	swap_chain_description.Windowed = TRUE;
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL feature_level;
	D3D_FEATURE_LEVEL feature_level_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, feature_level_array, 2, D3D11_SDK_VERSION, &swap_chain_description, &swap_chain, &d3d_device, &feature_level, &device_context) != S_OK)
		return false;

	InitRenderTarget();

	ShowWindow(window_handle, SW_SHOW);
	UpdateWindow(window_handle);

	return true;
}

void Overlay::DestroyDevice()
{
	DestroyRenderTarget();
	swap_chain->Release();
	device_context->Release();
	d3d_device->Release();
}

void Overlay::DestroyImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Overlay::InitRenderTarget()
{
	ID3D11Texture2D* back_buffer = nullptr;
	swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
	if (back_buffer) 
	{
		d3d_device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);
		back_buffer->Release();
	}
}

void Overlay::DestroyRenderTarget() 
{
	if (!render_target_view)
		return;

	render_target_view->Release();
	render_target_view = NULL;
}

void Overlay::DestroyWindow()
{
	char command[256];
	snprintf(command, sizeof(command), "taskkill /F /IM \"%s\"", "ow-overlay.exe");
	system(command);
}

void Overlay::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	/* styles & fonts here */
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init(window_handle);
	ImGui_ImplDX11_Init(d3d_device, device_context);
}

Overlay::Overlay(HWND target_window)
{
	window_handle = target_window;
	if (!window_handle)
		return;

	if (!InitDevice())
		return;

	InitImGui();
}

Overlay::~Overlay() 
{
	DestroyImGui();
	DestroyDevice();
}

const void Overlay::Render()
{
	InputHandler();

	if (input_manager.toggle_menu) {
		ImGui::ShowDemoWindow();
	}

	ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 10), ImColor(255, 255, 255, 255), "test");
}

bool Overlay::MessageLoop()
{
	if (window_handle)
	{
		ImVec4 clear_clr = { 0, 0, 0, 0 };

		while (true)
		{
			MSG msg{ 0 };

			if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					return false;

				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				Render();

				ImGui::Render();
				device_context->OMSetRenderTargets(1, &render_target_view, NULL);
				device_context->ClearRenderTargetView(render_target_view, (float*)&clear_clr);

				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				swap_chain->Present(1, 0);
			}
		}
	}
	return false;
}

void Overlay::InputHandler()
{
	Input input;
	input.SetHook();

	ImGuiIO& io = ImGui::GetIO();

	io.MousePos = input_manager.mouse_position;
	io.MouseDown[0] = input_manager.left_click;
	io.MouseDown[1] = input_manager.right_click;
}