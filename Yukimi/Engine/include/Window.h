#pragma once

namespace Yukimi
{

class Win32Window
{
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

public:
    /// <summary>
    /// 
    /// </summary>
    Win32Window();

    ///
    virtual ~Win32Window();

    /// <summary>
    /// 
    /// </summary>
    /// <param name="width"></param>
    /// <param name="height"></param>
    /// <param name="name"></param>
    void Initialize(int width, int height, std::wstring name);

    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    bool ProcessMessage();

    /// <summary>
    /// 
    /// </summary>
    void Finalize();

    const HWND GetHandle() const {
        return mHandle;
    }

    const int GetWidth() const {
        return mWidth;
    }

    const int GetHeight() const {
        return mHeight;
    }

private:
    HWND mHandle;

    WNDCLASSEX mClassEx;

    int mWidth;

    int mHeight;
};

}
