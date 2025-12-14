#include "EnginePCH.h"
#include "Window.h"

namespace Yukimi
{

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

Win32Window::Win32Window()
    : mHandle(HWND())
    , mClassEx(WNDCLASSEX())
    , mWidth(1280)
    , mHeight(720)
{
}

Win32Window::~Win32Window()
{
}

void Win32Window::Initialize(int width, int height, std::wstring name)
{
    mWidth = width;
    mHeight = height;

    mClassEx.cbSize = sizeof(WNDCLASSEX);
    mClassEx.lpfnWndProc = static_cast<WNDPROC>(WindowProc);
    mClassEx.lpszClassName = name.c_str();
    mClassEx.hInstance = GetModuleHandle(nullptr);
    mClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassEx(&mClassEx);

    RECT wrc = { 0, 0, static_cast<LONG>(mWidth), static_cast<LONG>(mHeight) };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    const int MAX_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    const int MAX_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

    int pos_x = static_cast<int>((MAX_WIDTH / 2) - (mWidth / 2));
    int pos_y = static_cast<int>((MAX_HEIGHT / 2) - (mHeight / 2));
    int size_x = wrc.right - wrc.left;
    int size_y = wrc.bottom - wrc.top;

    mHandle = CreateWindow(
        mClassEx.lpszClassName,
        name.c_str(),
        WS_OVERLAPPEDWINDOW,
        pos_x,
        pos_y,
        size_x,
        size_y,
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

}
