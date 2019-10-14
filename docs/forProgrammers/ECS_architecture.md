# Document overview
This document has been created to contain information about ECS model in our project.
The main theme is creating and managing systems.

#### Table of contents
- [Game objects](#game-objects)
- [Source code](#source-code)
  - [Entity](#entity)
  - [Components](#components)
  - [Systems](#systems)
- [Useful links](#useful-links)

## Game objects
Currently as a game objects architecture we use inheritance model, but we are transforming into ECS (Entity-Component-System). We use Entt library.
Tutorials and documentation to both ECS and Entt will be linked at the end of this document.

## Source code
#### Entity
In most implementations entities are lightweight classes with just bitset or identifier. Since we are using Entt library we use its entity, and we don't need to create our own.
#### Components
Components are just POD structs (just .hpp file). We need a lot of them, so we decided to group them (but not limit) in thematic files (e.g. physicsComponents.hpp contain Position and Velocity components).
These files must be in src/ECS/Components directory and **components must be in ph::component namespace!**
#### Systems

#### Useful links:
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
