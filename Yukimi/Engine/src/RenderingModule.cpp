#include "EnginePCH.h"
#include "RenderingModule.h"

#include "Window.h"

#define HR_CHECK(hr, msg) \
    do { \
        HRESULT _hr = (hr); \
        if (FAILED(_hr)) { \
            assert(false && msg); \
        } \
    } while (0)

namespace Yukimi
{

RenderingModule::RenderingModule()
    : mFenceValue(0)
{
}

RenderingModule::~RenderingModule()
{
}

void RenderingModule::Initialize()
{
    {
#ifdef _DEBUG
        HR_CHECK(D3D12GetDebugInterface(IID_PPV_ARGS(&mDebugger)), "mDebugger Init Failed");
        mDebugger->EnableDebugLayer();
#endif // _DEBUG
    }

    {
        // DXGI = DirectX Graphicss Infrastructure
        // 描画結果を画面に表示するまでのレイヤーを担当する部分
        // グラフィックスアダプターの列挙やディスプレイへの接続情報といったものを扱う

#ifdef _DEBUG
        HR_CHECK(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&mDxgiFactory)), "mDxgiFactory Init Failed");
#else
        HR_CHECK(CreateDXGIFactory1(IID_PPV_ARGS(&mDxgiFactory)), "mDxgiFactory Init Failed");
#endif // _DEBUG

        ComPtr<IDXGIAdapter4> use_adapter;
        HR_CHECK(mDxgiFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&use_adapter)), "GetAdaoter Failed");

        // 対応レベルの配列
        D3D_FEATURE_LEVEL levels[] =
        {
            D3D_FEATURE_LEVEL_12_2,
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
        };

        int level_num = _countof(levels);
        for (int i = 0; i < level_num; i++)
        {
            //採用したアダプターでデバイスを生成
            if (SUCCEEDED(D3D12CreateDevice(use_adapter.Get(), levels[i], IID_PPV_ARGS(&mDevice))))
            {
                break;
            }
            else if (i == level_num)
            {
                HR_CHECK(S_FALSE, "mDevice Init Failed");
            }
        }

    }

    {
        // GPUに対する命令をまとめるオブジェクト
        HR_CHECK(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCmdAllocator)), "mCmdAllocator Init Failed");

        // GPUにまとめて命令を送るもの
        HR_CHECK(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCmdAllocator.Get(), nullptr, IID_PPV_ARGS(&mCmdList)), "mCmdList Init Failed");

        // コマンドリストをGPUに順に実行させていく為の仕組み
        D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
        HR_CHECK(mDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&mCmdQueue)), "mCmdQueue Init Failed");
    }

    {
        //ダブルバッファリングをするための仕組み

        //各種設定をしてスワップチェーンを生成
        DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
        swapchainDesc.Width = static_cast<UINT>(mWindow.lock()->GetWidth());
        swapchainDesc.Height = static_cast<UINT>(mWindow.lock()->GetHeight());
        swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;          //色情報の設定
        swapchainDesc.SampleDesc.Count = 1;                         //マルチサンプルしない
        swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;         //バックバッファ用
        swapchainDesc.BufferCount = 2;                              //バッファ数を2つに設定
        swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;   //フリップ後は破棄
        swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1;

        HR_CHECK(mDxgiFactory->CreateSwapChainForHwnd(
            mCmdQueue.Get(),
            mWindow.lock()->GetHandle(),
            &swapchainDesc,
            nullptr,
            nullptr,
            &swapchain1), "Swapchain Init Failed");
        swapchain1.As(&mSwapchain);

        HR_CHECK(mDevice->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)), "mFence Init Failed");
    }
}

void RenderingModule::Update()
{
    //int bbIdx = mSwapchain->GetCurrentBackBufferIndex();
    //D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
    //rtvH.ptr += bbIdx * mDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    //BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    //BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    //BarrierDesc.Transition.pResource = mBackBuffers[bbIdx];
    //BarrierDesc.Transition.Subresource = 0;

    //BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    //BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    //mCmdList->ResourceBarrier(1, &BarrierDesc);

    //mCmdList->OMSetRenderTargets(1, &rtvH, true, nullptr);

    //float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    //mCmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

    //BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    //BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    //mCmdList->ResourceBarrier(1, &BarrierDesc);

    //mCmdList->Close();

    //ID3D12CommandList* cmdlists[] = { mCmdList };
    //mCmdQueue->ExecuteCommandLists(1, cmdlists);

    //mCmdQueue->Signal(mFence, ++mFenceVal);

    //if (mFence->GetCompletedValue() != mFenceVal)
    //{
    //    HANDLE event = CreateEvent(nullptr, false, false, nullptr);

    //    mFence->SetEventOnCompletion(mFenceVal, event);

    //    WaitForSingleObject(event, INFINITE);

    //    CloseHandle(event);
    //}

    //mCmdAllocator->Reset();
    //mCmdList->Reset(mCmdAllocator, nullptr);

    //mSwapchain->Present(1, 0);
}

void RenderingModule::Finalize()
{
}

}