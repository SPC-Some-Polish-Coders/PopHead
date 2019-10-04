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
struct Name { // for example for debugging and logging
    std::string name;
};
struct Health {
    int healtPoints;
};
struct HitBox {
    ph::Rect hitbox;
};
struct Sprite {
    sf::Sprite sprite;
};
struct Animation {
    // TODO: add actual variables here
};
struct Velocity {
    // !!! there are two possible versions (both with the same size)
    // 1. polar coordinates
    float speed;
    float angle; // [0, 360)
    // 2. cartesian coordinates
    sf::Vector2f velocity;
};
struct StaticCollisionBody {
    ph::Rect body;
};
struct KinematicCollisionBody { // possibly we could create different component for hitbox
    ph::Rect body;
    float mass;
};
struct Player {
    // do we need an id for multiple players?
};
struct PlayerFollower { // is controlled by A* to find a player
    ph::Path path;
    // possibly more variables
};
struct Spawner { // TODO: we need to add type of object to spawn
    float distanceToPlayerToWork;
    sf::Vector2f areaToSpawn; // different type?
    sf::Time interval; // range for randomness?
    // std::pair<size_t, size_t> numberOfObjectsToSpawn; // do we need custom amount?
};
struct MeleeAttacker {
    // TODO: add variables
    float minSecondsInterval;
};
struct GunAttacker {
    // TODO: add variables
    float minSecondsInterval;
    unsigned short bullets;
};
struct Lifetime {
    float secondsToDie;
};
```

### Systems Proposal
System's description MUST contain list of used components.
- Spawners (Player, Sprite, Spawner)
- A-star (PlayerFollower, Velocity)
- Animations (Animation, Sprite)
- Rendering (Sprite)
