#pragma once
class Arduino_Canvas;
bool pokebyteDrawSceneOverride(Arduino_Canvas *canvas);
bool pokebyteDrawMinigameSceneOverride(Arduino_Canvas *canvas);
void pokebyteToggleBackground();
void pokebyteSetPreviewMode(const char *mode);
const char* pokebyteCurrentSceneMode();
