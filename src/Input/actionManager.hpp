#ifndef POPHEAD_INPUT_ACTIONMANAGER_H_
#define POPHEAD_INPUT_ACTIONMANAGER_H_

#include <SFML/Window.hpp>
#include <string>
#include <vector>
#include <map>

namespace PopHead {
namespace Input {

class ActionManager
{
public:
    ActionManager();

    void addAction( const std::string& action, std::vector<sf::Keyboard::Key> );
    void addAction( const std::string& action, sf::Keyboard::Key );
    void addKeyToAction( const std::string& action, sf::Keyboard::Key );
    void deleteKeyFromAction( const std::string& action, sf::Keyboard::Key );
    void deleteAction(const std::string& action);
    void clearAllActions() noexcept;

    bool isActionPressed( const std::string& action );
    bool isActionJustPressed( const std::string& action );
    bool isActionJustReleased( const std::string& action );

private:
    std::map< std::string, std::vector<sf::Keyboard::Key> > mActions;
};

}}

#endif // !POPHEAD_INPUT_ACTIONMANAGER_H_
