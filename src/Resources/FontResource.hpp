#ifndef POPHEAD_RESOURCE_FONTRESOURCE_H_
#define POPHEAD_RESOURCE_FONTRESOURCE_H_

#include <string>

#include <SFML/Graphics.hpp>

#include "Resources/resource.hpp"

namespace PopHead {
namespace Resources {

class FontResource : public Resource< FontResource >
{
  private:
    virtual bool load( std::string ) override;
    sf::Font mFont;
};

}}

#endif // !POPHEAD_RESOURCE_FONTRESOURCE_H_
