#include <entities_collection.h>

#include <texture_loader.h>
#include <window.h>

EntitiesCollection::EntitiesCollection(const Window& window, const TextureLoader& texture_loader)
    : window_(window), texture_loader_(texture_loader) {
}

void EntitiesCollection::Add(EntityIdType entity_id, const EntityProperties& entity_properties) {
  entities_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(entity_id),
                    std::forward_as_tuple(window_.GetRenderer(), texture_loader_, entity_properties));
}

Entity& EntitiesCollection::Get(EntityIdType entity_id) {
  try {
    return entities_.at(entity_id);
  } catch (std::out_of_range&) {
    ThrowEntityIdNotExists(entity_id);
  }
}

const Entity& EntitiesCollection::Get(EntityIdType entity_id) const {
  try {
    return entities_.at(entity_id);
  } catch (std::out_of_range&) {
    ThrowEntityIdNotExists(entity_id);
  }
}

std::optional<EntityProperties> EntitiesCollection::EntityPropertiesForIndex(IndexType index) const {
  for (const auto& [entity_id, entity] : entities_) {
    if (entity.GetProperties().index == index) {
      return entity.GetProperties();
    }
  }
  return std::nullopt;
}

void EntitiesCollection::Remove(EntityIdType entity_id) {
  entities_.erase(entity_id);
}

void EntitiesCollection::Draw(Window& window) {
  std::vector<Entity*> sorted_entities;
  sorted_entities.reserve(entities_.size());
  for (auto& [entity_id, entity] : entities_) {
    sorted_entities.emplace_back(&entity);
  }
  std::sort(std::begin(sorted_entities), std::end(sorted_entities), [](const auto e1, const auto e2) {
    return e1->GetOrder() < e2->GetOrder();
  });
  for (const auto& entity : sorted_entities) {
    entity->Draw(window);
  }
}

void EntitiesCollection::ThrowEntityIdNotExists(EntityIdType entity_id) {
  throw std::out_of_range(
      "EntitiesCollection::Get: entity with id: " + std::to_string(entity_id) + " does not exists!");
}
