#include "ECS/system.hpp"

namespace ph::system {

class WeatherSystem : public System
{
public:
	WeatherSystem(entt::registry&);	

	void update(float dt) override;
};

}
