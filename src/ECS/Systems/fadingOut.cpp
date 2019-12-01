#include "fadingOut.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

void FadingOut::update(float seconds)
{
	auto view = mRegistry.view<component::FadingOut, component::Color>();
	for(auto entity : view)
	{
		float& timeFromDeath = view.get<component::FadingOut>(entity).timeFromDeath;
		if(timeFromDeath > 10.f)
			mRegistry.assign<component::TaggedToDestroy>(entity);

		timeFromDeath += seconds;
		sf::Color& color = view.get<component::Color>(entity).color;
		color.a = static_cast<unsigned char>(255.f - (timeFromDeath * 25.5f));
	}
}

}
