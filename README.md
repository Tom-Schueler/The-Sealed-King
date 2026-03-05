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

My goal for this System was to make as easy to use as possible for the Designteam. In order to achieve this I had to make sure all parts of the System are self explanetory, Documented and easyly extensible. At the core, this whole System is based on a [_Trie_](https://en.wikipedia.org/wiki/Trie). 

### 1. Combat Action- [Rooot](Source/Slasher/Public/CombatActionRoot.h) | [Node](Source/Slasher/Public/CombatActionNode.h)


