#include <load_resources.h>

#include <texture_loader.h>

void LoadResources(TextureLoader& texture_loader) {
  // TODO: maybe load those from some file or directory

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

//  abilities
  texture_loader.LoadTexture("move");

  texture_loader.LoadTexture("shoot", "bullet");
  texture_loader.LoadTexture("grenade");

  texture_loader.LoadTexture("blow_the_ax");
  texture_loader.LoadTexture("armor");

  texture_loader.LoadTexture("drain");
  texture_loader.LoadTexture("counterattack");

  texture_loader.LoadTexture("magic_bullet");

  texture_loader.LoadTexture("power_bullet");
  texture_loader.LoadTexture("stone_skin");

  texture_loader.LoadTexture("poisoned_missile");

  texture_loader.LoadTexture("invisibility");
  texture_loader.LoadTexture("sabers");

  texture_loader.LoadTexture("sword_blow");
  texture_loader.LoadTexture("dodge");

  texture_loader.LoadTexture("hypnosis");
  texture_loader.LoadTexture("sniper_shot", "sniper_bullet");

  texture_loader.LoadTexture("claws");

  texture_loader.LoadTexture("tongue_of_fire");

//  effects
  texture_loader.LoadTexture("bullet");
  texture_loader.LoadTexture("bum");
  texture_loader.LoadTexture("detonation");
  texture_loader.LoadTexture("native_attack");
  texture_loader.LoadTexture("sniper_bullet_explosion");
  texture_loader.LoadTexture("poisoned_missile_explosion");

  texture_loader.LoadTexture("destroyer_attack");
}
