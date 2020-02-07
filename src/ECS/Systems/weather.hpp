#include "ECS/system.hpp"

namespace ph {

struct Rain
{
	enum Type {
		Drizzle,
		Normal,
		Heavy
	};

	Type type = Type::Normal;
	bool needsInitialization = true;
};

// TODO: Add fog

namespace system {

class Weather : public System
{
public:
	Weather(entt::registry&);	

	void update(float dt) override;

	enum Mode {
		Sunny,
		Rainy 
	};

	static void setMode(Mode mode) { sMode = mode; } 
	static void setRainType(Rain::Type type);

private:
	inline static Mode sMode = Sunny;
	inline static Rain sRain;
};

}}
