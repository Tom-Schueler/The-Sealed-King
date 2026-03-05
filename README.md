⚠️ Portfolio Notice

This repository exists only as a demonstration of my work. The code may be viewed and tested but may not be reused in other projects.

# The-Sealed-King

is the project I worked on during the third semester of my education at the _School for Games_ (S4G) in collaboration with ten of my fellow students -_you can find their names on the itch page_. We used Uneral Engine to realise this project.

The game is a 3D third person action game focused on melee combat, where you fight through a castle by using tactics like parry, finish, block, stun and drain their blood.

<img width="960" height="300" alt="image" src="https://github.com/user-attachments/assets/fa8e257b-e6e8-49d3-990e-224cf341eca3" />

##

In this project I mainly focused on Gameplay Programming and maintaing the Project's architecture -Specificly the behaviour of the main character. This task consisted of:

- Camera control (Free and Focused Camera)
- Interacting with interactables
- Integrating VFX
- Implementing Player controls (attacks, block, etc.)
- Chaining attacks smoothly

Especially much work went into the technical design of making the attacks feel as smooth and satisfying as possible. In the following, I want to go into greater detail on the:

## Combat Action System

My goal for this system was to make it as easy to use as possible for the designer team. In order to achieve this, I had to make sure all parts of the system are self-explanatory, documented and easily extensible. To ensure this, at its core, this whole System is based on a [_Trie_](https://en.wikipedia.org/wiki/Trie). Now I will explain why I think this works really great. 

### 1. Combat Action [Rooot](Source/Slasher/Public/CombatActionRoot.h) and [Node](Source/Slasher/Public/CombatActionNode.h)

<img align="right" width="477" height="510" alt="image" src="https://github.com/user-attachments/assets/f070c8eb-527d-4d40-a1dd-b78185a13e91" />

Both the Combat Action Root and Node are assets to be created by the Designers. With these, they are able to customize the length, order and more of a specified attack sequence/combo. Each node contains all information needed for the attack that is defined. 

This part ensures extensibility. With sufficient animations there is no limit on how many _combos_ can exist.

### 2. Montages

We use Anim Notifiers (States) to customize the animations for things such as FX of course, but more importantly, we can define for each animation when the:

- weapons Hitbox should actually be active
- transition to the next Animation should start (if any)
- Character can still turn

This ensures the Attack animations and transitions feel as smooth as possible.
<br clear="right"/>
### 3. Combat Action Executor and Node Navigator

Altough the nodes represent the trie, on their own they are pretty useless. This is where the [Node Navigator](https://blueprintue.com/blueprint/fx25ystk/) comes in. It represents the device with which the trie is traversed. 

Simply put, _The Combat Action Executor_ gets a root Assigned. It additionaly handles the playing of the [Montages](http://blueprintue.com/blueprint/wbj_7cj1/), Cooldown, etc., and [uses the _Node Navigator_ like a Cursor](https://blueprintue.com/blueprint/tq0lzvpy/).


