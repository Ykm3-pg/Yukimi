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
    auto window = mWindow.lock();
    if (!window) {
        assert(false && "window nullptr");
    }

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
            else if (i == level_num - 1)
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
        mCmdList->Close();

        // コマンドリストをGPUに順に実行させていく為の仕組み
        D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
        HR_CHECK(mDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&mCmdQueue)), "mCmdQueue Init Failed");
    }

    {
        //ダブルバッファリングをするための仕組み

        //各種設定をしてスワップチェーンを生成
        DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
        swapchainDesc.Width = static_cast<UINT>(window->GetWidth());
        swapchainDesc.Height = static_cast<UINT>(window->GetHeight());
        swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;          //色情報の設定
        swapchainDesc.SampleDesc.Count = 1;                         //マルチサンプルしない
        swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;         //バックバッファ用
        swapchainDesc.BufferCount = 2;                              //バッファ数を2つに設定
        swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;   //フリップ後は破棄
        swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1;

        HR_CHECK(mDxgiFactory->CreateSwapChainForHwnd(
            mCmdQueue.Get(),
            window->GetHandle(),
            &swapchainDesc,
            nullptr,
            nullptr,
            &swapchain1), "Swapchain Init Failed");
        swapchain1.As(&mSwapchain);

        HR_CHECK(mDevice->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)), "mFence Init Failed");
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc ={};
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 0;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = 2;

        HR_CHECK(mDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mRTVHeap)), "mRTVHeap Init Failed");
    }

    {
        mBackBufferResources.resize(2);
        mHandles.resize(2);

        //CPUハンドル取得用の変数
        auto start_handle = mRTVHeap->GetCPUDescriptorHandleForHeapStart();
        auto increment_size = mDevice->GetDescriptorHandleIncrementSize(mRTVHeap->GetDesc().Type);

        //バックバッファ用の
        for (int i = 0; i < 2; i++)
        {
            //スワップチェーンからバッファを取得
            mSwapchain->GetBuffer(i, IID_PPV_ARGS(&mBackBufferResources[i]));

            //ディスクリプターヒープのハンドルを取得
            mHandles[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(start_handle, i, increment_size);

            //レンダーターゲットビューの生成
            mDevice->CreateRenderTargetView(mBackBufferResources[i].Get(), nullptr, mHandles[i]);
        }
    }
}

void RenderingModule::Update()
{
    UINT index = mSwapchain->GetCurrentBackBufferIndex();
    auto resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(mBackBufferResources[index].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    mCmdList->ResourceBarrier(1, &resource_barrier);

    mCmdList->OMSetRenderTargets(1, &mHandles[index], true, nullptr);

    float color[] = { 0.f, 1.f, 1.f, 1.0f };

    mCmdList->ClearRenderTargetView(mHandles[index], color, 0, nullptr);

    resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(mBackBufferResources[index].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    mCmdList->ResourceBarrier(1, &resource_barrier);

    mCmdList->Close();

    ID3D12CommandList* cmdlists[] = { mCmdList.Get() };
    mCmdQueue->ExecuteCommandLists(1, cmdlists);

    mSwapchain->Present(1, 0);

    mCmdQueue->Signal(mFence.Get(), ++mFenceValue);

    if (mFence->GetCompletedValue() < mFenceValue)
    {
        HANDLE event = CreateEvent(nullptr, false, false, nullptr);

        mFence->SetEventOnCompletion(mFenceValue, event);

        WaitForSingleObject(event, INFINITE);

        CloseHandle(event);
    }

    mCmdAllocator->Reset();
    mCmdList->Reset(mCmdAllocator.Get(), nullptr);
}

void RenderingModule::Finalize()
{
}

}