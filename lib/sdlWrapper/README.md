# The most basic version of the wrapper will be created somewhere else

Our plan is to create a very basic wrapper for SDL in a scratch project somewhere else.
It will then be migrated to here, and new features will be added as needed.
We don't want to have to touch SDL at all in our actual game code, everything should go via this wrapper.
Possibly we need to extract different parts of this wrapper (e.g. for sound) to different submodules, etc.

## Minimal features

- Window creation (no resize)
- Render background (that ideally pans somewhat)
- Render immobile entities
- Render mobile entities (animations via spritesheets), including bullets, etc.
- Render player character

- Sound (not of critical importance)
