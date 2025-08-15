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
    // Sync IndexedDB to virtual filesystem
    EM_ASM(
        FS.mkdir('/persistent');
    FS.mount(IDBFS, {}, '/persistent');
    FS.syncfs(true, function(err) {
        if (err) {
            console.log('Failed to load from IndexedDB: ' + err);
        }
        else {
            console.log('Loaded save data from IndexedDB');
        }
    });
        );
    m_saveFilePath = "/persistent/player_data.sav";
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

#ifdef __EMSCRIPTEN__
    // Sync virtual filesystem to IndexedDB
    EM_ASM(
        FS.syncfs(false, function(err) {
        if (err) {
            console.log('Failed to save to IndexedDB: ' + err);
        }
        else {
            console.log('Saved data to IndexedDB');
        }
    });
    );
#endif
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