#ifndef POPHEAD_RENDERER_LAYER_H_
#define POPHEAD_RENDERER_LAYER_H_

#include <list>
#include <string>

namespace PopHead {

namespace World { namespace Entity { class Object; } }

namespace Renderer {

class Layer
{
  public:
    using ObjectsSequence = std::list< World::Entity::Object* >;
    void addObject( World::Entity::Object* const object );

    void removeObject( const World::Entity::Object* const object );
    void removeObject( std::string name );

    inline auto begin() const -> const ObjectsSequence::const_iterator;
    inline auto end() const -> const ObjectsSequence::const_iterator;

    inline void clear();

  private:
    ObjectsSequence mObjects;
};

inline auto Layer::begin() const -> const ObjectsSequence::const_iterator
{ return mObjects.cbegin(); }

inline auto Layer::end() const -> const ObjectsSequence::const_iterator
{ return mObjects.cend(); }

inline void Layer::clear()
{ mObjects.clear(); }

}}

#endif // !POPHEAD_RENDERER_LAYER_H_
