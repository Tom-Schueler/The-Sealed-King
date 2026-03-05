⚠️ Portfolio Notice

This repository exists only as a demonstration of my work. The code may be viewed and tested but may not be reused in other projects.

# The-Sealed-King

is the project I worked on during the third semester of my education at the _School for Games_ (S4G) in collaboration with 10 of my fellow students -_you can find their names on the itch page_. We used Uneral Engine to realise this project.

It is a 3D third person action game focused on melee combat, where you fight through a castle by using tactics like parry, finish, block, stun and drain their blood.

<img width="960" height="300" alt="image" src="https://github.com/user-attachments/assets/fa8e257b-e6e8-49d3-990e-224cf341eca3" />

##

In this project I concentrated mainly on Gameplay Programming and maintaing the Project's architecture. Specificly the behaviour of the main character. This task consisted of:

- Camera control (Free-, Focused-Camera)
- Interacting with Interactables
- Integrating VFX
- Implementing Player Controlles (Attacks, Block, etc.)
- Chaining Attacks smoothly

Especially much work went into the Technical Design of making the Attacks as smooth and satisfying as possible. In the following I want to go into greater detail of the:

## Combat Action System

My goal for this System was to make it as easy to use as possible for the Designerteam. In order to achieve this I had to make sure all parts of the System are self explanetory, Documented and easyly extensible. To ensure this, at the core, this whole System is based on a [_Trie_](https://en.wikipedia.org/wiki/Trie). Now I will explain why I think this works really great. 

### 1. Combat Action [Rooot](Source/Slasher/Public/CombatActionRoot.h) and [Node](Source/Slasher/Public/CombatActionNode.h)

<img align="right" width="477" height="510" alt="image" src="https://github.com/user-attachments/assets/f070c8eb-527d-4d40-a1dd-b78185a13e91" />

Both the Combat Action Root and Node are assets to be created by the Designers. With this they are abled to customise the length, order and so on, of a specified Attack-Sequence/Combo. Each node conains all information needed for the Attack that is Defined. 

This part ensures expandability. With sufficient Animations there is no limit on how many _Combos_ can exist.

### 2. Montages

We use Anim Notifier (States) to customise the animations for things such as FX of course, but more importantly we can define for each animation, when the:

- weapons Hitbox should actually be active
- transition to the next Animation should start (if any)
- Character can still turn

This ensures the Attack animations and transitions feel as smooth as possible.

<br clear="right"/>

### 3. Combat Action Executor and Node Navigator

Altough the Nodes represent the Trie, on their own they are pretty useless. This is where the Node Navigator comes in. It represents the Device with which the Trie is traversed. 

Simply said, _The Combat Action Executor_ gets a Root Assigned. It additionaly handles the playing of the [Montages](http://blueprintue.com/blueprint/wbj_7cj1/), Cooldown, etc., and [uses the _Node Navigator_ like a Cursor](https://blueprintue.com/blueprint/ax6pw9fo/).


