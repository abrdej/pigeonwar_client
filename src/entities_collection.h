#pragma once

#include <entity.h>

#include <deque>
#include <unordered_map>

class Window;
class TextureLoader;

class EntitiesCollection {
 public:
  EntitiesCollection(const Window& window, const TextureLoader& texture_loader);
  void Add(EntityIdType entity_id, const EntityProperties& entity_properties);
  Entity& Get(EntityIdType entity_id);
  const Entity& Get(EntityIdType entity_id) const;
  std::optional<EntityProperties> EntityPropertiesForIndex(IndexType index) const;
  void Remove(EntityIdType entity_id);

  void Draw(Window& window);

 private:
  [[noreturn]] static void ThrowEntityIdNotExists(EntityIdType entity_id);

  const Window& window_;
  const TextureLoader& texture_loader_;
  std::unordered_map<EntityIdType, Entity> entities_;
};
