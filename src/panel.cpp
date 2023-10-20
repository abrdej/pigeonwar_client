#include <panel.h>

#include <text.h>
#include <texture_loader.h>

#include <iostream>

Panel::Panel(TextureLoader& texture_loader, int pos_x, int pos_y, int number_of_buttons)
    : texture_loader_(texture_loader), pos_x_(pos_x), pos_y_(pos_y) {
  buttons_.reserve(number_of_buttons);
  for (int n = 0; n < number_of_buttons; ++n) {
    buttons_.emplace_back(texture_loader.GetTexture("border"),
                          pos_x + n * 60,
                          pos_y,
                          60).OnClicked([n](const Button& button) {
      std::cout << "Button: " << n << " clicked\n";
    });
    buttons_order_.emplace_back(ButtonOrderHolder{&buttons_.back()});
  }
}

void Panel::Draw(Window& window) {
  std::sort(std::begin(buttons_order_), std::end(buttons_order_), Compare{});
  for (auto& button : buttons_order_) {
    button.button->Draw(window);
  }
  if (entity_button_) {
    entity_button_->Draw(window);
  }
}

void Panel::InteractMove(int x, int y) {
  for (auto& button : buttons_) {
    button.InteractMove(x, y);
  }
}

void Panel::InteractPress(int x, int y) {
  for (auto& button : buttons_) {
    button.InteractPress(x, y);
  }
}

bool Panel::Clicked(int x, int y) const {
  return x >= pos_x_ && x <= pos_x_ + buttons_.size() * 60 && y >= pos_y_ && y <= pos_y_ + buttons_.size() * 60;
}

//void Panel::SetCurrentEntity(const Entity& entity) {
//  std::string entity_name;
//
//  entity_button_ = std::make_unique<Button>(texture_loader_.GetTexture(entity_name), 15, 600, 120);
//  entity_name_ = std::make_unique<Text>(texture_loader_.GetTexture(entity_name), 15, 600, 120);
//
//}
