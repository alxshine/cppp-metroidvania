# Entity definitions
They each have their own directory and file format, as well as file extension.

The following should be contained in **EVERY** entity definition:
- ID

Due to our structure, every entity type will require their own parser, but that is still better than using a single format for all types.
All formats will probably be something akin to property files or something similar.

## Animations
Animations in general follow the same structure:
- the name of the spritesheet to use for the animation
- list of rectangles for individual frames
- time (in ms) per frame

## Sound Effects
Sound effects consist of:
- the filename of the sound effect to play
- the number of times it should be looped

## Attacks
attacks consist of:
- an *animation*
- a *sound effect*
- a damage value
- an attack type (melee, ranged)
- [for ranged]:
 - projectile sprite
 - travel speed
 - is the projectile blocked by walls/platforms/players
 - [maybe later on projectiles should have sounds]
- [for melee]:
 - a hitbox


## Behaviour
Behaviour will be modeled in different classes, all implementing an interface with an id and a simple loop function (arguments TBD).
