#pragma once
#include <cstdint>
#include <cmath>
#include <algorithm>

struct ShowcaseInstance {
    Matrix4x4 WVP;
    Matrix4x4 World;
    Vector4 color;
};
struct ShowcaseParticle {
    float x = 0, y = 0, z = 0;
    float size = 0.1f;
    float r = 1, g = 1, b = 1, a = 0;
};
class ParticleShowcase {
public:
    static constexpr uint32_t Count = 512;
    ShowcaseParticle particles[Count]{};
    uint32_t seed = 0;
    void Reset() { ++seed; }
    static float Hash(uint32_t n) {
        n ^= n >> 16; n *= 0x7feb352du; n ^= n >> 15;
        n *= 0x846ca68bu; n ^= n >> 16;
        return float(n & 0xffffffu) / float(0x1000000u);
    }
    void Update(int mode, float t, int count, float size) {
        constexpr float pi = 3.14159265358979323846f;
        for (uint32_t i = 0; i < Count; ++i) {
            ShowcaseParticle& p = particles[i];
            p.a = 0;
            if (int(i) >= count) continue;
            const float a = Hash(i * 13u + seed * 101u + 1u);
            const float b = Hash(i * 17u + seed * 103u + 2u);
            const float c = Hash(i * 19u + seed * 107u + 3u);
            const float d = Hash(i * 23u + seed * 109u + 4u);
            const float angle = a * 2.0f * pi;
            p.size = size * (0.45f + c * 1.2f);
            float age = std::fmod(t * (mode == 0 ? 0.43f : 0.22f) + b, 1.0f);
            if (mode == 0) { // Fireworks: repeated spherical bursts
                float theta = std::acos(2.0f * b - 1.0f);
                float radius = 4.0f * age * (1.0f - 0.15f * age);
                float burstX = (float(int(i % 3) - 1) * 2.6f);
                p.x = burstX + radius * std::sin(theta) * std::cos(angle);
                p.y = 0.7f + radius * std::cos(theta) - 1.8f * age * age;
                p.z = radius * std::sin(theta) * std::sin(angle);
                p.r = 0.75f + 0.25f * a;
                p.g = 0.18f + 0.72f * c;
                p.b = 0.22f + 0.78f * d;
                p.a = (1.0f - age) * (1.0f - age);
            } else if (mode == 1) { // Fountain
                float spread = 1.8f * age * (0.2f + b);
                p.x = spread * std::cos(angle);
                p.y = -2.1f + 8.0f * age - 8.0f * age * age + c * 0.6f;
                p.z = spread * std::sin(angle);
                p.r = 0.15f + 0.35f * c;
                p.g = 0.5f + 0.4f * d;
                p.b = 1.0f;
                p.a = (((1.0f - age) * 2.0f) < 1.0f ? ((1.0f - age) * 2.0f) : 1.0f);
            } else { // Galaxy: spiral arms
                float phase = angle + t * 0.75f + (i % 4) * pi * 0.5f;
                float radius = 0.3f + 4.3f * std::sqrt(b);
                float spin = phase + radius * 1.5f;
                p.x = radius * std::cos(spin);
                p.y = (c - 0.5f) * (0.3f + 0.5f * radius);
                p.z = radius * std::sin(spin);
                p.r = 0.3f + 0.7f * c;
                p.g = 0.25f + 0.45f * d;
                p.b = 1.0f;
                p.a = 0.4f + 0.6f * a;
            }
        }
    }
};
