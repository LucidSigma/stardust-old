#pragma once
#ifndef RECT_H
#define RECT_H

#include <optional>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>

namespace stardust
{
	namespace rect
	{
		using Point = SDL_Point;
		using Rect = SDL_Rect;
		using Line = std::pair<Point, Point>;

		[[nodiscard]] Point Create(const int x, const int y);
		[[nodiscard]] Rect Create(const int x, const int y, const unsigned int width, const unsigned int height);
		[[nodiscard]] Line Create(const Point& firstPoint, const Point& secondPoint);

		[[nodiscard]] bool IsEmpty(const Rect& rect);
		[[nodiscard]] extern unsigned int GetArea(const Rect& rect);

		[[nodiscard]] extern bool IsAnyPointEnclosed(const std::vector<Point>& points, const Rect& clippingRect);
		[[nodiscard]] extern Rect GetMinimumEnclosingRect(const std::vector<Point>& points);
		[[nodiscard]] extern Rect GetMinimumEnclosingRect(const std::vector<Point>& points, const Rect& clippingRect);

		[[nodiscard]] extern bool IsPointInRect(const Point& point, const Rect& rect);
		[[nodiscard]] extern bool HasIntersection(const Rect& a, const Rect& b);
		[[nodiscard]] extern bool HasIntersection(const Rect& rect, const Line& line);
		[[nodiscard]] extern std::optional<Rect> GetIntersection(const Rect& a, const Rect& b);
		[[nodiscard]] extern Rect GetUnion(const Rect& a, const Rect& b);
		extern void ClipLineToRect(const Rect& rect, Line& line);
	}
}

#endif