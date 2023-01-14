#pragma once

#include <chrono>

#include <entity.h>
#include <iostream>

inline double Distance(int from_x, int from_y, int to_x, int to_y) {
  return std::sqrt(std::pow(from_x - to_x, 2) + std::pow(from_y - to_y, 2));
}

struct MoveAnimation {
  int entity_id;
  int to_index;

  float delta_x;
  float delta_y;

  float to_x;
  float to_y;

  float speed = 0.1;

  Entity* entity_{nullptr};

  void Handle(Entity& entity) {
    entity_ = &entity;

    int x, y;
    IndexToPos(to_index, x, y);
    to_x = x;
    to_y = y;

    std::cout << "To x: " << to_x << "\n";
    std::cout << "To y: " << to_y << "\n";

    float from_x, from_y;
    entity.GetPos(from_x, from_y);

    std::cout << "From x: " << from_x << "\n";
    std::cout << "From y: " << from_y << "\n";

    auto distance = Distance(from_x, from_y, to_x, to_y);

    entity.Flip(from_x - to_x > 0);

    delta_x = (to_x - from_x);
    delta_y = (to_y - from_y);

    delta_x = delta_x / distance;
    delta_y = delta_y / distance;

    std::cout << "delta_x: " << delta_x << "\n";
    std::cout << "delta_y: " << delta_y << "\n";
  }

  bool Update(std::chrono::milliseconds delta_time) {
    float x, y;
    entity_->GetPos(x, y);

    auto pos_x = x + speed * delta_x * delta_time.count();
    auto pos_y = y + speed * delta_y * delta_time.count();

    std::cout << "to_x: " << to_x << "\n";
    std::cout << "to_y: " << to_y << "\n";
    std::cout << "pos_x: " << pos_x << "\n";
    std::cout << "pos_y: " << pos_y << "\n";

    if (std::abs(pos_x - to_x) < 2.5 && std::abs(pos_y - to_y) < 2.5) {
      entity_->SetPos(to_x, to_y);
      return true;
    } else {
      entity_->SetPos(pos_x, pos_y);
      return false;
    }
  }
};
