#ifndef POPHEAD_INPUT_ACTIONMANAGER_H_
#define POPHEAD_INPUT_ACTIONMANAGER_H_

#include <SFML/Window.hpp>
#include <string>
#include <vector>

namespace PopHead {
namespace Input {

class ActionManager
{
public:
    ActionManager();

    void addAction ( const std::string& action, std::vector<sf::Event> );
    void addAction ( const std::string& action, sf::Event );
    void addEventWhichDoAction ( const std::string& toWhichAction, sf::Event );

    bool isActionPressed ( std::string action ) const;
    bool isActionJustPressed ( std::string action ) const;

private:
    std::map< std::string action, std::vector<sf::Event> >;

};

}}

#endif // !POPHEAD_INPUT_ACTIONMANAGER_H_
