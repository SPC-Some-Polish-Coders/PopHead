template <typename T>
void ph::DrawableGameObject::forEachChildWhichIsDrawableGameObject(std::function<void(DrawableGameObject*, T)> func, T param)
{
	for (auto& child : mChildren) {
		auto drawableGameObjectPtr = dynamic_cast<DrawableGameObject*>(child.get());
		if (drawableGameObjectPtr)
			func(drawableGameObjectPtr, param);
	}
}
