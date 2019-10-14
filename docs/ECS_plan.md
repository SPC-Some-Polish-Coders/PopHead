# Overview
This document will contain all plans, schemas, assets, etc. that will be used to change our current game objects architecture (inheritance based) to ECS (Entity-Component-System). We decided to use existing ECS library - Entt - to not deal with problems of implementing ECS.

The documentation for systems that have been already implemented is [here](forProgrammers/ECS_architecture.md). 

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
  - [Main tutorial](https://skypjack.github.io/entt/autotoc_md8.html)

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
// struct Transformation {
//     sf::Transform transform; // model transformation matrix
//     sf::Vector2f position;
//     sf::Vector2f scale;
//     float rotation;
// };
struct Position {
    sf::Vector2f position;
};
struct VertexArray {
    ph::VertexArray vao; // binds VertexBuffer with its layout and with IndexBuffer
};
struct Shader {
    ph::Shader shader;
    // later here might be Material instead
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
struct Spawner { // TODO: we need to add type of object to spawn, later it'll be done in scripting language
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
struct Medkit {
    int healtPoints;
};
struct Bullets {
    unsigned short number;
};
struct ActivationState {
    bool turnedOn;
};
```

### Predefined entities
Our current game objects classes must be represented as lists of components.
These lists will be predefined in code for facilitation.
They can contain only obligatory components for object to work.
Optional components should be distinguished somehow.

- Player (VertexArray, Shader, KinematicCollisionBody, GunAttacker, MeleeAttacker, Health, Animation, Player, Velocity, ...)
- Zombie (VertexArray, Shader, KinematicCollisionBody, Health, Animation, PlayerFollower, Velocity, ...)
- Spawner (Spawner, Player, ...)
- Dead character (VertexArray, Shader, Lifetime, ...)
- Npc (VertexArray, Shader, ...)
- Entrance
- SlowDownArea?
- ActivateArea?
- CutSceneArea?
- Gate (VertexArray, Shader, ActivationState, StaticCollisionBody?)
- Lever (VertexArray, Shader, ...)
- Car (VertexArray, Shader, StaticCollisionBody, ...)
- CrawlingNpc (VertexArray, Shader, ...)
- GateGuard (VertexArray, Shader, ...)
- Bullet item (VertexArray, Shader, Bullets)
- Medkit (VertexArray, Shader, Medkit)
- Particles (VertexArray, Shader, ...)

### Systems Proposal
System's description MUST contain list of used components.<br>
Later the systems will be ordered.
- Spawners (Player, KinematicCollisionBody, Spawner)
- A-star (PlayerFollower, Velocity)
- Animation (Animation)
- Renderer (Transform?, VertexArray, Shader, Animation)
- Kinematic collisions (KinematicCollisionBody, Velocity)
- Static collisions (KinematicCollisionBody, StaticCollisionBody, Velocity)
- Medkits collecting (Medkit, Player, KinematicCollisionBody, ...)
- Bullets collecting (Bullets, Player, KinematicCollisionBody, ...)
- Moving objects (Velocity, Position)
