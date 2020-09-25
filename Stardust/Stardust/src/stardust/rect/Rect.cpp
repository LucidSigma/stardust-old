#include "Rect.h"

namespace stardust
{
	namespace rect
	{
		[[nodiscard]] Point Create(const int x, const int y)
		{
			return Point{ x, y };
		}

		[[nodiscard]] Rect Create(const int x, const int y, const unsigned int width, const unsigned int height)
		{
			return Rect{ x, y, static_cast<int>(width), static_cast<int>(height) };
		}

		[[nodiscard]] Line Create(const Point& firstPoint, const Point& secondPoint)
		{
			return Line{ firstPoint, secondPoint };
		}

		[[nodiscard]] bool IsEmpty(const Rect& rect)
		{
			return static_cast<bool>(SDL_RectEmpty(&rect));
		}

		[[nodiscard]] unsigned int GetArea(const Rect& rect)
		{
			return rect.w * rect.h;
		}

		[[nodiscard]] bool IsAnyPointEnclosed(const std::vector<Point>& points, const Rect& clippingRect)
		{
			return static_cast<bool>(SDL_EnclosePoints(points.data(), static_cast<int>(points.size()), &clippingRect, nullptr));
		}

		[[nodiscard]] Rect GetMinimumEnclosingRect(const std::vector<Point>& points)
		{
			Rect minimumEnclosingRect{ };
			SDL_EnclosePoints(points.data(), static_cast<int>(points.size()), nullptr, &minimumEnclosingRect);

			return minimumEnclosingRect;
		}

		[[nodiscard]] Rect GetMinimumEnclosingRect(const std::vector<Point>& points, const Rect& clippingRect)
		{
			Rect minimumEnclosingRect{ };
			SDL_EnclosePoints(points.data(), static_cast<int>(points.size()), &clippingRect, &minimumEnclosingRect);

			return minimumEnclosingRect;
		}

		[[nodiscard]] bool IsPointInRect(const Point& point, const Rect& rect)
		{
			return static_cast<bool>(SDL_PointInRect(&point, &rect));
		}

		[[nodiscard]] bool HasIntersection(const Rect& a, const Rect& b)
		{
			return static_cast<bool>(SDL_HasIntersection(&a, &b));
		}

		[[nodiscard]] bool HasIntersection(const Rect& rect, const Line& line)
		{
			Line lineCopy = line;

			return static_cast<bool>(SDL_IntersectRectAndLine(&rect, &lineCopy.first.x, &lineCopy.first.y, &lineCopy.second.x, &lineCopy.second.y));
		}

		[[nodiscard]] std::optional<Rect> GetIntersection(const Rect& a, const Rect& b)
		{
			Rect intersection{ };
			const bool doesIntersectionExist = static_cast<bool>(SDL_IntersectRect(&a, &b, &intersection));

			return doesIntersectionExist ? std::optional<Rect>(intersection) : std::nullopt;
		}

		[[nodiscard]] Rect GetUnion(const Rect& a, const Rect& b)
		{
			Rect rectUnion{ };
			SDL_UnionRect(&a, &b, &rectUnion);

			return rectUnion;
		}

		void ClipLineToRect(const Rect& rect, Line& line)
		{
			SDL_IntersectRectAndLine(&rect, &line.first.x, &line.first.y, &line.second.x, &line.second.y);
		}
	}
}