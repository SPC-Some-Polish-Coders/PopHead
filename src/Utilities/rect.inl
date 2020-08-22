#include "rect.hpp"

template<typename T>
Rect<T>::Rect() :x(0), y(0), w(0), h(0) {}

template<typename T>
Rect<T>::Rect(T x, T y, T w, T h) :x(x), y(y), w(w), h(h) {}

template<typename T>
Rect<T>::Rect(sf::Vector2<T> pos, sf::Vector2<T> size) :pos(pos), size(size) {}

template <typename T>
template <typename U>
Rect<T>::Rect(const Rect<U>& rectangle) :
	x(Cast<T>(rectangle.x)),
	y(Cast<T>(rectangle.y)),
	w(Cast<T>(rectangle.w)),
	h(Cast<T>(rectangle.h)) 
{}

template <typename T>
void Rect<T>::setCenter(Vec2Base<T> center)
{
	x = center.x - w / 2;
	y = center.y - h / 2;
}

template <typename T>
void Rect<T>::setSizeWithFixedCenter(Vec2Base<T> size)
{
	Vec2 previousCenter = center();
	this->size = size;
	setCenter(previousCenter);
}

template<typename T>
T Rect<T>::right() const
{
	return x + w;
}

template<typename T>
T Rect<T>::bottom() const
{
	return y + h;
}

template<typename T>
sf::Vector2<T> Rect<T>::center() const
{
	return {x + w / 2, y + h / 2};
}

template<typename T>
sf::Vector2<T> Rect<T>::topRight() const
{
	return {right(), y};
}

template<typename T>
sf::Vector2<T> Rect<T>::bottomLeft() const
{
	return {x, bottom()};
}

template<typename T>
sf::Vector2<T> Rect<T>::bottomRight() const
{
	return {right(), bottom()};
}

template<typename T>
Vec2Base<T> Rect<T>::topCenter() const
{
	return {x + w / 2, y};
}

template<typename T>
Vec2Base<T> Rect<T>::bottomCenter() const
{
	return {x + w / 2, bottom()};
}

template<typename T>
Vec2Base<T> Rect<T>::leftCenter() const
{
	return {x, y + h / 2};
}

template<typename T>
Vec2Base<T> Rect<T>::rightCenter() const
{
	return {right(), y + h / 2};
}

template<typename T>
bool Rect<T>::contains(const sf::Vector2<T>& point) const
{
	auto r = right();
	auto b = bottom();

	T minX = std::min(x, r);
	T maxX = std::max(x, r);
	T minY = std::min(y, b);
	T maxY = std::max(y, b);

	return (point.x >= minX) && (point.x <= maxX) && (point.y >= minY) && (point.y <= maxY);
}

template<typename T>
bool intersect(const Rect<T>& a, const Rect<T>& b)
{
	// this function only works properly for rects with positive w and h
	return a.x < b.x + b.w
		&& a.x + a.w > b.x
		&& a.y < b.y + b.h
		&& a.y + a.h > b.y;
}

template<typename T>
bool fullyIntersect(const Rect<T>& a, const Rect<T>& b)
{
	return a.contains(b.pos) && a.contains(b.topRight()) &&
	       a.contains(b.bottomLeft()) && a.contains(b.bottomRight());
}

template<typename T>
bool intersectFlipAllowed(const Rect<T>& a, const Rect<T>& b)
{
    Rect<T> intersection;
    return a.intersects(b, intersection);
}

template<typename T>
bool Rect<T>::intersects(const Rect<T>& rectangle, Rect<T>& intersection) const
{
    // Rectangles with negative (fliped) dimensions are allowed, so we must handle them correctly

    // Compute the min and max of the first rectangle on both axes
    T r1MinX = std::min(x, Cast<T>(x + w));
    T r1MaxX = std::max(x, Cast<T>(x + w));
    T r1MinY = std::min(y, Cast<T>(y + h));
    T r1MaxY = std::max(y, Cast<T>(y + h));

    // Compute the min and max of the second rectangle on both axes
    T r2MinX = std::min(rectangle.x, Cast<T>(rectangle.x + rectangle.w));
    T r2MaxX = std::max(rectangle.x, Cast<T>(rectangle.x + rectangle.w));
    T r2MinY = std::min(rectangle.y, Cast<T>(rectangle.y + rectangle.h));
    T r2MaxY = std::max(rectangle.y, Cast<T>(rectangle.y + rectangle.h));

    // Compute the intersection boundaries
    T interLeft   = std::max(r1MinX, r2MinX);
    T interTop    = std::max(r1MinY, r2MinY);
    T interRight  = std::min(r1MaxX, r2MaxX);
    T interBottom = std::min(r1MaxY, r2MaxY);

    // If the intersection is valid (positive non zero area), then there is an intersection
    if ((interLeft < interRight) && (interTop < interBottom))
    {
        intersection = Rect<T>(interLeft, interTop, interRight - interLeft, interBottom - interTop);
        return true;
    }
    else
    {
        intersection = Rect<T>(0, 0, 0, 0);
        return false;
    }
}

template<typename T>
bool Rect<T>::touch(const Rect<T>& rect, sf::Vector2<i16>& direction) const
{
	bool collideOnX = x < rect.right() && right() > rect.x;
	bool collideOnY = y < rect.bottom() && bottom() > rect.y;

	if (!(collideOnX ^ collideOnY))
	{
		direction = { 0, 0 };
		return false;
	}

	if (collideOnX)
	{
		if (y == rect.bottom())
		{
			direction = { 0, 1 };
			return true;
		}
		if (bottom() == rect.y)
		{
			direction = { 0, -1 };
			return true;
		}
	}
	if (collideOnY)
	{
		if (x == rect.right())
		{
			direction = { 1, 0 };
			return true;
		}
		if (right() == rect.x)
		{
			direction = { -1, 0 };
			return true;
		}
	}

	direction = { 0, 0 };
	return false;
}

template<typename T>
std::vector<Rect<T>> connectRects(std::vector<std::pair<Rect<T>, int>>& rects, float maxGapSize, float maxAlignSize)
{
	std::vector<FloatRect> addedRects;

	int n = rects.size() * 2;
	bool aux[n];

	for (auto& e : aux)
	{
		e = false;
	}

	for (int i = 0; i < rects.size(); i++)
	{
		auto rect1 = rects[i].first;
		for (int j = 0; j < rects.size(); j++)
		{
			if (i == j)
				continue;

			auto rect2 = rects[j].first;

			int index = rects[i].second + rects[j].second;

			if (aux[index]) // Means that these two rects are already connected
				continue;

			float gapX = (rect1.x > rect2.x) ? rect1.x - rect2.right() : rect2.x - rect1.right(),
				  gapY = (rect1.y > rect2.y) ? rect1.y - rect2.bottom() : rect2.y - rect1.bottom();

			float distX = std::abs(rect1.x - rect2.x),
				  distY = std::abs(rect1.y - rect2.y);


			if (gapX <= maxGapSize && distY <= std::min(rect1.h, rect2.h)) // Alignment on X axis
			{
				if (rect1.x > rect2.x)
					addedRects.emplace_back(FloatRect(rect2.right(), rect2.y, gapX, rect2.h));
				else
					addedRects.emplace_back(FloatRect(rect1.right(), rect1.y, gapX, rect1.h));

				aux[index] = true;
			}
			else
			if (gapY <= maxGapSize && distX <= std::min(rect1.w, rect2.w)) // Alignment on Y axis
			{
				if (rect1.y > rect2.y)
					addedRects.emplace_back(FloatRect(rect2.x, rect2.bottom(), rect2.w, gapY));
				else
					addedRects.emplace_back(FloatRect(rect1.x, rect1.bottom(), rect1.w, gapY));

				aux[index] = true;
			}
		}
	}

	return addedRects;
}

template <typename T>
bool operator ==(const Rect<T>& a, const Rect<T>& b)
{
	return a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h;
}

template <typename T>
bool operator !=(const Rect<T>& a, const Rect<T>& b)
{
	return !(a == b); 
}

