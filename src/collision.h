#pragma once

#include "singleton.h"
// TODO forward declaration.
#include "nmgr/line.hpp"
#include "nmgr/point.h"

namespace nmgr {
namespace shapes {

class IShape {
public:
  virtual bool isPointInside(Point const &P) = 0;
  virtual ~IShape(){};
};

// class BoundingBox : public IShape {};
class Sphere : public IShape {
  Point Center{};
  float R_sq{};

public:
  bool isPointInside(Point const &P) override {
    return Point::length_sq(P, Center) <= R_sq;
  }
};
// class BoundingEllipse{};
// class BoundingCapsule{};
} // namespace shapes

namespace collision {

class ICollided {
public:
  virtual bool isPointInside(Point const &P) const = 0;
  virtual ~ICollided(){};
};

class Collision : public ICollided {
  // LineBase is actually pointer to points but easily can be treated as
  // Line<Width>.
  LineBase *Line{};
  shapes::IShape *Shape{};

public:
  bool isPointInside(Point const &P) const override {
    return Shape->isPointInside(P);
  }
};

// TODO temporary collisionmanager
class CollManager : public singleton<CollManager> {
  friend singleton;
  CollManager() {}

  std::vector<shapes::IShape *> Shapes{};
  std::vector<ICollided *> Collisions{};

public:
  // TODO template with perfect forwarding.
  shapes::IShape *getShape() {
    shapes::Sphere *Shape = new shapes::Sphere{};
    Shapes.push_back(Shape);
    return Shape;
  }
  // TODO template with perfect forwarding.
  ICollided *getCollision() {
    Collision *C = new Collision{};
    Collisions.push_back(C);
    return C;
  }

  ~CollManager() {
    for (auto x : Shapes)
      delete x;
    for (auto x : Collisions)
      delete x;
  }
};

} // namespace collision
} // namespace nmgr
