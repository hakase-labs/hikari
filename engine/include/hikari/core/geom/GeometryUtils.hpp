#ifndef HIKARI_CORE_GEOM_GEOMETRYUTILS
#define HIKARI_CORE_GEOM_GEOMETRYUTILS

#include "hikari/core/geom/BoundingBox.hpp"
#include "hikari/core/geom/Rectangle2D.hpp"

namespace hikari {
namespace geom {

    /**
     * Checks if a bounding box fully contains a given rectangle.
     * @param  box  a BoundingBox (the "container")
     * @param  rect a Rectangle2D (the "containee")
     * @return      true if rect is inside box
     */
    template <typename T>
    bool contains(const BoundingBox<T>& box, const Rectangle2D<T>& rect) {
        return (box.contains(rect.getLeft(), rect.getTop()) &&
                box.contains(rect.getRight(), rect.getTop()) &&
                box.contains(rect.getLeft(), rect.getBottom()) &&
                box.contains(rect.getRight(), rect.getBottom()));
    }

    /**
     * Checks if a bounding box intersects with a rectangle.
     * @param  box  a BoundingBox
     * @param  rect a Rectangle2D
     * @return      true if box and rect overlap
     */
    template <typename T>
    bool intersects(const BoundingBox<T>& box, const Rectangle2D<T>& rect) {
        return !(box.getLeft() > rect.getRight() || box.getRight() < rect.getLeft() ||
                box.getTop() > rect.getBottom() || box.getBottom() < rect.getTop());
    }

    /**
     * Checks if a rectangle fully contains a given bounding box.
     * @param  rect a Rectangle2D (the "containee")
     * @param  box  a BoundingBox (the "container")
     * @return      true if box is inside rect
     */
    template <typename T>
    bool contains(const Rectangle2D<T>& rect, const BoundingBox<T>& box) {
        return (rect.contains(box.getLeft(), box.getTop()) &&
                rect.contains(box.getRight(), box.getTop()) &&
                rect.contains(box.getLeft(), box.getBottom()) &&
                rect.contains(box.getRight(), box.getBottom()));
    }

    /**
     * Checks if a rectangle intersects with a bounding box.
     * @param  rect a Rectangle2D
     * @param  box  a BoundingBox
     * @return      true if rect and box overlap
     */
    template <typename T>
    bool intersects(const Rectangle2D<T>& rect, const BoundingBox<T>& box) {
        return !(rect.getLeft() > box.getRight() || rect.getRight() < box.getLeft() ||
                rect.getTop() > box.getBottom() || rect.getBottom() < box.getTop());
    }

    /**
     * Returns the intersection of two BoundingBoxes. If the two BoundingBoxes 
     * do not actually intersect then the result is undefined.
     *
     * @return new BoundingBox representing the intersection of a and b
     */
    template <typename T>
    BoundingBox<T> intersection(const BoundingBox<T>& a, const BoundingBox<T>& b) {
        T top = std::max(a.getTop(), b.getTop());
        T right = std::min(a.getRight(), b.getRight());
        T bottom = std::min(a.getBottom(), b.getBottom());
        T left = std::max(a.getLeft(), b.getLeft());

        return BoundingBox<T>(top, left, (right - left), (bottom - top));
    }

} // hikari::geom
} // hikari

#endif // HIKARI_CORE_GEOM_GEOMETRYUTILS