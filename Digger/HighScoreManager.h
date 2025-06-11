#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

struct HighScoreEntry
{
    std::string initials = "...";
    int score;
};

class HighScoreManager
{
public:
    static std::vector<HighScoreEntry> LoadHighScores(const std::string& fileName);
    static void SaveHighScores(const std::string& filePath,const HighScoreEntry& scores);
};
