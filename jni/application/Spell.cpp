#include <zenilib.h>
#include <iostream>
#include "Spell.h"
#include "Model_state.h"

using namespace Zeni;
using std::cout; using std::endl;

namespace Flame {

  void Spell::update(float time)
  {
    life_time -= time;
    if (life_time < 0.f)
      active = false;
  }

  Moving_spell::Moving_spell(const Point2f& location_,
                             const Vector2f& orientation_,
                             const Vector2f& size_,
                             float speed_,
                             float life_time_) :
    Spell(life_time_),
    Moving_object(speed_, location_),
    size(size_)
  {
    scale = Model_state::get_instance()->get_scale();
    Moving_object::set_orientation(orientation_);
    Moving_object::set_moving(true);
    relative_location = (get_center_location() - Model_state::get_instance()->get_center_location()) * scale + Point2f(400.0f, 300.0f);
  }

  void Moving_spell::update(float time)
  {
    Spell::update(time);
    if (!Spell::is_active())
      return;
    Moving_object::update_location(time);
    scale = Model_state::get_instance()->get_scale();
    relative_location = (get_center_location() - Model_state::get_instance()->get_center_location()) * scale + Point2f(400.0f, 300.0f);
  }

  void Moving_spell::render(const String& texture, Color filter) const
  {
    Vector2f orientation = get_current_orientation();
    float theta = orientation.angle_between(Vector2f(0.f, 1.f));
    if (orientation.x < 0.f)
      theta = 2 * Global::pi - theta;
    theta += Global::pi;
    render_image(texture, // which texture to use
                 relative_location - scale * size / 2, // upper-left corner
                 relative_location + scale * size / 2, // lower-right corner
                 theta, // rotation in radians
                 1.f, // scaling factor
                 relative_location, // point to rotate & scale about
                 false,
                 filter);
  }

  void Moving_spell_circle::update_body()
  {
    Point2f location = get_center_location();
    Vector2f size = get_size();
    body = Collision::Capsule(Point3f(location.x, location.y, 0.f),
                              Point3f(location.x, location.y, kCollision_object_height),
                              size.x / 2);
  }

  void Moving_spell_circle::update(float time)
  {
    Moving_spell::update(time);
    Moving_spell_circle::update_body();
    if (!Model_state::get_instance()->can_move(body))
      disable_spell();
  }

  void Moving_spell_rectangle::update_body()
  {
    Vector2f size = get_size();
    Point2f location = get_center_location() - size / 2;
    body = Collision::Parallelepiped(Point3f(location.x - size.x / 2, location.y - size.y / 2, 0.f),
                                     Vector3f(size.x, 0.f, 0.f),
                                     Vector3f(0.f, size.y, 0.f),
                                     Vector3f(0.f, 0.f, kCollision_object_height));
  }

  void Moving_spell_rectangle::update(float time)
  {
    Moving_spell::update(time);
    Moving_spell_rectangle::update_body();
    if (!Model_state::get_instance()->can_move(body))
      disable_spell();
  }

  Resizable_spell::Resizable_spell(const Point2f& location_,
                                   const Vector2f& size_,
                                   const Vector2f& resize_speed_,
                                   float life_time_) :
    Spell(life_time_),
    location(location_),
    size(size_),
    resize_speed(resize_speed_),
    body(Point3f(location_.x, location_.y, 0.f),
         Point3f(location_.x, location_.y, kCollision_object_height),
         size_.x / 2)
  {}

  void Resizable_spell::update_body()
  {
    body = Collision::Capsule(Point3f(location.x, location.y, 0.f),
                              Point3f(location.x, location.y, kCollision_object_height),
                              size.x / 2);
  }

  void Resizable_spell::update(float time)
  {
    Spell::update(time);
    if (!Spell::is_active())
      return;
    location -= resize_speed * time / 2;
    size += resize_speed * time;
    Resizable_spell::update_body();
  }

  void Resizable_spell::render(const String& texture, Color filter) const
  {
    float scale = Model_state::get_instance()->get_scale();
    Point2f relative_location = (get_center_location() - Model_state::get_instance()->get_center_location()) * scale + Point2f(400.0f, 300.0f);
    render_image(texture, // which texture to use
                 relative_location - scale * size / 2, // upper-left corner
                 relative_location + scale * size / 2, // lower-right corner
                 0.f, // rotation in radians
                 1.f, // scaling factor
                 relative_location, // point to rotate & scale about
                 false,
                 filter);
  }

}