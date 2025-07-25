#include "Map.h"
#include "AssetManager.h"
#include "Camera.h"
#include "PerlinNoise.h" // MỚI: Thêm công cụ tạo thế giới
#include <iostream>
#include <chrono>
#include <sstream> // Cần cho việc đọc file CSV cũ nếu bạn muốn quay lại
#include <fstream> // Cần cho việc đọc file CSV cũ nếu bạn muốn quay lại

Map::Map(const std::string& tilesetId, int tileSize)
    : m_tilesetId(tilesetId),
    m_pTilesetTexture(nullptr),
    m_tileSize(tileSize),
    m_mapWidthInTiles(0),
    m_mapHeightInTiles(0),
    m_tilesetWidthInTiles(0)
{
}

Map::~Map() {}

// SỬA: Hàm này bây giờ sẽ tạo ra bản đồ
void Map::GenerateMap(int widthInTiles, int heightInTiles) {
    m_mapWidthInTiles = widthInTiles;
    m_mapHeightInTiles = heightInTiles;

    // 1. Lấy texture của tileset và tính toán chiều rộng của nó
    m_pTilesetTexture = AssetManager::GetInstance()->GetTexture(m_tilesetId);
    if (!m_pTilesetTexture) {
        std::cerr << "Failed to get tileset texture for map generation: " << m_tilesetId << std::endl;
        return;
    }
    int tilesetTextureWidth;
    SDL_QueryTexture(m_pTilesetTexture, nullptr, nullptr, &tilesetTextureWidth, nullptr);
    m_tilesetWidthInTiles = tilesetTextureWidth / m_tileSize;

    // 2. Khởi tạo Perlin Noise với một "hạt giống" ngẫu nhiên
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    PerlinNoise pn(seed);

    // 3. Vòng lặp để tạo dữ liệu cho từng ô
    m_mapData.assign(heightInTiles, std::vector<int>(widthInTiles, 0));

    double frequency = 8.0; // Tần số, càng lớn thì các "lục địa" càng nhỏ
    double scaleX = frequency / widthInTiles;
    double scaleY = frequency / heightInTiles;

    for (int y = 0; y < heightInTiles; ++y) {
        for (int x = 0; x < widthInTiles; ++x) {
            // Lấy giá trị noise (từ 0.0 đến 1.0)
            double noiseValue = pn.noise(x * scaleX, y * scaleY, 0.8);

            // 4. Chuyển giá trị noise thành Tile ID
            // Tile ID: 0=Grass, 1=Dirt, 2=Water, 3=Lava
            if (noiseValue > 0.8) {
                m_mapData[y][x] = 3; // Lava (rất hiếm)
            }
            else if (noiseValue > 0.6) {
                m_mapData[y][x] = 1; // Dirt (đồi)
            }
            else if (noiseValue > 0.3) {
                m_mapData[y][x] = 0; // Grass (đồng bằng)
            }
            else {
                m_mapData[y][x] = 2; // Water (vùng trũng)
            }
        }
    }
    std::cout << "Procedural map generated: " << m_mapWidthInTiles << "x" << m_mapHeightInTiles << " tiles." << std::endl;
}

// SỬA: BỔ SUNG PHẦN THÂN HÀM CÒN THIẾU
void Map::Render(SDL_Renderer* renderer) {
    if (!m_pTilesetTexture) return;

    SDL_Rect cameraBox = Camera::GetInstance()->GetViewBox();

    int startCol = cameraBox.x / m_tileSize;
    int endCol = (cameraBox.x + cameraBox.w) / m_tileSize + 1;
    int startRow = cameraBox.y / m_tileSize;
    int endRow = (cameraBox.y + cameraBox.h) / m_tileSize + 1;

    if (endCol > m_mapWidthInTiles) endCol = m_mapWidthInTiles;
    if (endRow > m_mapHeightInTiles) endRow = m_mapHeightInTiles;
    if (startCol < 0) startCol = 0;
    if (startRow < 0) startRow = 0;

    for (int row = startRow; row < endRow; ++row) {
        for (int col = startCol; col < endCol; ++col) {
            int tileId = m_mapData[row][col];
            if (tileId == -1) continue;

            int srcX = (tileId % m_tilesetWidthInTiles) * m_tileSize;
            int srcY = (tileId / m_tilesetWidthInTiles) * m_tileSize;
            SDL_Rect srcRect = { srcX, srcY, m_tileSize, m_tileSize };

            SDL_Rect destRect = { (col * m_tileSize) - cameraBox.x, (row * m_tileSize) - cameraBox.y, m_tileSize, m_tileSize };

            SDL_RenderCopy(renderer, m_pTilesetTexture, &srcRect, &destRect);
        }
    }
}

// SỬA: BỔ SUNG PHẦN THÂN HÀM CÒN THIẾU
int Map::GetWorldWidth() const {
    return m_mapWidthInTiles * m_tileSize;
}

// SỬA: BỔ SUNG PHẦN THÂN HÀM CÒN THIẾU
int Map::GetWorldHeight() const {
    return m_mapHeightInTiles * m_tileSize;
}

int Map::GetTileIdAt(int x, int y) const {
    if (x < 0 || x >= GetWorldWidth() || y < 0 || y >= GetWorldHeight()) {
        return -1; // Tọa độ nằm ngoài bản đồ
    }

    int tileCol = x / m_tileSize;
    int tileRow = y / m_tileSize;

    // Đảm bảo không truy cập ngoài mảng m_mapData
    if (tileRow >= 0 && tileRow < m_mapHeightInTiles && tileCol >= 0 && tileCol < m_mapWidthInTiles) {
        return m_mapData[tileRow][tileCol];
    }

    return -1; // Trả về -1 nếu có lỗi
}