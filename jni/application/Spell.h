#ifndef SPELL_H
#define SPELL_H

#include "Collision.h"
#include "Moving_object.h"
#include "Sim_object.h"

namespace Flame {

  class Spell : private Moving_object, public Sim_object {
  public:
    Spell(const Zeni::Point2f& location_ = Zeni::Point2f(),
          const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
          float speed_ = 0.f);

    virtual Zeni::Point2f get_location()
      {return get_current_location();}

    virtual void update(float time = 0.f);

    bool is_active() const
      {return active;}
    void inactivate_spell()
      {active = false;}

    Zeni::Vector2f get_size() const
      {return size;}

  protected:
    void render(const Zeni::String& texture, const Zeni::Color& filter = Zeni::Color()) const;

    void set_size(Zeni::Vector2f size_)
      {size = size_;}

  private:
    Zeni::Vector2f size;
    bool active;
  };

  class Light_ball : public Spell {
  public:
    Light_ball(const Zeni::Point2f& location_ = Zeni::Point2f(),
               const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
               const Zeni::Vector2f& size_ = Zeni::Vector2f(),
               float speed_ = 0.f,
               float life_time_ = 0.f);

    virtual void update(float time = 0.f);

    virtual void render();

  private:
    Collision_circle collision_body;
    float life_time;
  };

}

#endif