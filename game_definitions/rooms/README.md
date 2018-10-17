# Room definitions
Every room gets a separate definition file.
Maybe different subdirectories for different areas.

Each definition should contain the following things:
- The room layout (in some human readable format)
- Entity spawn positions
- The tile set to use
- The global coordinate of the top left corner (maybe we want to be able to use any point as reference?)
  My idea is, we mark any point in the layout file as reference point, and define that position in global coordinates somewhere else.
- Background
- Music
