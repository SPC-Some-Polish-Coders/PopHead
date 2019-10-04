# Overview
This document will contain all plans, schemas, assets, etc. that will be used to change our current game objects architecture (inheritance based) to ECS (Entity-Component-System). We decided to use existing ECS library - Entt - to not deal with problems of implementing ECS.

#### Useful Docs:
- ECS introduction
  - [wikipedia](https://en.wikipedia.org/wiki/Entity_component_system)
  - [https://www.richardlord.net/blog/ecs/what-is-an-entity-framework.html](https://www.richardlord.net/blog/ecs/what-is-an-entity-framework.html)
  - [https://www.gamedev.net/articles/programming/general-and-gameplay-programming/understanding-component-entity-systems-r3013/](https://www.gamedev.net/articles/programming/general-and-gameplay-programming/understanding-component-entity-systems-r3013/)
  - [https://medium.com/ingeniouslysimple/entities-components-and-systems-89c31464240d](https://medium.com/ingeniouslysimple/entities-components-and-systems-89c31464240d)
  - [https://www.gamedev.net/articles/programming/general-and-gameplay-programming/the-entity-component-system-c-game-design-pattern-part-1-r4803/](https://www.gamedev.net/articles/programming/general-and-gameplay-programming/the-entity-component-system-c-game-design-pattern-part-1-r4803/)
- Entt library
  - [GitHub repository](https://github.com/skypjack/entt)
  - [Official documentation](https://skypjack.github.io/entt/)

## Components and Systems
In order to create a new game objects architecture we need to make a list of components and systems.
 Since we are not adding new types of objects we just need to divide existing classes into variables (components) and logic (systems).

### Components Proposal
If a component is not clear it's useful to add comment with use cases.
```cpp
std::string name; // for accessing certain object and debugging

int healtPoints;

ph::Rect hitbox; // Can be used for physics calculations as well as for fight system

struct Transformation {
    sf::Transform transform; // model transformation matrix
    sf::Vector2f position;
    sf::Vector2f scale;
    float rotation;
};

ph::VertexArray vao; // binds VertexBuffer with its layout and with IndexBuffer

ph::Shader shader;
// later here might be Material instead

struct Animation {
    // TODO: add actual variables here
};

sf::Vector2f velocity;

struct PlayerFollower { // is controlled by A* to find a player, later probably will be done in scripting language
    ph::Path path;
    // possibly more variables
};

struct Spawner { // TODO: we need to add type of object to spawn, later it'll be done in scripting language
    float distanceToPlayerToWork;
    sf::Vector2f areaToSpawn; // different type?
    sf::Time interval; // range for randomness?
    // std::pair<size_t, size_t> numberOfObjectsToSpawn; // do we need custom amount?
};
```

### Systems Proposal
System's description MUST contain list of used components.
- Animation (Animation)
- Renderer (Transorm, VertexArray, Shader, Animation)
- PhysicsEngine (Transform, Hitbox, velocity)
- AIManager (Transform, PlayerFollower) 
- And some game mechanics systems for example shooting. But they ofcourse will be implemented in scripting language later on 
