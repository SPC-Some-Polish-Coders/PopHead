#ifndef POPHEAD_RENDERER_LAYER_H_
#define POPHEAD_RENDERER_LAYER_H_

#include <list>
#include <string>

namespace ph {

class Object;

class Layer 
{
public:
	void addObject(Object* const object);
	void removeObject(const Object* const object);
	void removeObject(std::string name);
	inline void Layer::clear() { mObjects.clear(); }

	using ObjectsSequence = std::list<Object*>;
	inline auto Layer::begin() const -> const ObjectsSequence::const_iterator { return mObjects.cbegin(); }
	inline auto Layer::end() const -> const ObjectsSequence::const_iterator { return mObjects.cend(); }

private:
	ObjectsSequence mObjects;
};

}

#endif // !POPHEAD_RENDERER_LAYER_H_
