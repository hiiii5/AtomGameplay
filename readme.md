# Atom
One of the most annoying things is rewriting basic gameplay systems which are needed for most games. The core principles of the Atom project is reusability, modularity, and ease of access.

Atom is an Unreal Engine 5.x plugin consisting of a set of reusable components that make basic gameplay elements easy to enable. Systems that are not needed can be turned off and on at will, and any actor in the world can utilize these components, even at runtime. The system is meant to be editor friendly so that designers can extend the system with user interface and other gameplay systems.

## Limitations
Atom is not designed for multiplayer at this time. It is a good fit for single player experiences that require any of listed features. Details of the features can be found in their individual sections along with code and editor examples.

## Features 
- Health System
- Status system
- Defense System
- Damage System

## Health System
The health system is a set of components that are interoperable to each other. This makes it easy to add a health component to an actor and use a heal over time component on its own, or based on the status system.

## Status System
Status effects are gameplay effects that require a threshold being met before being applied. This can be a gameplay effect like a sword doing bleed damage by having overcoming a set threshold, the default is from 0%-100%.

## Defense System
Armor and status effects are dictated by the defense system. Upon receiving damage the health component will determine the true amount of damage applied by the following formula as default:
`True Damage = Base Damage + (Armor Damage - Armor) + ∑(Status Effect Damage - Status Effect Defense)`

Status effect defenses are a map of strings with float values. So if there is a matching status effect, say for example bleed, the effect would be:
`Bleed Damage - Bleed Defense`. These are number based and not percentage based, so that a weapon can apply an amount of bleed damage.

The defense calculation can be overloaded in blueprint or c++ by overloading the `CalculateDamage` function found inside the AtomHealthComponent.

## Damage System
To build on top of the Unreal damage system, Atom provides an ease of access system to directly target any actor. The Unreal damage system can be overloaded and routed to the health system, allowing for existing systems to work on top of Atom.



