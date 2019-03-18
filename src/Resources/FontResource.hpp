#ifndef POPHEAD_RESOURCE_FONTRESOURCE_H_
#define POPHEAD_RESOURCE_FONTRESOURCE_H_

#include <SFML/Graphics.hpp>

#include "Resources/Resource.hpp"

namespace PopHead {
namespace Resources {

class FontResource : public Resource< FontResource >
{
  private:
    virtual bool load() override;
    sf::Font mFont;
};

}}

#endif // !POPHEAD_RESOURCE_FONTRESOURCE_H_
