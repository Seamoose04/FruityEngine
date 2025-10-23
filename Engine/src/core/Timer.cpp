#include "Timer.h"

using Clock = std::chrono::high_resolution_clock;

Timer::Timer() {
    _last = Clock::now();
}

float Timer::GetDeltaTime() {
    auto now = Clock::now();
    std::chrono::duration<float> diff = now - _last;
    _last = now;
    float dt = diff.count();
    _total += dt;
    return dt;
}

float Timer::GetTotalTime() const {
    return _total;
}