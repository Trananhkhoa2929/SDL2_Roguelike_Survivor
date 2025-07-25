// file: PlayerEvents.h
#pragma once
#include "Event.h"

class Entity; // Forward declaration

// Sự kiện được gửi đi khi người chơi nhận sát thương
struct PlayerDamagedEvent : public Event {
    Entity* playerEntity;
    int damageAmount;

    PlayerDamagedEvent(Entity* player, int damage)
        : playerEntity(player), damageAmount(damage) {
    }
};