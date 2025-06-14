#include "Pathfinder.h"
#include "TerrainComponent.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <limits>
#include <algorithm>
#include "PhysicsManager.h"

std::vector<glm::ivec2> Pathfinder::GetNeighbors(const glm::ivec2& pos)
{
    return {
        {pos.x + 1, pos.y},
        {pos.x - 1, pos.y},
        {pos.x, pos.y + 1},
        {pos.x, pos.y - 1}
    };
}

float Pathfinder::Heuristic(const glm::ivec2& a, const glm::ivec2& b)
{
    return static_cast<float>(std::abs(a.x - b.x)) + static_cast<float>(std::abs(a.y - b.y));
}
std::vector<glm::vec2> Pathfinder::FindPath(TerrainComponent* terrain, const glm::vec2& startPixel, const glm::vec2& endPixel, const glm::vec2& tileSize)
{
    Rinigin::Physics& physics = Rinigin::Physics::GetInstance();

    const glm::ivec2 start = glm::ivec2(startPixel.x / tileSize.x, startPixel.y / tileSize.y);
    const glm::ivec2 end = glm::ivec2(endPixel.x / tileSize.x, endPixel.y / tileSize.y);

    auto hash = [](glm::ivec2 p, int w) { return p.y * w + p.x; };

    const glm::ivec2 tileOrigin = glm::ivec2(terrain->GetOrigin().x / tileSize.x, terrain->GetOrigin().y / tileSize.y);
    const int mapWidth = static_cast<int>(terrain->GetMapWidth() / tileSize.x);
    const int mapHeight = static_cast<int>(terrain->GetMapHeight() / tileSize.y);

    auto cmp = [](Node* a, Node* b) { return a->FCost() > b->FCost(); };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> openSet(cmp);

    std::unordered_map<int, Node*> allNodes;
    std::unordered_map<int, bool> closed;

    std::vector<std::unique_ptr<Node>> nodePool;
    nodePool.emplace_back(std::make_unique<Node>(Node{ start, 0.0f, Heuristic(start, end), nullptr }));

    Node* startNode = nodePool.back().get();
    openSet.push(startNode);
    allNodes[hash(start, mapWidth)] = startNode;

    while (!openSet.empty())
    {
        Node* current = openSet.top();
        openSet.pop();

        if (current->pos == end)
        {
            std::vector<glm::vec2> path;
            for (Node* node = current; node; node = node->parent) {
                glm::vec2 tileCenter = glm::vec2(
                    (node->pos.x + 0.5f) * tileSize.x,
                    (node->pos.y + 0.5f) * tileSize.y
                );
                path.emplace_back(tileCenter);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closed[hash(current->pos, mapWidth)] = true;

        for (const glm::ivec2& neighbor : GetNeighbors(current->pos))
        {
            if (neighbor.x < tileOrigin.x or neighbor.y < tileOrigin.y or neighbor.x >= mapWidth + tileOrigin.x or neighbor.y >= tileOrigin.y + mapHeight)
                continue;

            glm::vec3 bounds(tileSize.x, tileSize.y, 0);
            glm::vec3 worldPos(
                neighbor.x * tileSize.x,
                neighbor.y * tileSize.y,
                0
            );

            float solidRatio = physics.GetMaskCoverage(worldPos, bounds);
            if (solidRatio > 0.5f) continue;

            int neighborHash = hash(neighbor, mapWidth);
            if (closed[neighborHash]) continue;

            float tentativeG = current->gCost + 1.0f;
            if (!allNodes.count(neighborHash) or tentativeG < allNodes[neighborHash]->gCost)
            {
                nodePool.emplace_back(std::make_unique<Node>(Node{ neighbor, tentativeG, Heuristic(neighbor, end), current }));
                Node* neighborNode = nodePool.back().get();
                openSet.push(neighborNode);
                allNodes[neighborHash] = neighborNode;
            }
        }
    }

    return {}; // No path found
}
