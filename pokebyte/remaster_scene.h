#pragma once
#include <cstdint>
class Arduino_Canvas;
void pokebyteEnhanceOriginalScene(Arduino_Canvas *canvas, uint8_t biome, uint32_t now, bool gameNight, int gameHour);
void pokebyteToggleRemaster();
void pokebyteSetPreviewHour(int hour);
