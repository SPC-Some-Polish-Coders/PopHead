#pragma once

namespace ph{

class Bullet
{
public:
	Bullet(const float damage, const float range);
   
private:
	void dealDamage();

private:
    const float mDamage;
    const float mRange;
};

}
