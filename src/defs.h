#pragma once

#include <cstdint>
#include <limits>
#include <string>

using EntityIdType = std::uint32_t;
using ClientIdType = std::uint32_t;
using IndexType = std::uint32_t;
using HealthType = std::int32_t;
using PowerType = std::int32_t;
using PlayerIdType = std::uint32_t;
using TextureKey = std::string;

static constexpr auto no_health = std::numeric_limits<HealthType>::max();
static constexpr auto no_power = std::numeric_limits<PowerType>::max();
static constexpr auto no_index = std::numeric_limits<IndexType>::max();
static constexpr auto no_player = std::numeric_limits<PlayerIdType>::max();

static constexpr auto undefined_client_id = std::numeric_limits<ClientIdType>::max();
