#include "RealmShaper.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define NECESSARY_ENERGY 2000 // Necessary energy to craft an Isle

RealmShaper::RealmShaper(std::string name, int honour) : name(name)
{
    this->honour = honour;
}

const std::string &RealmShaper::getName() const
{
    return this->name;
}

int RealmShaper::getEnergyLevel()
{
    return this->collectedEnergyPoints;
}

int RealmShaper::getHonour()
{
    return this->honour;
}

void RealmShaper::gainHonour()
{
    this->honour += 150;
}

void RealmShaper::loseHonour()
{
    this->honour -= 150;
}

void RealmShaper::collectItem(Item item)
{
    this->collectedEnergyPoints += item;
}

void RealmShaper::loseEnergy()
{
    this->collectedEnergyPoints -= NECESSARY_ENERGY;
}

bool RealmShaper::hasEnoughEnergy()
{
    return this->collectedEnergyPoints >= NECESSARY_ENERGY;
}

std::vector<RealmShaper *> RealmShaper::readFromFile(const std::string &filename)
{
    std::vector<RealmShaper *> players;

    // TODO: Read RealmShapers from the file,
    // add them to vector
    // return the vector
    // Input format: playerName[tab]honourPoints   


    std::ifstream infile(filename);

    if (!infile.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return players;
    }

    std::string line;
    while (std::getline(infile, line))
    {
        if (line.empty() || line[0] == '#') 
            continue;

        std::istringstream iss(line);
        std::string name;
        int honour;

        if (iss >> name >> honour)
        {
            players.push_back(new RealmShaper(name, honour));
        }
    }

    infile.close();

    return players;
}

bool RealmShaper::operator==(const RealmShaper &other) const
{
    // TODO: Compare by name, return true if same
        if (!this || !&other)
    {
        return false; 
    }

    return this->name == other.name && this->honour == other.honour;
}

std::ostream &operator<<(std::ostream &os, const RealmShaper &p)
{
    // Outs the player name
    return (os << p.name); // << std::endl);
}


bool RealmShaper::operator>(const RealmShaper &other) const
{
    return this->honour > other.honour;
}


Isle* RealmShaper::getCurrentIsle() const {
    return currentIsle;
}

void RealmShaper::setCurrentIsle(Isle* isle) {
    currentIsle = isle;
}