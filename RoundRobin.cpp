// RoundRobin.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

#define MATCH_SIZE 3

bool sorting(std::pair<std::string, int> i, std::pair<std::string, int> j) { return (i.second < j.second); }

bool addPlayer(std::vector<std::pair<std::string, int>>& playersPrio, std::vector<std::string>& match, std::map<std::string, std::set<std::string>>& map);

int main()
{
    std::string players[] = { "Cedric G.", "Cedric K.", "Theo", "Marc", "Nicolas", "Maxime", "Alexis", "Joffrey", "Matthieu", "Ethem", "Francois", "Solene"};

    std::vector<std::pair<std::string, int>> playersPrio;

    std::map<std::string, std::set<std::string>> map;

    for (std::string player : players)
    {
        map.insert(std::pair<std::string, std::set<std::string>>(player, std::set<std::string>()));
        playersPrio.push_back(std::pair<std::string, int>(player, 0));
    }

    std::vector<std::string> match;

    for (int i = 0; i < playersPrio.size(); i++)
    {
        bool success = true;

        while (success == true)
        {
            success = addPlayer(playersPrio, match, map);

            if (success == true)
            {
                for (int cpt = 0; cpt < match.size(); cpt++)
                {
                    std::cout << match[cpt] << std::endl;
                }
                std::cout << std::endl;

                match.clear();
            }else{

            }
        }
    }

    for (int i = 0; i < playersPrio.size(); i++)
    {
        std::cout << playersPrio[i].first << " " << playersPrio[i].second << std::endl;
    }
}

bool addPlayer(std::vector<std::pair<std::string, int>>& playersPrio, std::vector<std::string>& match, std::map<std::string, std::set<std::string>>& map)
{
    for (int n = 0; n < playersPrio.size() && match.size() != MATCH_SIZE; n++)
    {
        bool invalid = false;

        for (int cpt = 0; cpt < match.size() && invalid == false; cpt++)
        {
            if (playersPrio[n].first == match[cpt])
            {
                invalid = true;
            }
            else {
                if (map[match[cpt]].find(playersPrio[n].first) != map[match[cpt]].end())
                {
                    invalid = true;
                }
            }
        }

        if (invalid == false)
        {
            for (int cpt = 0; cpt < match.size(); cpt++)
            {
                map[match[cpt]].insert(playersPrio[n].first);
                map[playersPrio[n].first].insert(match[cpt]);
            }

            match.push_back(playersPrio[n].first);
        }
    }

    if (match.size() == MATCH_SIZE)
    {
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

        return true;
    }
    else {
        return false;
    }
}

