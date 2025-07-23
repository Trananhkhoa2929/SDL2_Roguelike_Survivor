#pragma once
#include <string>
#include <vector>
#include <SDL.h>

class Map {
public:
    Map(const std::string& tilesetId, int tileSize);
    ~Map();

    // SỬA: Thay thế LoadMap bằng GenerateMap
    void GenerateMap(int widthInTiles, int heightInTiles);

    void Render(SDL_Renderer* renderer);

    int GetWorldWidth() const;
    int GetWorldHeight() const;

private:
    std::string m_tilesetId;
    SDL_Texture* m_pTilesetTexture;
    int m_tileSize;
    int m_mapWidthInTiles;
    int m_mapHeightInTiles;
    int m_tilesetWidthInTiles;
    std::vector<std::vector<int>> m_mapData;
};