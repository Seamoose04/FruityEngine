#pragma once

#include <chrono>

class Timer {
public:
    Timer();
    float GetDeltaTime();
    float GetTotalTime() const;

private:
    std::chrono::high_resolution_clock::time_point _last;
    float _total = 0.0f;
};