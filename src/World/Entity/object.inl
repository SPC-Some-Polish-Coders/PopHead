template <typename T>
void PopHead::World::Entity::Object::forEachChildWhichIsObject(std::function<void(T)> func, T param)
{
	for (auto& child : mChildren) {
		Object* objectPtr = dynamic_cast<Object*>(child.get());
		if (objectPtr)
			func(param);
	}
}