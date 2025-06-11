#include "HighScoreManager.h"
#include <iostream>
using json = nlohmann::json;

#include "DiggerConstants.h"

std::vector<HighScoreEntry> HighScoreManager::LoadHighScores(const std::string& fileName)
{
    std::vector<HighScoreEntry> scores(DIGGER::MAX_HIGHSCORE_ENTRIES);
    std::ifstream inFile(std::string{ "../Data/" + fileName });

    if (!inFile.is_open()) return scores;

    nlohmann::json j;
    inFile >> j;

    for (const auto& item : j)
    {
        if (item.contains("initials") && item.contains("score"))
        {
            HighScoreEntry entry;
            entry.initials = item["initials"];
            entry.score = item["score"];
            scores.push_back(entry);
        }
    }

    std::sort(scores.begin(), scores.end(), [](const HighScoreEntry& a, const HighScoreEntry& b) {
        return a.score > b.score;
        }
    );

    return scores;
}

void HighScoreManager::SaveHighScores(const std::string& fileName, const HighScoreEntry& newEntry)
{
    std::vector<HighScoreEntry> scores = LoadHighScores("../Data/" + fileName);
    scores.emplace_back(newEntry);

    std::sort(scores.begin(), scores.end(), [](const HighScoreEntry& a, const HighScoreEntry& b) {
        return a.score > b.score;
        }
    );

    if (scores.size() > DIGGER::MAX_HIGHSCORE_ENTRIES) scores.resize(DIGGER::MAX_HIGHSCORE_ENTRIES);

    nlohmann::json j;
    for (const auto& entry : scores)
    {
        j.push_back({
            {"initials", entry.initials},
            {"score", entry.score}
            }
        );
    }

    std::ofstream outFile("../Data/" + fileName);
    if (outFile.is_open())
    {
        outFile << j.dump(4);
    }
}
