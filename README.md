# Burger Time | Kobengine

This is a custom-built, lightweight 2D game engine developed during Programming 4 at Howest - Digital Arts & Entertainment. It was used to implement a version of the classic arcade game *Burger Time*.

Made by Kobe Dereyne.

## Source Control

Game: [Burger Time Repository](https://github.com/Kobazaaa/BurgerTime)
<br>
Engine: [Kobengine Repository](https://github.com/Kobazaaa/Kobengine)

## Design Patterns & Choices

### Update Loop

The game loop separates update phases for a clear logic flow. It includes Input Processing, Updates, Rendering, and Collision.
![Update Loop](assets/UpdateLoop.png)

### Component
The engine uses a component-based architecture. Game objects are composed of reusable components, some provided by the engine, others made by the user.
Some components that are a part of the engine are:
- Image Renderer
- Text Renderer
- Animator
- Collider
- ...

### Events
For communication, the user has the option to use events that are built into the engine. Kobengine tries to mimic Unity events in C++, binding (member) functions and/or lambdas to events, which get called once the event gets invoked.
Kobengine already supports events to have lambdas and event listeners as its listeners, but the user can always make a new class that inherits from EventListener to have extra functionality if they need.

### Commands
Similar to events, Kobengine already supports a Command interface to easily hook up input to actions.

### Service Locator & Singletons
Both the Sound System and Collision System are part of the Service Locator Pattern and can be swapped out by a personal implementation if wanted. For some other classes such as the Timer class, this is not possible.

