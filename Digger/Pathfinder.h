#pragma once
#include <vector>
#include <glm/glm.hpp>

class TerrainComponent;

class Pathfinder final
{
public:
    std::vector<glm::vec2> FindPath(TerrainComponent* terrain, const glm::vec2& startPixel, const glm::vec2& endPixel, const glm::vec2& tileSize);
private:
    struct Node
    {
        glm::ivec2 pos;
        float gCost;
        float hCost;
        Node* parent;

        float FCost() const { return gCost + hCost; }
        bool operator>(const Node& other) const { return FCost() > other.FCost(); }
    };

    static float Heuristic(const glm::ivec2& a, const glm::ivec2& b);
    static std::vector<glm::ivec2> GetNeighbors(const glm::ivec2& pos);
};
