#include <load_resources.h>

#include <texture_loader.h>

void LoadResources(TextureLoader& texture_loader) {
// entities
  texture_loader.LoadTexture("destroyer");
  texture_loader.LoadTexture("golem");
  texture_loader.LoadTexture("monk");
  texture_loader.LoadTexture("native");
  texture_loader.LoadTexture("poisoner");
  texture_loader.LoadTexture("saberhand");
  texture_loader.LoadTexture("samurai");
  texture_loader.LoadTexture("saurian");
  texture_loader.LoadTexture("shooter");
  texture_loader.LoadTexture("sniper");
  texture_loader.LoadTexture("wolf");

// environment
  texture_loader.LoadTexture("tree");
  texture_loader.LoadTexture("fir");
  texture_loader.LoadTexture("fire");
  texture_loader.LoadTexture("stone");
  texture_loader.LoadTexture("water");
  texture_loader.LoadTexture("wall");

//  grass
  texture_loader.LoadTexture("grass");
  texture_loader.LoadTexture("grass_move");
  texture_loader.LoadTexture("grass_damage");
  texture_loader.LoadTexture("grass_attack");
  texture_loader.LoadTexture("grass_boost");
  texture_loader.LoadTexture("grass_friendly");
  texture_loader.LoadTexture("grass_selected");

//  panel
  texture_loader.LoadTexture("border");
  texture_loader.LoadTexture("end_turn");
  texture_loader.LoadTexture("panel_background");
}
