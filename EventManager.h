// file: EventManager.h
#pragma once
#include <functional>
#include <map>
#include <vector>
#include <typeindex>
#include <memory>
#include "Event.h"

// Lớp quản lý sự kiện trung tâm (Singleton)
class EventManager {
public:
    static EventManager* GetInstance() {
        static EventManager instance;
        return &instance;
    }

    // Gửi đi một sự kiện
    template<typename TEvent, typename... TArgs>
    void Publish(TArgs&&... args) {
        auto type = std::type_index(typeid(TEvent));
        if (m_subscribers.find(type) != m_subscribers.end()) {
            auto event = std::make_shared<TEvent>(std::forward<TArgs>(args)...);
            for (auto& callback : m_subscribers.at(type)) {
                callback(*event);
            }
        }
    }

    // Đăng ký để lắng nghe một loại sự kiện
    template<typename TEvent>
    void Subscribe(std::function<void(Event&)> callback) {
        auto type = std::type_index(typeid(TEvent));
        m_subscribers[type].push_back(callback);
    }

private:
    EventManager() = default;
    ~EventManager() = default;
    std::map<std::type_index, std::vector<std::function<void(Event&)>>> m_subscribers;
};