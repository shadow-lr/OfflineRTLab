#pragma once

#include <algorithm>
#include <iostream>
#include <chrono>
#include <ranges>

template <std::integral T>
constexpr auto range(T end)
{
	return std::ranges::views::iota(static_cast<T>(0), end);
}

template <std::integral T>
constexpr auto range(T start ,T end)
{
	return std::ranges::views::itoa(start, end);
}