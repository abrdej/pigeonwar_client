#include <panel.h>

#include <text.h>
#include <texture_loader.h>

#include <iostream>

Panel::Panel(Renderer renderer, TextureLoader& texture_loader, int pos_x, int pos_y, int number_of_buttons,
             int number_of_effects)
    : renderer_(renderer), texture_loader_(texture_loader), pos_x_(pos_x), pos_y_(pos_y) {

  background_ = std::make_unique<Texture>(texture_loader_.GetTexture("panel_background"));
  background_->SetPos(0, pos_y);

  auto create_button = [&](const std::string& texture_key, int n, auto callback) {
    auto& new_button = buttons_.emplace_back(texture_loader.GetTexture(texture_key),
                                             pos_x + n * 60,
                                             pos_y + 10,
                                             60);
    new_button.OnClicked(callback);
    new_button.OnIn([this, n](Button& button) {
      if (on_in_) {
        on_in_(n);
      }
      button.GetTexture().Scale(1.2);
    });
    new_button.OnOut([this, n](Button& button) {
      button.GetTexture().Scale(1.0);
      if (on_out_) {
        on_out_(n);
      }
    });
    buttons_order_.emplace_back(ButtonOrderHolder{&new_button});
  };

  buttons_.reserve(number_of_buttons + number_of_effects + 1u);
  for (int n = 0; n < number_of_buttons; ++n) {
    create_button("border", n, [n](Button& button) {
      std::cout << "Button: " << n << " clicked\n";
    });
  }
  for (int n = 0; n < number_of_effects; ++n) {
    create_button("border", n + number_of_buttons + 1, [n](Button& button) {
      std::cout << "Effect: " << n << " clicked\n";
    });
  }
  create_button("end_turn", number_of_buttons + number_of_effects + 2, [](Button& button) {
    std::cout << "End turn clicked\n";
  });
}

void Panel::Draw(Window& window) {
  background_->Draw(window);
  std::sort(std::begin(buttons_order_), std::end(buttons_order_), Compare{});
  for (auto& button : buttons_order_) {
    button.button->Draw(window);
  }
  if (entity_button_) {
    entity_button_->Draw(window);
  }
  if (entity_name_) {
    entity_name_->Draw(window);
  }
  if (entity_health_) {
    entity_health_->Draw(window);
  }
  if (entity_power_) {
    entity_power_->Draw(window);
  }
}

void Panel::InteractMove(int x, int y) {
  for (auto& button : buttons_) {
    button.InteractMove(x, y);
  }
  if (entity_button_) {
    entity_button_->InteractMove(x, y);
  }
}

void Panel::InteractPress(int x, int y) {
  for (auto& button : buttons_) {
    button.InteractPress(x, y);
  }
  if (entity_button_) {
    entity_button_->InteractPress(x, y);
  }
}

bool Panel::Clicked(int x, int y) const {
  return x >= pos_x_ && x <= pos_x_ + buttons_.size() * 60 && y >= pos_y_ && y <= pos_y_ + buttons_.size() * 60;
}

void Panel::SetCurrentEntity(const EntityProperties& entity_properties) {
  auto entity_texture = texture_loader_.GetTexture(entity_properties.name);
  entity_texture.Scale(1.5);

  entity_button_ = std::make_unique<Button>(entity_texture, 25, 615, 120);
  entity_button_->OnIn([this](Button& button) {
    button.GetTexture().Scale(1.7);
    if (on_in_) {
      // TODO: clean up button ids
      on_in_(-1);
    }
  });
  entity_button_->OnOut([this](Button& button) {
    if (on_out_) {
      on_out_(-1);
    }
    button.GetTexture().Scale(1.5);
  });

  entity_name_ = std::make_unique<Text>(renderer_, 18);
  entity_health_ = std::make_unique<Text>(renderer_, 22);
  entity_power_ = std::make_unique<Text>(renderer_, 22);

  entity_name_->SetPos(15, 600);
  entity_name_->SetText(entity_properties.name);
  entity_name_->SetColor(Color{235, 235, 235});

  if (entity_properties.health != no_health) {
    entity_health_->SetPos(95, 605);
    entity_health_->SetText(std::to_string(entity_properties.health));
    entity_health_->SetColor(Color{50, 122, 55});
  }

  if (entity_properties.power != no_power) {
    entity_power_->SetPos(100, 635);
    entity_power_->SetText(std::to_string(entity_properties.power));
    entity_power_->SetColor(Color{90, 114, 140});
  }
}

void Panel::OnIn(std::function<void(int)> callback) {
  on_in_ = std::move(callback);
}

void Panel::OnOut(std::function<void(int)> callback) {
  on_out_ = std::move(callback);
}
