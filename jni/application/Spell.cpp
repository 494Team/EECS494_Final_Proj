#include <zenilib.h>

#include "Spell.h"

using namespace Zeni;

namespace Flame {

  Spell::Spell(const Point2f& location_, const Vector2f& orientation_, float speed_) :
    Moving_object(speed_, location_),
    size(Vector2f()),
    active(true)
    {Moving_object::set_orientation(orientation_);}

  void Spell::update(float time)
    {Moving_object::update_location(time);}

  void Spell::render(const String& texture, const Color& filter) const
  {
    render_image(texture, // which texture to use
                 Moving_object::get_current_location(), // upper-left corner
                 Moving_object::get_current_location() + size, // lower-right corner
                 0.f, // rotation in radians
                 1.f, // scaling factor
                 Moving_object::get_current_location() + 0.5f * size, // point to rotate & scale about
                 false, // whether or not to horizontally flip the texture
                 filter); // what Color to "paint" the texture
  }

  Moving_ball::Moving_ball(const Point2f& location_,
                           const Vector2f& orientation_,
                           const Vector2f& size_,
                           const float speed_,
                           const float life_time_) :
    Spell(location_, orientation_, speed_),
    life_time(life_time_)
    {}

  void Moving_ball::update(float time)
  {
    if (!Moving_ball::is_active())
      return;
    life_time -= time;
    if (life_time < 0.f) {
      Spell::inactivate_spell();
      return;
    }
    Spell::update(time);
  }

  void Moving_ball::render()
  {
    if (Spell::is_active())
      Spell::render("brick");
  }

}