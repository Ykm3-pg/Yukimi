#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "d3dx12.h"

#include "IModule.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace Yukimi
{

class Win32Window;

class RenderingModule : public IModule
{
public:
    /// <summary>
    /// 
    /// </summary>
    RenderingModule();

    /// <summary>
    /// 
    /// </summary>
    virtual ~RenderingModule();

    /// <summary>
    /// 
    /// </summary>
    virtual void Initialize() override;

    /// <summary>
    /// 
    /// </summary>
    virtual void Update() override;

    /// <summary>
    /// 
    /// </summary>
    virtual void Finalize() override;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="window"></param>
    void SetWindow(std::weak_ptr<Win32Window> window) {
        mWindow = window;
    }

private:
    std::weak_ptr<Win32Window> mWindow;

    ComPtr<IDXGIFactory7> mDxgiFactory;

    ComPtr<ID3D12Device14> mDevice;

    ComPtr<ID3D12GraphicsCommandList10> mCmdList;

    ComPtr<ID3D12CommandAllocator> mCmdAllocator;

    ComPtr<ID3D12CommandQueue> mCmdQueue;

    ComPtr<IDXGISwapChain4> mSwapchain;

    ComPtr<ID3D12Fence1> mFence;

    UINT64 mFenceValue;

    //ダブルバッファリング用
    std::vector<ComPtr<ID3D12Resource>> mBackBufferResources;

    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> mHandles;


    D3D12_VIEWPORT mViewport;

    D3D12_RECT mScissorRect;

    ComPtr<ID3D12DescriptorHeap> mRTVHeap;


#ifdef _DEBUG
    Microsoft::WRL::ComPtr<ID3D12Debug> mDebugger;
#endif // _DEBUG
};

}
