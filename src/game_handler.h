#pragma once

class TextureLoader;
class EntitiesCollection;
class Renderer;

struct GameHandler {
  virtual TextureLoader& GetTextureLoader() = 0;
  virtual EntitiesCollection& GetEntitiesCollection() = 0;
  virtual Renderer GetRenderer() = 0;
  virtual void ClearSelectedIndex() = 0;
};
