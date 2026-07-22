#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <cstdint>

class DebugCamera {
public:
    void Initialize(int32_t clientWidth, int32_t clientHeight, HWND hwnd);
    void Update(const unsigned char key[256]);

    const DirectX::XMFLOAT4X4& GetViewMatrix() const { return viewMatrix_; }
    const DirectX::XMFLOAT4X4& GetProjectionMatrix() const { return projectionMatrix_; }
    const DirectX::XMFLOAT3& GetTranslation() const { return translation_; }
    const DirectX::XMFLOAT3& GetRotation() const { return rotation_; }

    void SetTranslation(float x, float y, float z);
    void SetRotation(float x, float y, float z);

private:
    void UpdateInput(const unsigned char key[256]);
    void UpdateMatrices();

    HWND hwnd_ = nullptr;
    DirectX::XMFLOAT3 rotation_{ 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 translation_{ 0.0f, 0.0f, -10.0f };
    DirectX::XMFLOAT4X4 matRot_{};
    DirectX::XMFLOAT4X4 viewMatrix_{};
    DirectX::XMFLOAT4X4 projectionMatrix_{};

    float moveSpeed_ = 0.15f;
    float rotateSpeed_ = 0.02f;
    int32_t clientWidth_ = 1280;
    int32_t clientHeight_ = 720;

    POINT previousMousePosition_{};
    bool isRightDragging_ = false;
};