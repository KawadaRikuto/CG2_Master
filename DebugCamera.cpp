#include "DebugCamera.h"
#include <dinput.h>

using namespace DirectX;

void DebugCamera::Initialize(int32_t clientWidth, int32_t clientHeight, HWND hwnd) {
    clientWidth_ = clientWidth;
    clientHeight_ = clientHeight;
    hwnd_ = hwnd;

    rotation_ = { 0.0f, 0.0f, 0.0f };
    translation_ = { 0.0f, 0.0f, -10.0f };

    XMStoreFloat4x4(&matRot_, XMMatrixIdentity());
    XMStoreFloat4x4(&viewMatrix_, XMMatrixIdentity());
    XMStoreFloat4x4(&projectionMatrix_, XMMatrixIdentity());

    GetCursorPos(&previousMousePosition_);
    UpdateMatrices();
}

void DebugCamera::Update(const unsigned char key[256]) {
    UpdateInput(key);
    UpdateMatrices();
}

void DebugCamera::SetTranslation(float x, float y, float z) {
    translation_ = { x, y, z };
    UpdateMatrices();
}

void DebugCamera::SetRotation(float x, float y, float z) {
    rotation_ = { x, y, z };
    const XMMATRIX rotateMatrix =
        XMMatrixRotationX(rotation_.x) *
        XMMatrixRotationY(rotation_.y) *
        XMMatrixRotationZ(rotation_.z);
    XMStoreFloat4x4(&matRot_, rotateMatrix);
    UpdateMatrices();
}

void DebugCamera::UpdateInput(const unsigned char key[256]) {
    XMFLOAT3 deltaRotation{ 0.0f, 0.0f, 0.0f };

    if (key[DIK_UP] & 0x80) { deltaRotation.x += rotateSpeed_; }
    if (key[DIK_DOWN] & 0x80) { deltaRotation.x -= rotateSpeed_; }
    if (key[DIK_LEFT] & 0x80) { deltaRotation.y -= rotateSpeed_; }
    if (key[DIK_RIGHT] & 0x80) { deltaRotation.y += rotateSpeed_; }
    if (key[DIK_Q] & 0x80) { deltaRotation.z -= rotateSpeed_; }
    if (key[DIK_E] & 0x80) { deltaRotation.z += rotateSpeed_; }

    const bool rightButtonDown = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
    POINT currentMousePosition{};
    GetCursorPos(&currentMousePosition);

    if (rightButtonDown) {
        if (isRightDragging_) {
            const LONG deltaX = currentMousePosition.x - previousMousePosition_.x;
            const LONG deltaY = currentMousePosition.y - previousMousePosition_.y;
            deltaRotation.x += static_cast<float>(deltaY) * 0.003f;
            deltaRotation.y += static_cast<float>(deltaX) * 0.003f;
        }
        isRightDragging_ = true;
    } else {
        isRightDragging_ = false;
    }
    previousMousePosition_ = currentMousePosition;

    const XMMATRIX currentRotation =
        XMMatrixRotationX(deltaRotation.x) *
        XMMatrixRotationY(deltaRotation.y) *
        XMMatrixRotationZ(deltaRotation.z);

    const XMMATRIX oldRotation = XMLoadFloat4x4(&matRot_);
    const XMMATRIX accumulatedRotation = oldRotation * currentRotation;
    XMStoreFloat4x4(&matRot_, accumulatedRotation);

    rotation_.x += deltaRotation.x;
    rotation_.y += deltaRotation.y;
    rotation_.z += deltaRotation.z;

    const XMVECTOR localRight = XMVector3TransformNormal(
        XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), accumulatedRotation);
    const XMVECTOR localUp = XMVector3TransformNormal(
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), accumulatedRotation);
    const XMVECTOR localForward = XMVector3TransformNormal(
        XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), accumulatedRotation);

    XMVECTOR move = XMVectorZero();

    if (key[DIK_W] & 0x80) { move = XMVectorAdd(move, XMVectorScale(localForward, moveSpeed_)); }
    if (key[DIK_S] & 0x80) { move = XMVectorSubtract(move, XMVectorScale(localForward, moveSpeed_)); }
    if (key[DIK_A] & 0x80) { move = XMVectorSubtract(move, XMVectorScale(localRight, moveSpeed_)); }
    if (key[DIK_D] & 0x80) { move = XMVectorAdd(move, XMVectorScale(localRight, moveSpeed_)); }
    if (key[DIK_R] & 0x80) { move = XMVectorAdd(move, XMVectorScale(localUp, moveSpeed_)); }
    if (key[DIK_F] & 0x80) { move = XMVectorSubtract(move, XMVectorScale(localUp, moveSpeed_)); }

    XMVECTOR position = XMLoadFloat3(&translation_);
    position = XMVectorAdd(position, move);
    XMStoreFloat3(&translation_, position);
}

void DebugCamera::UpdateMatrices() {
    const XMMATRIX rotationMatrix = XMLoadFloat4x4(&matRot_);
    const XMMATRIX translationMatrix = XMMatrixTranslation(
        translation_.x, translation_.y, translation_.z);

    const XMMATRIX cameraWorldMatrix = rotationMatrix * translationMatrix;
    const XMMATRIX viewMatrix = XMMatrixInverse(nullptr, cameraWorldMatrix);

    const float aspectRatio = static_cast<float>(clientWidth_) /
        static_cast<float>(clientHeight_);
    const XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(
        0.45f, aspectRatio, 0.1f, 100.0f);

    XMStoreFloat4x4(&viewMatrix_, viewMatrix);
    XMStoreFloat4x4(&projectionMatrix_, projectionMatrix);
}