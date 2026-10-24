#pragma once
class Arduino_Canvas;
// Ambient visuals are PC-only and rendered behind sprites and interactive UI.
void pokebyteApplyAmbient(Arduino_Canvas *canvas, bool minigame);
void pokebyteToggleAmbient();
void pokebyteToggleWeather();
