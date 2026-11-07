#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <chrono>
#include <cstdint>

class Window;

class DirectXCommon {
public:
    static constexpr UINT kBackBufferCount = 2;
    void Initialize(Window* window);
    void PreDraw(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle);
    void PostDraw();
    void Finalize();

    // ComPtrのGet()で通常のポインタを取り出す
    ID3D12Device* GetDeviceRaw() const { return device_.Get(); }
    ID3D12GraphicsCommandList* GetCommandListRaw() const { return commandList_.Get(); }
    Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() const { return device_; }
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() const { return commandList_; }
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetRTVDescriptorHeap() const { return rtvDescriptorHeap_; }

private:
    void WaitForGPU();
    void LimitFPS();

    Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
    Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter_;
    Microsoft::WRL::ComPtr<ID3D12Device> device_;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_;
    Microsoft::WRL::ComPtr<ID3D12Resource> backBuffers_[kBackBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[kBackBufferCount]{};
    HANDLE fenceEvent_ = nullptr;
    uint64_t fenceValue_ = 0;
    std::chrono::steady_clock::time_point nextFrameTime_{};
    bool initialized_ = false;
};
