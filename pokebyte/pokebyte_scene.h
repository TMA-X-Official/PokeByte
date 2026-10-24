#pragma once
class Arduino_Canvas;
bool pokebyteDrawSceneOverride(Arduino_Canvas *canvas);
void pokebyteToggleBackground();

// Same selected time-of-day remaster, extended to the full mini-game field.
bool pokebyteDrawMinigameSceneOverride(Arduino_Canvas *canvas);
