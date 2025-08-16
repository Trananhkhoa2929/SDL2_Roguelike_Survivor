#include "SaveSystem.h"
#include <fstream>
#include <iostream>
#include <sstream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

SaveSystem* SaveSystem::s_Instance = nullptr;

void SaveSystem::LoadGame() {
#ifdef __EMSCRIPTEN__
    // Đơn giản hóa cho web - không dùng IDBFS
    m_saveFilePath = "/tmp/player_data.sav";

    // Khởi tạo giá trị mặc định cho web
    m_gold = 0;
    m_permanentUpgrades.clear();
    std::cout << "Web version: Using default save data." << std::endl;
    return;
#endif

    std::ifstream saveFile(m_saveFilePath);
    if (!saveFile.is_open()) {
        std::cout << "No save file found. Starting fresh." << std::endl;
        SaveGame();
        return;
    }

    std::string line;
    while (std::getline(saveFile, line)) {
        std::stringstream ss(line);
        std::string key;
        std::string value;
        if (std::getline(ss, key, '=') && std::getline(ss, value)) {
            if (key == "gold") {
                m_gold = std::stoi(value);
            }
            else {
                m_permanentUpgrades[key] = std::stoi(value);
            }
        }
    }
    std::cout << "Game data loaded." << std::endl;
    saveFile.close();
}

void SaveSystem::SaveGame() {
#ifdef __EMSCRIPTEN__
    // Đơn giản hóa cho web - chỉ in log
    std::cout << "Web version: Save data updated (not persistent)." << std::endl;
    return;
#endif

    std::ofstream saveFile(m_saveFilePath);
    if (!saveFile.is_open()) {
        std::cerr << "Failed to open save file for writing!" << std::endl;
        return;
    }

    saveFile << "gold=" << m_gold << std::endl;
    for (const auto& pair : m_permanentUpgrades) {
        saveFile << pair.first << "=" << pair.second << std::endl;
    }
    std::cout << "Game data saved." << std::endl;
    saveFile.close();
}

int SaveSystem::GetGold() const { return m_gold; }

void SaveSystem::AddGold(int amount) { m_gold += amount; }

bool SaveSystem::SpendGold(int amount) {
    if (m_gold >= amount) {
        m_gold -= amount;
        return true;
    }
    return false;
}

int SaveSystem::GetUpgradeLevel(const std::string& upgradeId) {
    if (m_permanentUpgrades.count(upgradeId)) {
        return m_permanentUpgrades[upgradeId];
    }
    return 0;
}

void SaveSystem::IncreaseUpgradeLevel(const std::string& upgradeId) {
    if (m_permanentUpgrades.count(upgradeId)) {
        m_permanentUpgrades[upgradeId]++;
    }
    else {
        m_permanentUpgrades[upgradeId] = 1;
    }
}