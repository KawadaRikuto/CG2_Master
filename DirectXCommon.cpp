#include "DirectXCommon.h"
#include "Window.h"
#include <cassert>
#include <thread>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

void DirectXCommon::Initialize(Window* window) {
    assert(window != nullptr && window->GetHwnd() != nullptr);
#ifdef _DEBUG
    ComPtr<ID3D12Debug1> debug;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf())))) {
        debug->EnableDebugLayer();
    }
#endif
    HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(dxgiFactory_.GetAddressOf()));
    assert(SUCCEEDED(hr));

    for (UINT i = 0; ; ++i) {
        ComPtr<IDXGIAdapter4> candidate;
        if (dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
            IID_PPV_ARGS(candidate.GetAddressOf())) == DXGI_ERROR_NOT_FOUND) break;
        DXGI_ADAPTER_DESC3 desc{};
        candidate->GetDesc3(&desc);
        if (!(desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
            adapter_ = candidate;
            break;
        }
    }
    assert(adapter_ != nullptr);
    const D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_11_0 };
    for (D3D_FEATURE_LEVEL level : levels) {
        hr = D3D12CreateDevice(adapter_.Get(), level, IID_PPV_ARGS(device_.GetAddressOf()));
        if (SUCCEEDED(hr)) break;
    }
    assert(device_ != nullptr);

#ifdef _DEBUG
    ComPtr<ID3D12InfoQueue> infoQueue;
    if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(infoQueue.GetAddressOf())))) {
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
    }
#endif
    D3D12_COMMAND_QUEUE_DESC queueDesc{};
    hr = device_->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(commandQueue_.GetAddressOf()));
    assert(SUCCEEDED(hr));
    hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator_.GetAddressOf()));
    assert(SUCCEEDED(hr));
    hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr,
        IID_PPV_ARGS(commandList_.GetAddressOf()));
    assert(SUCCEEDED(hr));

    DXGI_SWAP_CHAIN_DESC1 swapDesc{};
    swapDesc.Width = Window::kWindowWidth;
    swapDesc.Height = Window::kWindowHeight;
    swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.BufferCount = kBackBufferCount;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    ComPtr<IDXGISwapChain1> swapChain1;
    hr = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), window->GetHwnd(), &swapDesc,
        nullptr, nullptr, swapChain1.GetAddressOf());
    assert(SUCCEEDED(hr));
    hr = swapChain1.As(&swapChain_);
    assert(SUCCEEDED(hr));

    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NumDescriptors = kBackBufferCount;
    hr = device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(rtvDescriptorHeap_.GetAddressOf()));
    assert(SUCCEEDED(hr));

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
    UINT step = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    for (UINT i = 0; i < kBackBufferCount; ++i) {
        hr = swapChain_->GetBuffer(i, IID_PPV_ARGS(backBuffers_[i].GetAddressOf()));
        assert(SUCCEEDED(hr));
        rtvHandles_[i] = handle;
        device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc, handle);
        handle.ptr += step;
    }

    hr = device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence_.GetAddressOf()));
    assert(SUCCEEDED(hr));
    fenceEvent_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    assert(fenceEvent_ != nullptr);
    nextFrameTime_ = std::chrono::steady_clock::now();
    initialized_ = true;
}

void DirectXCommon::PreDraw(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) {
    const UINT index = swapChain_->GetCurrentBackBufferIndex();
    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = backBuffers_[index].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList_->ResourceBarrier(1, &barrier);
    commandList_->OMSetRenderTargets(1, &rtvHandles_[index], FALSE, &dsvHandle);
    const float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f };
    commandList_->ClearRenderTargetView(rtvHandles_[index], clearColor, 0, nullptr);
    commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void DirectXCommon::WaitForGPU() {
    ++fenceValue_;
    HRESULT hr = commandQueue_->Signal(fence_.Get(), fenceValue_);
    assert(SUCCEEDED(hr));
    if (fence_->GetCompletedValue() < fenceValue_) {
        hr = fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);
        assert(SUCCEEDED(hr));
        WaitForSingleObject(fenceEvent_, INFINITE);
    }
}

void DirectXCommon::LimitFPS() {
    using namespace std::chrono;
    constexpr auto frame = duration<double>(1.0 / 60.0);
    nextFrameTime_ += duration_cast<steady_clock::duration>(frame);
    const auto now = steady_clock::now();
    if (nextFrameTime_ > now) std::this_thread::sleep_until(nextFrameTime_);
    else nextFrameTime_ = now;
}

void DirectXCommon::PostDraw() {
    const UINT index = swapChain_->GetCurrentBackBufferIndex();
    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = backBuffers_[index].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList_->ResourceBarrier(1, &barrier);
    HRESULT hr = commandList_->Close();
    assert(SUCCEEDED(hr));
    ID3D12CommandList* lists[] = { commandList_.Get() };
    commandQueue_->ExecuteCommandLists(1, lists);
    hr = swapChain_->Present(1, 0);
    assert(SUCCEEDED(hr));
    WaitForGPU();
    hr = commandAllocator_->Reset();
    assert(SUCCEEDED(hr));
    hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
    assert(SUCCEEDED(hr));
    LimitFPS();
}

void DirectXCommon::Finalize() {
    if (initialized_) WaitForGPU();
    if (fenceEvent_) { CloseHandle(fenceEvent_); fenceEvent_ = nullptr; }
    // DirectX12オブジェクトはすべてComPtrで保持し、Release()を直接呼ばない
    for (auto& buffer : backBuffers_) buffer.Reset();
    rtvDescriptorHeap_.Reset();
    swapChain_.Reset();
    commandList_.Reset();
    commandAllocator_.Reset();
    commandQueue_.Reset();
    fence_.Reset();
    device_.Reset();
    adapter_.Reset();
    dxgiFactory_.Reset();
    initialized_ = false;
}
