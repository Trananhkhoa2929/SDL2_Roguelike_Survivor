#pragma once
#include <string>
#include <vector>
//#include <functional>
#include <random>
#include <chrono>
#include <algorithm>

#include "Entity.h"
#include "WeaponComponents.h"

// SỬA: Struct UpgradeData được định nghĩa lại để linh hoạt hơn
struct UpgradeData {
    std::string id;
    std::string name;
    std::string description;

    // Phân loại để lọc nâng cấp một cách thông minh
    enum class Category { GENERAL, PROJECTILE, AREA, NEW_WEAPON };
    Category category = Category::GENERAL;

    std::string targetStat; // Tên của chỉ số trong StatsComponent hoặc ID vũ khí mới
    float value;            // Giá trị để cộng/nhân
    bool isMultiplier;      // True nếu là nhân (%), false nếu là cộng (+)
};

class UpgradeRegistry {
public:
    UpgradeRegistry() {
        m_RandomEngine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        PopulateUpgrades();
    }

    // SỬA: Hàm này bây giờ lọc nâng cấp một cách chính xác
    std::vector<UpgradeData> GetRandomChoices(Entity* player, int count) {
        if (!player) return {};

        std::vector<UpgradeData> availablePool;
        std::vector<UpgradeData> generalPool = m_GeneralUpgrades; // Tạo bản sao để có thể xáo trộn

        // 1. Ưu tiên các nâng cấp cho vũ khí hiện có
        if (player->HasComponent<MagicWandComponent>()) {
            availablePool.insert(availablePool.end(), m_ProjectileUpgrades.begin(), m_ProjectileUpgrades.end());
        }
        if (player->HasComponent<WhipComponent>() || player->HasComponent<GarlicAuraComponent>()) {
            availablePool.insert(availablePool.end(), m_AreaUpgrades.begin(), m_AreaUpgrades.end());
        }

        // 2. Thêm các vũ khí mới mà người chơi chưa có vào danh sách
        if (!player->HasComponent<MagicWandComponent>()) availablePool.push_back(m_NewWeapon_MagicWand);
        if (!player->HasComponent<WhipComponent>()) availablePool.push_back(m_NewWeapon_Whip);
        if (!player->HasComponent<GarlicAuraComponent>()) availablePool.push_back(m_NewWeapon_Garlic);

        // 3. Xáo trộn các nâng cấp đã được ưu tiên
        std::shuffle(availablePool.begin(), availablePool.end(), m_RandomEngine);

        // 4. Lấy ra các lựa chọn cuối cùng, nếu không đủ thì bổ sung bằng các nâng cấp chung
        std::vector<UpgradeData> choices;
        for (int i = 0; i < count; ++i) {
            if (!availablePool.empty()) {
                choices.push_back(availablePool.back());
                availablePool.pop_back();
            }
            else if (!generalPool.empty()) {
                // Nếu hết nâng cấp đặc biệt, lấy từ các nâng cấp chung
                std::shuffle(generalPool.begin(), generalPool.end(), m_RandomEngine);
                choices.push_back(generalPool.back());
                generalPool.pop_back();
            }
        }
        return choices;
    }

private:
    // SỬA: Chia kho thành các nhóm để dễ quản lý
    std::vector<UpgradeData> m_GeneralUpgrades;
    std::vector<UpgradeData> m_ProjectileUpgrades;
    std::vector<UpgradeData> m_AreaUpgrades;
    UpgradeData m_NewWeapon_MagicWand;
    UpgradeData m_NewWeapon_Whip;
    UpgradeData m_NewWeapon_Garlic;

    std::mt19937 m_RandomEngine;

    // SỬA: Định nghĩa lại toàn bộ kho nâng cấp theo triết lý "Lego"
    void PopulateUpgrades() {
        // Nâng cấp chung
        m_GeneralUpgrades.push_back({ "more_damage", "Power Gauntlet", "+10% Damage", UpgradeData::Category::GENERAL, "damageMultiplier", 1.10f, true });
        m_GeneralUpgrades.push_back({ "faster_cooldown", "Tome of Celerity", "-8% Cooldown", UpgradeData::Category::GENERAL, "cooldownReduction", 0.92f, true });
        m_GeneralUpgrades.push_back({ "more_health", "Thicker Armor", "+20% Max Health", UpgradeData::Category::GENERAL, "maxHealthMultiplier", 1.20f, true });

        // Nâng cấp cho vũ khí bắn đạn
        m_ProjectileUpgrades.push_back({ "more_projectiles", "Duplicator Tome", "+1 Projectile", UpgradeData::Category::PROJECTILE, "projectileCountBonus", 1, false });
        m_ProjectileUpgrades.push_back({ "faster_projectiles", "Swiftness Gem", "+20% Projectile Speed", UpgradeData::Category::PROJECTILE, "projectileSpeedMultiplier", 1.20f, true });

        // Nâng cấp cho vũ khí cận chiến/hào quang
        m_AreaUpgrades.push_back({ "bigger_area", "Bracer of Expansion", "+25% Area of Effect", UpgradeData::Category::AREA, "areaSizeMultiplier", 1.25f, true });

        // Định nghĩa các vũ khí mới
        m_NewWeapon_MagicWand = { "new_weapon_magic_wand", "Magic Wand", "Automatically shoots at the nearest enemy.", UpgradeData::Category::NEW_WEAPON, "magic_wand", 0, false };
        m_NewWeapon_Whip = { "new_weapon_whip", "Whip", "Whips horizontally in front.", UpgradeData::Category::NEW_WEAPON, "whip", 0, false };
        m_NewWeapon_Garlic = { "new_weapon_garlic", "Garlic", "Aura that deals continuous damage.", UpgradeData::Category::NEW_WEAPON, "garlic", 0, false };
    }
};