#pragma once

namespace Yukimi
{

struct WindowInfo
{
    float width;
    float height;
    std::wstring name;
};

class IWindow
{
public:
    virtual void Initialize(const WindowInfo& info) = 0;
    virtual bool ProcessMessage() = 0;
    virtual void Finalize() = 0;
};

class Win32Window : public IWindow
{
public:
    Win32Window();
    virtual ~Win32Window();
    void Initialize(const WindowInfo& info) override;
    bool ProcessMessage() override;
    void Finalize() override;

private:
    /// <summary>
    /// ウィンドウプロシージャ
    /// </summary>
    /// <param name="hwnd"></param>
    /// <param name="msg"></param>
    /// <param name="wparam"></param>
    /// <param name="lparam"></param>
    /// <returns></returns>
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
    HWND mHandle;
    WNDCLASSEX mClassEx;
};

}
