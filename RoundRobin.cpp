// RoundRobin.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <Qfile>
#include <QString>

#define MATCH_SIZE 3
#define REMATCH 2

bool sorting(std::pair<std::string, int> i, std::pair<std::string, int> j) { return (i.second < j.second); }

int addPlayer(std::map<std::string, int>& playersPrio, std::vector<std::string>& match, std::map<std::string, std::map<std::string, int>>& map);

int placeMatch(std::map<std::string, int>& playersPrio, std::vector<std::pair<std::vector<std::string>, bool>>& matches, int playing);

int main(int argc, char* argv[])
{
    std::string players[] = { "Cedric G.", "Cedric K.", "Theo", "Marc", "Nicolas", "Maxime", "Alexis", "Joffrey", "Matthieu", "Ethem", "Francois" };

    std::map<std::string, int> playersPrio;

    std::map<std::string, std::map<std::string, int>> map;

    for (std::string player : players)
    {
        map.insert(std::pair<std::string, std::map<std::string, int>>(player, std::map<std::string, int>()));
        playersPrio.insert(std::pair<std::string, int>(player, 0));

        for (std::string player2 : players)
        {
            map[player].insert(std::pair<std::string, int>(player2, 0));
        }
    }

    std::vector<std::pair<std::vector<std::string>, bool>> matches;

    if (argc == 1)
    {
        int rematch = 0;

        while (rematch <= REMATCH)
        {
            matches.push_back(std::pair<std::vector<std::string>, bool>(std::vector<std::string>(), false));

            rematch = addPlayer(playersPrio, matches[matches.size() - 1].first, map);

            if (rematch > REMATCH)
            {
                for (int cpt = 0; cpt < matches[matches.size() - 1].first.size(); cpt++)
                {
                    playersPrio[matches[matches.size() - 1].first[cpt]]--;
                }

                matches.pop_back();
            }
            else {
                for (int cpt = 0; cpt < matches[matches.size()-1].first.size(); cpt++)
                {
                    std::cout << matches[matches.size() - 1].first[cpt] << std::endl;
                }

                std::cout << std::endl;
            }
        }

        for (auto it = playersPrio.begin(); it != playersPrio.end(); ++it)
        {
            std::cout << it->first << " " << it->second << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Total number of matches: " << matches.size() << std::endl;
    }
    else if (argc == 2)
    {
        QFile file(argv[1]);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            std::cout << "Failed to open file" << std::endl;
            return 1;
        }

        QStringList strList = QString(file.readAll()).split("\n");

        int line = 0;

        while(line < strList.size())
        {
            matches.push_back(std::pair<std::vector<std::string>, bool>(std::vector<std::string>(), false));
            matches[matches.size() - 1].first.push_back(strList[line].toStdString());
            matches[matches.size() - 1].first.push_back(strList[line+1].toStdString());
            matches[matches.size() - 1].first.push_back(strList[line+2].toStdString());
            line += 4;
        }

        int placed = 0;

        while (placed != matches.size())
        {
            for (auto it = playersPrio.begin(); it != playersPrio.end(); ++it)
            {
                it->second++;
            }

            int playing = 0;

            playing = placeMatch(playersPrio, matches, -1);

            if (playing == -1)
            {
                break;
            }
            else
            {
                for (int cpt = 0; cpt < matches[playing].first.size(); cpt++)
                {
                    std::cout << matches[playing].first[cpt] << std::endl;
                }

                std::cout << std::endl;
            }

            placed++;

            playing = placeMatch(playersPrio, matches, playing);

            if (playing == -1)
            {
                break;
            }
            else
            {
                for (int cpt = 0; cpt < matches[playing].first.size(); cpt++)
                {
                    std::cout << matches[playing].first[cpt] << std::endl;
                }

                std::cout << std::endl;
            }

            placed++;
        }

        if (placed != matches.size())
        {
            std::cout << "ERROR -----------------------------------------------------------------------------\n";

            for (int i = 0; i < matches.size(); i++)
            {
                if (matches[i].second == false)
                {
                    for (int cpt = 0; cpt < matches[i].first.size(); cpt++)
                    {
                        std::cout << matches[i].first[cpt] << std::endl;
                    }

                    std::cout << std::endl;
                }
            }
        }

        std::cout << "Total number of matches: " << matches.size() << std::endl;
    }
}

int placeMatch(std::map<std::string, int>& playersPrio, std::vector<std::pair<std::vector<std::string>, bool>>& matches, int playing)
{
    int bestMatch = -1;
    int bestCore = 0;

    for (int i = 0; i < matches.size(); i++)
    {
        int score = 0;
        bool valid = true;

        if (matches[i].second == false && i != playing)
        {
            for (int cpt = 0; cpt < matches[i].first.size() && valid == true; cpt++)
            {
                if (playing != -1)
                {
                    for (int n = 0; n < matches[playing].first.size(); n++)
                    {
                        if (matches[i].first[cpt] == matches[playing].first[n])
                        {
                            valid == false;
                            break;
                        }
                    }
                }

                score += playersPrio[matches[i].first[cpt]];
            }
        }

        if (valid == true && score > bestCore)
        {
            bestCore = score;
            bestMatch = i;
        }
    }

    if (bestMatch != -1)
    {
        matches[bestMatch].second = true;

        for (int i = 0; i < matches[bestMatch].first.size(); i++)
        {
            playersPrio[matches[bestMatch].first[i]] = 0;
        }
    }

    return bestMatch;
}

int addPlayer(std::map<std::string, int>& playersPrio, std::vector<std::string>& match, std::map<std::string, std::map<std::string, int>>& map)
{
    int rematch = 0;

    while (match.size() != MATCH_SIZE)
    {
        float bestScore = 9999;
        std::string bestPlayer = "";

        for (auto it = playersPrio.begin(); it != playersPrio.end(); ++it)
        {
            float score = 0;

            if (match.size() == 0)
            {
                score = 500 + it->second;
            }
            else
            {
                for (int cpt = 0; cpt < match.size(); cpt++)
                {
                    if (it->first != match[cpt])
                    {
                        score += map[match[cpt]][it->first];
                    }
                    else
                    {
                        score = 9999;
                    }
                }

                score = score / float(match.size());
            }

            if (bestScore > score)
            {
                bestScore = score;
                bestPlayer = it->first;
            }
        }


        for (int cpt = 0; cpt < match.size(); cpt++)
        {
            map[match[cpt]][bestPlayer]++;
            map[bestPlayer][match[cpt]]++;

            if (rematch < map[match[cpt]][bestPlayer])
            {
                rematch = map[match[cpt]][bestPlayer];
            }
        }

        match.push_back(bestPlayer);
    }

    for (int cpt = 0; cpt < match.size(); cpt++)
    {
        playersPrio[match[cpt]]++;
    }

    return rematch;
}

