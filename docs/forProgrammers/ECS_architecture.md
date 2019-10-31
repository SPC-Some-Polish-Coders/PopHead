# Document overview
This document has been created to contain information about ECS model in our project.
The main theme is creating and managing systems.

#### Table of contents
- [Game objects](#game-objects)
- [Source code](#source-code)
  - [Entity](#entity)
  - [Components](#components)
  - [Systems](#systems)
  - [Systems queue](#systems-queue)
- [Systems documentation](#systems-documentation)
- [Useful links](#useful-links)

## Game objects
Currently as a game objects architecture we use inheritance model, but we are transforming into ECS (Entity-Component-System). We use Entt library.
Tutorials and documentation to both ECS and Entt will be linked at the end of this document.

## Source code
#### Entity
In most implementations entities are lightweight classes with just bitset or identifier. Since we are using Entt library we use its entity, and we don't need to create our own.
#### Components
Components are just POD structs (just .hpp file). We need a lot of them, so we decided to group them (but not limit) in thematic files (e.g. physicsComponents.hpp contain Position and Velocity components).
These files must be in src/ECS/Components directory and **components must be declared in ph::component namespace!**

The components probably will not be listed here.
#### Systems
Entt doesn't require systems to be classes (they can be free functions), but we want to have flexibility of OOP.
We have base 'System' class which all systems must inherit from.
For now we have three main requirements for systems:
- systems must override 'update(float second)' function
- all constructors must take as a first argument entt::registry& (it is needed in systems queue)
- they must meet the requirements of std::vector

**Systems must be declared in ph::system namespace!**

##### Note: if the only constructor that you need takes only entt::registry& argument you can simply use constructor of 'System' class (using System::System;)

#### Systems queue
SystemsQueue class is an entry point for our systems. It is responsible for creating, managing and running systems.
For now it only supports basic features:
- update(float seconds) method runs update method on all systems that were added to queue. Systems are run only in main thread, but in near future there will be definitely created mechanism for running systems in parallel.
- 'appendSystem' is a template method that creates given type of system with given arguments and adds it at the end of the queue. Example:
  ```cpp
  entt::registry reg;
  ph::SystemsQueue queue(reg);
  queue.appendSystem<system::Movement>();
  ```
  You can give the rest of arguments for constructor of system, but remember to omit first argument, which is given by SystemsQueue! In example above constructor is defined as: ```Movement(entt::registry&)```

## Systems documentation

This is the most important part of the document.
It holds an information about which components system is working on.
It also tells which components are only read and which are modified.
This table is very important for multithreading of systems, so it **must be always up to date**.

<table>
    <tr>
        <th>System name (class)</th>
        <th>Component name</th>
        <th>R/W (Read/Write)</th>
    </tr>
    <tr>
        <td rowspan=3>PlayerInput</td>
        <td>Velocity</td>
        <td>W</td>
    </tr>
    <tr>
        <td>CharacterSpeed</td>
        <td>R</td>
    </tr>
    <tr>
        <td>Player</td>
        <td>R</td>
    </tr>
    <tr>
        <td rowspan=2>Movement</td>
        <td>Velocity</td>
        <td>R</td>
    </tr>
    <tr>
        <td>Position</td>
        <td>W</td>
    </tr>
    <tr>
        <td rowspan=2>SpritesSync</td>
        <td>Position</td>
        <td>R</td>
    </tr>
    <tr>
        <td>Sprite</td>
        <td>W</td>
    </tr>
    <tr>
        <td>Renderer</td>
        <td>Sprite</td>
        <td>R</td>
    </tr>
    <tr>
      <td rowspan=2>DyingCharacters</td>
      <td>Health</td>
      <td>R</td>
    </tr>
    <tr>
      <td>TaggedToDestroy</td>
      <td>W</td>
    <tr>
      <td>EntityDestroying</td>
      <td>TaggedToDestroy</td>
      <td>R</td>
    </tr>
  <tr>
      <td rowspan=8>PickupSystem</td>
      <td>Health</td>
      <td>W</td>
    </tr>
      <td>TaggedToDestroy</td>
      <td>W</td>
      </tr>
      <td>GunAttacker</td>
      <td>W</td>
    </tr>
    <td>Position</td>
  <td>R</td>
      </tr>
    <td>Size</td>
    <td>R</td>    
  </tr>
      <td>Medkit</td>
      <td>R</td>
  </tr>
      <td>Bullet</td>
      <td>R</td>
      </tr>
      <td>Player</td>
      <td>R</td>
</table>
    
### Useful links:
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

#### Example of ECS in action

```cpp
ph::ActionEventManager::init();

sf::RenderWindow window(sf::VideoMode(500, 500), "Test");
sf::Texture texture;
texture.loadFromFile("resources/textures/vehicles/car.png");

using namespace ph;

entt::registry reg;

ph::SystemsQueue queue(reg);
queue.appendSystem<system::PlayerInput>();
queue.appendSystem<system::Movement>();
queue.appendSystem<system::SpritesSync>();
queue.appendSystem<system::Renderer>(std::ref(window));

component::Sprite spr;
spr.sprite.setTexture(texture);

auto entity = reg.create();
reg.assign<component::Position>(entity, 0.f, 0.f);
reg.assign<component::Velocity>(entity, 0.f, 0.f);
reg.assign<component::Sprite>(entity, std::move(spr));
reg.assign<component::CharacterSpeed>(entity, 55.f);
reg.assign<component::Player>(entity);

sf::Clock clock;
while (window.isOpen())
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    sf::Time delta = clock.restart();
    queue.update(delta.asSeconds());
}
```
