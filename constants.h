#pragma once
#include <chrono>

static const unsigned int updateFreq = 60;
static constexpr std::chrono::milliseconds updatePeriod{1000 / updateFreq};
