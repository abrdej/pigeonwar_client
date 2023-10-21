#pragma once

#include <cstdint>
#include <limits>

using EntityIdType = std::uint32_t;
using ClientIdType = std::uint32_t;
using IndexType = std::uint32_t;

static constexpr auto no_health = std::numeric_limits<std::int32_t>::max();
static constexpr auto no_power = std::numeric_limits<std::int32_t>::max();
static constexpr auto no_index = std::numeric_limits<IndexType>::max();

static constexpr auto undefined_client_id = std::numeric_limits<ClientIdType>::max();
