#ifndef SPELL_H
#define SPELL_H

#include "Collision.h"
#include "Moving_object.h"
#include "Model_state.h"
#include "Sim_object.h"

namespace Flame {

  // forward declaration
  //class Agent;

  class Spell : public Sim_object {
  public:
    // location_ is the upper left corner of the graph to be rendered
    Spell(float life_time_ = 0.f)
      : life_time(life_time_),
        active(true)
    {}

    // Accessors
    bool is_active() const
      {return active;}
    void disable_spell()
      {active = false;}
    virtual Zeni::Point2f get_location() const = 0;

    virtual void update_body() = 0;
    // first check if the spell should be inactivate,
    // then call Moving_object::update_location(time)
    virtual void update(float time = 0.f);
    virtual void render() = 0;

  private:
    float life_time;
    bool active;
  };

  // all spells that can move
  class Moving_spell : public Spell, private Moving_object {
  public:
    // location is the upper left corner!!!!!
    Moving_spell(const Zeni::Point2f& location_ = Zeni::Point2f(),
                 const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                 const Zeni::Vector2f& size_ = Zeni::Vector2f(),
                 float speed_ = 0.f,
                 float life_time_ = 0.f);

    virtual Zeni::Point2f get_location() const
      {return get_current_location();}
    Zeni::Vector2f get_orientation() const
      {return get_current_orientation();}
    Zeni::Vector2f get_size() const
      {return size;}

    virtual void update_body() = 0;
    virtual void update(float time = 0.f);
    virtual void render() = 0;
  protected:
    void render(const Zeni::String& texture) const;

    void set_orientation(const Zeni::Vector2f& orientation_)
      {set_orientation(orientation_);}
    void set_size(const Zeni::Vector2f& size_)
      {size = size_;}

  private:
    Zeni::Point2f relative_location;
    Zeni::Vector2f size;
    float scale;
  };

  // all moving spells that has a circular collision body
  class Moving_spell_circle : public Moving_spell {
  public:
    // location is the upper left corner!!!!!
    Moving_spell_circle(const Zeni::Point2f& location_ = Zeni::Point2f(),
                        const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                        const Zeni::Vector2f& size_ = Zeni::Vector2f(),
                        float speed_ = 0.f,
                        float life_time_ = 0.f) :
      Moving_spell(location_, orientation_, size_, speed_, life_time_),
      body(Zeni::Point3f(location_.x + size_.x / 2, location_.y + size_.y / 2, 0.f),
           Zeni::Point3f(location_.x + size_.x / 2, location_.y + size_.y / 2, kCollision_object_height),
           size_.x)
    {}

    virtual void update_body();
    virtual void update(float time = 0.f);
    virtual void render() = 0;

    Zeni::Collision::Capsule get_body() const
    {return body;}

  private:
    Zeni::Collision::Capsule body;
  };

  // all moving spells that has a rectangle collision body
  class Moving_spell_rectangle : public Moving_spell {
  public:
    // location is the upper left corner!!!!!
    Moving_spell_rectangle(const Zeni::Point2f& location_ = Zeni::Point2f(),
                           const Zeni::Vector2f& orientation_ = Zeni::Vector2f(),
                           const Zeni::Vector2f& size_ = Zeni::Vector2f(),
                           float speed_ = 0.f,
                           float life_time_ = 0.f) :
      Moving_spell(location_, orientation_, size_, speed_, life_time_),
      body(Zeni::Point3f(location_.x, location_.y, 0.f),
           Zeni::Vector3f(size_.x, 0.f, 0.f),
           Zeni::Vector3f(0.f, size_.y, 0.f),
           Zeni::Vector3f(0.f, 0.f, kCollision_object_height))
    {}

    virtual void update_body();
    virtual void update(float time = 0.f);
    virtual void render() = 0;
    
    Zeni::Collision::Parallelepiped get_body() const
    {return body;}

  private:
    Zeni::Collision::Parallelepiped body;
  };

  // all spell that is resizable
  class Resizable_spell : public Spell {
  public:
    // location is the upper left corner!!!!!
    Resizable_spell(const Zeni::Point2f& location_ = Zeni::Point2f(),
                    const Zeni::Vector2f& size_ = Zeni::Vector2f(),
                    const Zeni::Vector2f& resize_speed_ = Zeni::Vector2f(),
                    float life_time_ = 0.f);

    virtual Zeni::Point2f get_location() const
      {return location;}

    virtual void update_body();
    virtual void update(float time = 0.f);
    virtual void render() = 0;

  protected:
    void render(const Zeni::String& texture) const;

  private:
    Zeni::Point2f location;
    Zeni::Vector2f size;
    Zeni::Vector2f resize_speed;
    Zeni::Collision::Capsule body;
  };

}

#endif