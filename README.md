# N-Genius
Development of a 2D game Engine

The goal of this project was to build a tiny 2D game enjoying applying known Game Programming patterns such as entity-component, command, service locator or observer pattern.
In turns, I needed to program the old school QBert game using my engine.

# About QBert:
- 3 Level stage: 1 color swap, 2 color swap, 1 color swap with reversing.
- 3 Game mode: Single (classic), Co-op where 2 players play hand in hand to complete the levels and Versus, where 1 player controls QBert and the second controls the spawning Coilys.

Controls:

    Player 1:
        Keyboard: 
            W: North-West movement
            A: Sout-West Movement
            S: Sout-East Movement
            D: North-East Movement
    
    Player 1:
        Keyboard: 
            W: North-West movement
            A: Sout-West Movement
            S: Sout-East Movement
            D: North-East Movement

    Player 2:
        Keyboard: 
            Arrow UP: North-West movement
            Arrow LEFT: Sout-West Movement
            Arrow DOWN: Sout-East Movement
            Arrow RIGHT: North-East Movement
            

# About NGenius Engine:

# Component based:

The game entities within the engine are built around a GameObject class, not inheritable, that serves as a container class to tie up all components that will constitute the game entity.

![image](https://user-images.githubusercontent.com/76394390/120938923-969c5300-c715-11eb-9d1c-cd0b193a3b8a.png)

Its main functionnality is to define the bare bone of the objects, such as transform, name, tag, and provide a handle to the scene for queries (get GameObjects).
Then most attribute and behaviors given to the game entity is built around polymorphic "Components", (user) defined derived class that provides basic render and update capabilities.

![image](https://user-images.githubusercontent.com/76394390/120939071-5689a000-c716-11eb-8227-11dec707360a.png)

This programming design choice allows to build reusable, simple responsibility classes. It allows to keep each component short, focused annd prevent spaghetti code to try and handle all the different combination of behaviors needed into one monolithic class. Also, GameObjects being only a stack of Components working side by side, they can share similar behavior without getting them all which increase code flexibility.

i.e QBert has a HealthComponent (hold it's hp count and aplly damages) and a ScoreComponent (hold it's current score and handle the game events changing it), on the other side Coilys, eggs, Slick and Sam, etc.. also have their own health and damage handling but no score. The 2 being split apart (espceially as they don't relate to each other). Allow to have 1 component satisfaying both QBert and enemies needs without change.

A GameObject will accept anything inheriting the IComponent interface, we can go then one step further and add components inheriting from other components providing good encapsulation for behaviors.


# Event based communication (Observer pattern):

On important part of the engine I created is the way components/gameobjects talk to each other. Component that work tightly together can keep a reference to each other and try to find each other through their parent GameObject but it add strong coupling to their implementation and it gets trickier (or more expansive) when they have to find a component on another GameObject, somewhere else, at runtime.
I decide to create generic **Event<>** that anyone or anything can (un)register to. When invoked, it calls all the registered function. I implemented it that way with **C# delegate** or **UnityEvent<>** as I find them light and handy to send out a batch of callbacks.

![image](https://user-images.githubusercontent.com/76394390/120939956-e6c9e400-c71a-11eb-9c9e-57457d35663e.png)

I prefered this implementation as having some kind of Subject-Observer inteface that anything needing it would have to inherit as it would add another layer of virtual classes on the lot and would enforce the observer to boil down to one "OnNotify" method trying to handle all sort of different messages. **Prefer composition over inheritance**
With a generic Event, the observers can easily be anything and one structure can have as many Events as needed, and each events can have different parameter structure sent to their listeners. 

i.e A health component can have Events fired when they lose some life and when their life comes to 0:

![image](https://user-images.githubusercontent.com/76394390/120940250-9b183a00-c71c-11eb-9e2f-7abdee7aa065.png)

And then QBert could decide he is going to swear the damage away:

![image](https://user-images.githubusercontent.com/76394390/120940308-e6324d00-c71c-11eb-8db5-4e34c2b722ea.png)

Or Ugg/WrongWay will tell their spawner to call out someone else before destroying themselves on death:

![image](https://user-images.githubusercontent.com/76394390/120940365-3a3d3180-c71d-11eb-9669-c4a816e0d031.png)

Same component be the Event allowed for a wider flexibility and customization.

Still can come at a cost as non-unregistered listeners to those events may create memory leaks or crash if bound to destroyed objects. It's something that I found fairly powerful but still need to be careful with.


# Resource loading as a Service:

To be fair the idea came to me from using the Game Engine from my Graphics course but I liked the idea and find it really practical.
The system comes down to having one generic Resource manager class that will store, give access and load all resources needed. To do so it will register **loaders** which will each handle their own resource type and automatically use the correct one when the user request a resource and cache the data for later use.

As many loaders as we want can be attached to the manager as long as they implement the IBaseLoader interface:

![image](https://user-images.githubusercontent.com/76394390/120940658-a9ffec00-c71e-11eb-99c7-90d1dfaeb9c2.png)

I like this design a lot because it really centralised the access to resources and avoid having to remember which manager, class gives me access to which resource and pile up the includes; just need to know it's there somewhere and ask the ResourceManager for it.

![image](https://user-images.githubusercontent.com/76394390/120940828-71acdd80-c71f-11eb-82b3-9b0493daeb25.png)
![image](https://user-images.githubusercontent.com/76394390/120940865-b0429800-c71f-11eb-930d-4111ef06d770.png)

And if needed, a new custom loader can easily be added on top of the existing ones:

![image](https://user-images.githubusercontent.com/76394390/120940878-cb150c80-c71f-11eb-9759-079a112704e2.png)
![image](https://user-images.githubusercontent.com/76394390/120940891-d9632880-c71f-11eb-9c69-6d2fa3adb65b.png)


# Scene based

The game logic is built around a single SceneManager that holds all the existing Scenes objects and allow to swap between them. A scene has for single purpose to manage all the game objects that need to "work together" but game objects cannot communicate between scenes. The scene allows 3 Events to handle GameObject manmagement when it is first initialized or when it is (de)activated (scene change).


# Physics system (more like the idea of one)

I simply created the start of the bare bone of what could eventually be (yeah that little) the be physics system and so far have mostly enough to cover my QBert overlaps. But the idea comes to have a scene specific Physics handling system, whenever game objects are created or destroyed their rigid bodies are (un)registered to the physics handler which will process their collision in it's game update phase.
Each rigidbodies can register their overlap events to handle their own responses (if any) to the current overlap, giving them access to the objects their are overlapping with. It's really eary work but I have some rough idea how I could add other collision detection and/or collision resolution logic around the current design.

![image](https://user-images.githubusercontent.com/76394390/120941295-1b8d6980-c722-11eb-8391-790f832e7215.png)


# The GameMode supervisor

Another idea that came to me from Unreal (well what I assumed was the concept there), is to allow for a custom made (optional) GameMode implementation to allow the user to supervise theior own overall game logic, states, etc...
As the scenes are isolated from each other, it is difficult to send data around, that's where the GameMode comes in as it lives as long as the game does. The engine provides a simple GameMode interface that can be inherited to implement one specifically for the current game built:

![image](https://user-images.githubusercontent.com/76394390/120941411-f2210d80-c722-11eb-8008-a5723503ec49.png)
![image](https://user-images.githubusercontent.com/76394390/120941402-e5041e80-c722-11eb-9152-9129761612b3.png)




# What's next

A lot of work is still needed to make this engine resourceful and practical. 
- I would like to dig deeper into getting the physics handler up and running with different type of overlap event but also hard collisions.
- I should probably look more into the **Event<>** system I am working with as I might have ended up over using it to make QBert and had some problem with smart pointers and cicular references, unregistering the event is still a bit painstacking.
- Finish handling spritesheets (nice way to handle the tiles) and sprite animations.
- Finish logging system to include file write and custom asserts (asserts in release if needed !!!)
- And play around with it more to see what works and doesn't, those would probably be a good start to make it more interesting.
