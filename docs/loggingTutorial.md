# Logging Tutorial

### Log Levels
- Info
- Warning
- Error
- Critical

### Logging macros

Basic macros:
- PH_LOG_INFO(message);
- PH_LOG_WARNING(message);
- PH_LOG_ERROR(message);
- PH_LOG_CRITICAL(message);

They are used to report a log of corresponding level to global Log system. PH_LOG_INFO and PH_LOG_WARNING are turned off in Distribution version of application for optimazation purposes. PH_LOG_CRITICAL is the only one of them that throws an exception of type std::runtime_error. The more precise description of logging levels can be found below.

Assert macros:
- PH_ASSERT_WARNING(expression, message);
- PH_ASSERT_ERROR(expression, message);
- PH_ASSERT_CRITICAL(expression, message);

These are declared only for convenience. They check an expression and when it's expanding to false, then they report a log with corresponding level.

Additional macros:
- PH_UNEXPECTED_SITUATION(message);
- PH_ASSERT_UNEXPECTED_SITUATION(expression, message);

(not described for now)

## Precise description and use cases of logging levels
- Info - tells that an operation succeeded, or gives debug informations (e.g. statistics). Can only be used with positive messages (not errors or problems). It must contain enough informations to distinctly identify an operation, but not everything, like content of files.
	
    Bad use cases:
	```cpp
    Xml Xml::getChild(std::string name) const {
        // ...
        PH_LOG_INFO("Xml getChild(): " + xml.mContent);  
        // content of Xml is to large, we litter the logs
        // ...
    }
    
    bool Xml::hasAttribute(std::string name) const {
        // ...
        PH_LOG_INFO("Xml hasAttribute(): false");
        // absolutely useless, doesn't give any information, maybe write tag's and attibute's names?
        // ...
    }
    ```
	Good use cases:
	```cpp
    void Entity::addChild(std::unique_ptr<Entity> newChild) {
        // ...
        PH_LOG_INFO("Entity \"" + nameOfNewChild + "\" was added as child of the \"" + mName + "\"");
        // we can identify what was added, and where
    }

    void CollisionDebugSettings::turnOn() {
        mShouldDisplay = true;
        PH_LOG_INFO("Collision debug was turned on.");
        // in this context it is enough information
    }
    
    int main() {
        PH_LOG_INFO("start initializing PopHead");
        ph::Game game;
        PH_LOG_INFO("start executing PopHead");
        game.run();
        // that's ok, we seperate the sections of Logs for readability
    }
    ```

- Warning - tells about situation, that might be a problem, but is not harmful for now. These situations should be very easily handled, or even ignored.
	
    Bad use cases:
	```cpp
    ```
	Good use cases:
	```cpp
    void SceneManager::popAction() {
        if (mScene == nullptr)
            PH_LOG_WARNING("You are trying to pop scene but there is no scene to pop.");
        else {...}
        // ok, missing action doesn't cause any problems, or errors, but it's not good idea to call this function for no reason
    }

    std::vector<Xml> Map::getTilesetNodes(const Xml& mapNode) const {
        const std::vector<Xml> tilesetNodes = mapNode.getChildren("tileset");
        if (tilesetNodes.size() == 0)
            PH_LOG_WARNING("Map doesn't have any tilesets");
        return tilesetNodes;
        // this is fine, the signature of function indicates that there SHOULD be some tileset nodes, but that's not a problem, if there aren't any
        // NOTE: a caller of this function can still treat that (missing tilesets) as serious problem, and use Error, or even Critical, but in this exact context it would be too much of assumption
    }
    ```

- Error - the current operation cannot be completely executed. The problem should be handled in a way, that doesn't crash the application. Note that this level will not throw an exception. It depends only on you, if a problem is handled internally in current function, or somewhere above in calling function by catching an exception.
	
    Bad use cases:
	```cpp
    ```
	Good use cases:
	```cpp
    ```

- Critical - the most dangerous level of log. It happens in situations, when a problem cannot be handle in any way. These problems are so serious, that they force application to stop.
	
    Bad use cases:
	```cpp
    ```
	Good use cases:
	```cpp
    ```
