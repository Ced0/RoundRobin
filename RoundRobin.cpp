// RoundRobin.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

#define MATCH_SIZE 3
#define REMATCH 2

bool sorting(std::pair<std::string, int> i, std::pair<std::string, int> j) { return (i.second < j.second); }

int addPlayer(std::vector<std::pair<std::string, int>>& playersPrio, std::vector<std::string>& match, std::map<std::string, std::map<std::string, int>>& map);

int main()
{
    std::string players[] = { "Cedric G.", "Cedric K.", "Theo", "Marc", "Nicolas", "Maxime", "Alexis", "Joffrey", "Matthieu", "Ethem", "Francois"};

    std::vector<std::pair<std::string, int>> playersPrio;

    std::map<std::string, std::map<std::string, int>> map;

    for (std::string player : players)
    {
        map.insert(std::pair<std::string, std::map<std::string, int>>(player, std::map<std::string, int>()));
        playersPrio.push_back(std::pair<std::string, int>(player, 0));

        for (std::string player2 : players)
        {
            map[player].insert(std::pair<std::string, int>(player2, 0));
        }
    }

    std::vector<std::string> match;

    int totalMatches = 0;

    int rematch = 0;

    while (rematch <= REMATCH)
    {
        rematch = addPlayer(playersPrio, match, map);

        if (rematch <= REMATCH)
        {
            for (int cpt = 0; cpt < match.size(); cpt++)
            {
                std::cout << match[cpt] << std::endl;
            }
            std::cout << std::endl;

            match.clear();

            totalMatches++;
        }else{
            //delete count of last match

            for (int cpt = 0; cpt < match.size(); cpt++)
            {
                for (int n = 0; n < playersPrio.size(); n++)
                {
                    if (playersPrio[n].first == match[cpt])
                    {
                        playersPrio[n].second--;
                    }
                }
            }
        }
    }

    for (int i = 0; i < playersPrio.size(); i++)
    {
        std::cout << playersPrio[i].first << " " << playersPrio[i].second << std::endl;
    }

    std::cout << "Total number of matches: " << totalMatches << std::endl;
}

int addPlayer(std::vector<std::pair<std::string, int>>& playersPrio, std::vector<std::string>& match, std::map<std::string, std::map<std::string, int>>& map)
{
    int rematch = 0;

    while (match.size() != MATCH_SIZE)
    {
        float bestScore = 9999;
        std::string bestPlayer = "";
        
        for (int n = 0; n < playersPrio.size(); n++)
        {
            float score = 0;

            if (match.size() == 0)
            {
                bestPlayer = playersPrio[0].first;
                break;
            }
            else
            {
                for (int cpt = 0; cpt < match.size(); cpt++)
                {
                    if (playersPrio[n].first != match[cpt])
                    {
                        score += map[match[cpt]][playersPrio[n].first];
                    }
                    else
                    {
                        score = 9999;
                    }
                }
            }

            score = score / float(match.size());

            if (bestScore > score)
            {
                bestScore = score;
                bestPlayer = playersPrio[n].first;
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
        for (int i = 0; i < playersPrio.size(); i++)
        {
            if (playersPrio[i].first == match[cpt])
            {
                playersPrio[i].second++;
                break;
            }
        }
    }

    std::sort(playersPrio.begin(), playersPrio.end(), sorting);

    return rematch;
}

