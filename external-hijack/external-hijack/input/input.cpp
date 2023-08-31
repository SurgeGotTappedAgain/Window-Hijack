#include "includes.hpp"

/* credit for reference https://www.unknowncheats.me/forum/c-and-c-/83707-setwindowshookex-example.html */

LRESULT CALLBACK Input::KeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        if (wParam == WM_KEYDOWN)
        {
            KBDLLHOOKSTRUCT* keyboard_struct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

            switch (keyboard_struct->vkCode)
            {
                case VK_INSERT:
                {
                    input_manager.toggle_menu = !input_manager.toggle_menu;
                    break;
                }
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK Input::MouseCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        MOUSEHOOKSTRUCT* mouse_struct = (MOUSEHOOKSTRUCT*)lParam;

        switch (wParam)
        {
            case WM_LBUTTONDOWN:
            {
                input_manager.left_click = !input_manager.left_click;
                break;
            }
            case WM_LBUTTONUP:
            {
                /* ensure that it's 100% set back to false */
                input_manager.left_click = false;
                break;
            }
            case WM_RBUTTONDOWN:
            {
                input_manager.right_click = !input_manager.right_click;
                break;
            }
            case WM_RBUTTONUP:
            {
                /* ensure that it's 100% set back to false */
                input_manager.right_click = false;
                break;
            }
        }
        input_manager.mouse_position = ImVec2(mouse_struct->pt.x, mouse_struct->pt.y);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void Input::SetHook()
{
    static bool hook_set = false;

    if (!hook_set) 
    {
        if (!(keyboard_hook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardCallback, NULL, 0))) {
            ExceptionManger::Error("Failed WH_KEYBOARD_LL");
        }
        if (!(mouse_hook = SetWindowsHookExA(WH_MOUSE_LL, MouseCallback, NULL, 0))) {
            ExceptionManger::Error("Failed WH_MOUSE_LL");
        }
        hook_set = true;
    }
}

void Input::ReleaseHook()
{
    UnhookWindowsHookEx(keyboard_hook);
    UnhookWindowsHookEx(mouse_hook);
}
