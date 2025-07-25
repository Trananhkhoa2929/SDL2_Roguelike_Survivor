#include "SaveSystem.h"
#include <fstream>
#include <iostream>
#include <sstream>

SaveSystem* SaveSystem::s_Instance = nullptr;

void SaveSystem::LoadGame() {
    std::ifstream saveFile(m_saveFilePath);
    if (!saveFile.is_open()) {
        std::cout << "No save file found. Starting fresh." << std::endl;
        // Nếu không có file save, tạo một file mới với giá trị mặc định
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
                // Các dòng khác là nâng cấp vĩnh viễn
                m_permanentUpgrades[key] = std::stoi(value);
            }
        }
    }
    std::cout << "Game data loaded." << std::endl;
    saveFile.close();
}

void SaveSystem::SaveGame() {
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
    return 0; // Mặc định là cấp 0 nếu chưa mua
}

void SaveSystem::IncreaseUpgradeLevel(const std::string& upgradeId) {
    if (m_permanentUpgrades.count(upgradeId)) {
        m_permanentUpgrades[upgradeId]++;
    }
    else {
        m_permanentUpgrades[upgradeId] = 1;
    }
}