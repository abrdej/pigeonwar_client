#include <animation_plugin_provider_interface.h>

#include <iostream>

#include <boost/dll/alias.hpp>

#include <animations/animation.h>
#include <animations/animation_helper.h>
#include <entities_collection.h>
#include <renderer.h>

class MoveAnimation : public DummyDrawAnimation {
  using MoveToT = MoveTo<Entity, true, true>;
 public:
  explicit MoveAnimation(Entity& entity, IndexType target_index);
  bool Update(std::chrono::milliseconds delta_time) override;

  static constexpr auto name = "move";

 private:
  Entity& entity_;
  IndexType target_index_;
  std::unique_ptr<MoveToT> move_to_;
  float speed_{0.01f};
};

MoveAnimation::MoveAnimation(Entity& entity, IndexType target_index)
    : entity_(entity), target_index_(target_index) {
  auto [x, y] = IndexToPos(target_index_);
  move_to_ = std::make_unique<MoveToT>(entity, x, y, speed_);
}

bool MoveAnimation::Update(std::chrono::milliseconds delta_time) {
  if (move_to_->Update(delta_time)) {
    entity_.SetIndex(target_index_);
    return true;
  }
  return false;
}

class ChangeHealthAnimation : public AnimationInterface {
  using MoveByT = MoveBy<Text, false, false>;
 public:
  explicit ChangeHealthAnimation(Renderer renderer, Entity& entity, HealthType change_amount);
  bool Update(std::chrono::milliseconds delta_time) override;
  void Draw(Window& window) const override;

  static constexpr auto name = "change_health";

 private:
  Renderer renderer_;
  std::unique_ptr<MoveByT> move_by_;
  Entity& entity_;
  HealthType change_amount_;
  Color color_;
  std::unique_ptr<Text> text_;
};

ChangeHealthAnimation::ChangeHealthAnimation(Renderer renderer, Entity& entity, HealthType change_amount)
    : renderer_(renderer), entity_(entity), change_amount_(change_amount),
      color_(change_amount < 0 ? Color{173, 32, 14, 255} : Color{47, 117, 66, 255}) {

  auto [x, y] = entity.GetPos();
  text_ = std::make_unique<Text>(renderer_, 32); // TODO: Parametrize size
  text_->SetText(std::to_string(change_amount_));
  text_->SetAnchor(0.5, 1.0);
  text_->SetPos(x, y);
  text_->SetColor(color_);

  // TODO: speed here is not ok
  move_by_ = std::make_unique<MoveByT>(*text_, 0, -25, 0.00001f);
}

bool ChangeHealthAnimation::Update(std::chrono::milliseconds delta_time) {
  if (move_by_->Update(delta_time)) {
    entity_.ChangeHealth(change_amount_);
    move_by_ = nullptr;
    text_ = nullptr;
    return true;
  }
  return false;
}

void ChangeHealthAnimation::Draw(Window& window) const {
  if (text_) {
    text_->Draw(window);
  }
}

template <>
AnimationInterfacePtr AnimationTranslator<MoveAnimation>(GameHandler& game_handler, const DataType& data) {
  game_handler.ClearSelectedIndex();
  EntityIdType entity_id = data[1];
  IndexType index = data[2];
  return std::make_unique<MoveAnimation>(game_handler.GetEntitiesCollection().Get(entity_id), index);
}

template <>
AnimationInterfacePtr AnimationTranslator<ChangeHealthAnimation>(GameHandler& game_handler, const DataType& data) {
  EntityIdType entity_id = data[1];
  HealthType change_amount = data[2];
  return std::make_unique<ChangeHealthAnimation>(game_handler.GetRenderer(),
                                                 game_handler.GetEntitiesCollection().Get(entity_id),
                                                 change_amount);
}

using CommonAnimationsProvider = AnimationsPluginProvider<MoveAnimation, ChangeHealthAnimation>;

BOOST_DLL_ALIAS(CreateAnimationProvider<CommonAnimationsProvider>, animation_plugin_provider)
