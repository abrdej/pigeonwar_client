#pragma once

#include <button.h>
#include <button_id.h>
#include <entity_properties.h>
#include <game_state.h>
#include <text.h>
#include <timer.h>

#include <memory>
#include <vector>

class TextureLoader;

class Panel {
 public:
  using ButtonCallback = std::function<void(ButtonId)>;

  Panel(Renderer renderer, TextureLoader& texture_loader,
        int pos_x, int pos_y, int number_of_buttons, int number_of_effects);

  void Draw(Window& window);

  void InteractMove(int x, int y);
  void InteractPress(int x, int y);
  [[nodiscard]] bool Clicked(int x, int y) const;

  void SetCurrentEntity(const EntityProperties& entity_properties);
  void SetLocalState(const LocalGameState& local_game_state);

  void OnIn(ButtonCallback callback);
  void OnOut(ButtonCallback callback);
  void OnClicked(ButtonCallback callback);

 private:
  Renderer renderer_;
  TextureLoader& texture_loader_;
  int pos_x_{0};
  int pos_y_{0};

  ButtonCallback on_in_;
  ButtonCallback on_out_;
  ButtonCallback on_clicked_;

  std::unique_ptr<Texture> background_;

  std::unique_ptr<Button> entity_button_;
  std::unique_ptr<Text> entity_name_;
  std::unique_ptr<Text> entity_health_;
  std::unique_ptr<Text> entity_power_;

  std::vector<Button> buttons_;
  std::vector<Texture> icons_;
  std::vector<ButtonOrderHolder> buttons_order_;
};
