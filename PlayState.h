#pragma once
#include "Map.h"
#include "IGameState.h"
#include "PauseState.h"
#include "EntityManager.h"
#include "RenderSystem.h"
#include "PlayerInputSystem.h"
#include "CollisionSystem.h"
#include "AISystem.h"
#include "EnemySpawnerSystem.h"
#include "HealthSystem.h"
#include "WeaponControlSystem.h"
#include "TextRenderSystem.h"
#include "ProjectileSystem.h"
#include "FollowSystem.h"
#include "LevelSystem.h"
#include "LootSystem.h"
#include "UpgradeSystem.h"
#include "DamageSystem.h"
#include "StatsRenderSystem.h" 
#include "GameEventSystem.h"
#include "TimeRenderSystem.h"
#include "RangedAISystem.h"
#include <string>

// MỚI: Lấy kích thước thế giới từ bản đồ
const int WORLD_WIDTH = 50 * 32 * 2; // mapWidth * tileSize * scale
const int WORLD_HEIGHT = 40 * 32 * 2; // mapHeight * tileSize * scale

class Entity;

class PlayState : public IGameState
{
public:
	PlayState(const std::string& startingWeaponId);
	~PlayState(); // MỚI: Cần destructor để hủy map	

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	// MỚI: Hàm được gọi từ LevelSystem khi người chơi lên cấp
	void TriggerLevelUpScreen();

private:

	// MỚI: Thêm một con trỏ tới đối tượng Map
	Map* m_pMap = nullptr;

	EntityManager m_EntityManager;
	RenderSystem m_RenderSystem;

	PlayerInputSystem m_PlayerInputSystem;
	CollisionSystem m_CollisionSystem;

	AISystem m_AISystem;
	EnemySpawnerSystem m_EnemySpawnerSystem;

	RangedAISystem m_RangedAISystem;
	FollowSystem m_FollowSystem; // MỚI: Khai báo biến thành viên
	WeaponControlSystem m_WeaponControlSystem;          // <--- Thêm biến thành viên
	DamageSystem m_DamageSystem;
	HealthSystem m_HealthSystem;          // <--- Thêm biến thành viên
	TextRenderSystem m_TextRenderSystem;  // <--- Thêm biến thành viên (cho bước UI)
	ProjectileSystem m_ProjectileSystem; // <--- Thêm biến thành viên

	// SỬA: Chuyển các con trỏ thành biến thành viên (member variables).
	// Điều này cho phép chúng ta quản lý chúng trong suốt vòng đời của PlayState.
	Entity* m_player = nullptr;
	Entity* m_scoreUI = nullptr;

	LootSystem m_LootSystem;         // MỚI
	LevelSystem m_LevelSystem;       // MỚI

	// SỬA: Thêm các biến thành viên mới để quản lý trạng thái UI
	bool m_isPausedForUpgrade = false;
	UpgradeRegistry m_upgradeRegistry;
	std::vector<UpgradeData> m_currentUpgradeChoices;

	// MỚI: Lưu lại ID vũ khí khởi đầu
	std::string m_startingWeaponId;

	// MỚI: Các hàm riêng để quản lý UI
	StatsRenderSystem m_StatsRenderSystem;

	// MỚI: Thêm các biến thành viên mới
	float m_playTime;
	GameEventSystem m_GameEventSystem;
	TimeRenderSystem m_TimeRenderSystem;

	// MỚI: Thêm một con trỏ cho nút pause để xử lý click
	Entity* m_pauseButton = nullptr;

	void RenderUpgradeUI();
	void HandleUpgradeSelection();
	void ApplyUpgradeAndResume(const UpgradeData& chosenUpgrade);
};