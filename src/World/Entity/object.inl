template <typename T>
void ph::Object::forEachChildWhichIsObject(std::function<void(Object*, T)> func, T param)
{
	for (auto& child : mChildren) {
		Object* objectPtr = dynamic_cast<Object*>(child.get());
		if (objectPtr)
			func(objectPtr, param);
	}
}