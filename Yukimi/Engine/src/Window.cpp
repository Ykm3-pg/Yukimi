#include "EnginePCH.h"
#include "Window.h"

namespace Yukimi
{

Win32Window::Win32Window()
    : mHandle(HWND())
    , mClassEx(WNDCLASSEX())
{
}

Win32Window::~Win32Window()
{
}

void Win32Window::Initialize(const WindowInfo& info)
{
    mClassEx.cbSize = sizeof(WNDCLASSEX);
    mClassEx.lpfnWndProc = static_cast<WNDPROC>(WindowProc);
    mClassEx.lpszClassName = info.name.c_str();
    mClassEx.hInstance = GetModuleHandle(nullptr);
    mClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassEx(&mClassEx);

    RECT wrc = { 0, 0, static_cast<LONG>(info.width), static_cast<LONG>(info.height) };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    const int MAX_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    const int MAX_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

    int posX = static_cast<int>((MAX_WIDTH / 2) - (info.width / 2));
    int posY = static_cast<int>((MAX_HEIGHT / 2) - (info.height / 2));
    int sizeX = wrc.right - wrc.left;
    int sizeY = wrc.bottom - wrc.top;

    mHandle = CreateWindow(
        mClassEx.lpszClassName,
        info.name.c_str(),
        WS_OVERLAPPEDWINDOW,
        posX,
        posY,
        sizeX,
        sizeY,
        nullptr,
        nullptr,
        mClassEx.hInstance,
        nullptr);

    ShowWindow(mHandle, SW_SHOW);
}

bool Win32Window::ProcessMessage()
{
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void Win32Window::Finalize()
{
    UnregisterClass(mClassEx.lpszClassName, mClassEx.hInstance);
}

LRESULT Win32Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

}
