#pragma once
#include <string>
#include <map>

class SaveSystem {
public:
    // singleton
    static SaveSystem* GetInstance() {
        return s_Instance = (s_Instance ? s_Instance : new SaveSystem());
    }

    void LoadGame();
    void SaveGame();

    int GetGold() const;
    void AddGold(int amount);
    bool SpendGold(int amount);

    int GetUpgradeLevel(const std::string& upgradeId);
    void IncreaseUpgradeLevel(const std::string& upgradeId);

private:
    SaveSystem() {}
    static SaveSystem* s_Instance;

    std::string m_saveFilePath = "player_data.sav";

    int m_gold = 0;
    // Map để lưu: ID của nâng cấp -> Cấp độ hiện tại
    std::map<std::string, int> m_permanentUpgrades;
};