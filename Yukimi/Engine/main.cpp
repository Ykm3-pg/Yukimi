#include "EnginePCH.h"

#include "Window.h"
#include "RenderingModule.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_  HINSTANCE, _In_ LPSTR, _In_ int)
{
    std::shared_ptr<Yukimi::Win32Window> Window = std::make_shared<Yukimi::Win32Window>();
    Window->Initialize(1280, 720, L"Yukimi");

    std::shared_ptr<Yukimi::RenderingModule> RenderingModule = std::make_shared<Yukimi::RenderingModule>();
    RenderingModule->SetWindow(Window);
    RenderingModule->Initialize();

    while (Window->ProcessMessage()) {
        // ƒ‹[ƒv–{‘Ì
        RenderingModule->Update();
    }

    RenderingModule->Finalize();
    Window->Finalize();

    return 0;
}