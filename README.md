# cppp-metroidvania
## Building and Running
We use CMake, and included an empty build directory for convenience.
In whatever build directory you use, there should be an executable called *game*, which starts the game.

## Playing the Game
### Basic Controls
Use A and D for moving left and right.
You can jump using Space and slide using L.
While jumping you can press S to fall at terminal velocity.
Sliding makes you invulnerable (we didn't have a rolling animation).
By pressing K you can start attacking.
After an attack has ended, you have 50ms to start a combo attack with increased damage (you can also just hold the key).

### Transformation potions
The 3 first bosses drop transformation potions, which allow you to use one of their attacks.
To use them, press U, I, and O, respectively.
On picking up one of the potions, a prompt tells you which button to press.
