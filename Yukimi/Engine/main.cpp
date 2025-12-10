#include "EnginePCH.h"

#include "Window.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_  HINSTANCE, _In_ LPSTR, _In_ int)
{
    Yukimi::IWindow* Window = new Yukimi::Win32Window();
    Window->Initialize({ 1280.f, 720.f, L"Yukimi" });

    while (Window->ProcessMessage()) {
        // ƒ‹[ƒv–{‘Ì
    }

    Window->Finalize();

    delete Window;

    return 0;
}