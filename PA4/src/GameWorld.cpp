#include "GameWorld.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = false;


    // TODO: Check if the realmShaper has access to explore the isle
    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess
    // Use // std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
    if (!realmShaper)
    {
        std::cout << "[Access Control] RealmShaper not found!" << std::endl;
        return false; 
    }

    int playerDepth = getShaperTree().getDepth(realmShaper);
    // std::cout << playerDepth << std::endl;

    int totalMapDepth = getMapTree().getDepth();
    // std::cout << totalMapDepth<< std::endl;


    int totalShaperTreeHeight = getShaperTree().getDepth();
    // std::cout << totalMapDepth << std::endl;


    int minMapDepthAccess = getMapTree().calculateMinMapDepthAccess(playerDepth, totalShaperTreeHeight, totalMapDepth);
    // std::cout << minMapDepthAccess << std::endl;


    int isleDepth = getMapTree().getIsleDepth(isle);
    // std::cout << isleDepth << std::endl;


    if (isleDepth >= minMapDepthAccess)
    {
        hasAccess = true;
    }


    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    // TODO:
    // Check if realmShaper has access
    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
    // If realmShaper has access
    // Visit isle, 
    // collect item, 
    // check overcrowding for Isle, 
    // delete Isle if necessary

    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation

    Isle* currentIsle = realmShaper->getCurrentIsle();
    if (currentIsle != nullptr) {
        // std::cout << currentIsle->getName() <<  std::endl;
        // std::cout << currentIsle->getShaperCount() <<  std::endl;
        if (currentIsle->getShaperCount() != 0){
            currentIsle->decreaseShaperCount();

        }   
        // std::cout << currentIsle->getShaperCount() <<  std::endl;

        
    }



    if (hasAccess(realmShaper, isle))
    {




        realmShaper->setCurrentIsle(isle);
        Item item = isle->getItem();
        realmShaper->collectItem(item);

        if (isle->increaseShaperCount() )
        {
            std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
            std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
            std::cout << "[Overcrowding] " << isle->getName() << "self-destructed, it will be removed from the map" << std::endl;
            mapTree.remove(isle); 
            //ShaperTree::getTree() bunla al herbirini dolas currentisle ayniysa remove edilenle onu nulla cevir
            for (auto& player : getShaperTree().getTree()) {
                if (player->getCurrentIsle() == isle) {
                    player->setCurrentIsle(nullptr); 
                }
            }
        
            
        }
        else
        {
            std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
            std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
        
            

        }
    }
    else
    {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore " << *isle << std::endl;
    }


}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    // TODO: Check energy and craft new isle if possible
    // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;

    if (shaper->hasEnoughEnergy())
    {
        Isle* newIsle = new Isle(isleName);  
        mapTree.insert(newIsle);             
        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;
        if (Map::rebalanceCount % 3 == 0 && Map::rebalanceCount != 0)        
        {
            mapTree.populateWithItems();

            mapTree.dropItemBFS();
            Map::rebalanceCount = 0;
        }
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
        shaper->loseEnergy();  

    }
    else
    {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
    }

}

void GameWorld::displayGameState()
{
    // TODO: Implement functions to read and parse Access and Duel logs

    mapTree.displayMap();
    shaperTree.displayTree();
}


void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    // TODO:
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other

    // This function should call exploreArea and craft functions

    // Use displayGameState();

    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);

    if (!accessFile.is_open() || !duelFile.is_open())
    {
        std::cerr << "[Error] Could not open log files!" << std::endl;
        return;
    }

    std::string skipLine;
    std::getline(accessFile, skipLine); 
    std::getline(duelFile, skipLine); 


    std::string line;
    int accessCount = 0; 

    while (std::getline(accessFile, line))
    {

        std::istringstream ss(line);

        std::string playerName, isleName;
        ss >> playerName >> isleName;
   




        RealmShaper* shaper = getShaperTree().findPlayer(playerName);
        Isle* isle = mapTree.findIsle(isleName);

        if (shaper)
        {
            if (isle)
            {
                exploreArea(shaper, isle);
            }
            else 
            {
                craft(shaper, isleName);
            }
        }
        else
        {
            std::cerr << "[Error] Player not found: " << playerName << std::endl;
        }

        accessCount++;

        if (accessCount % 5 == 0)
        {
            std::string duelLine;
            if (std::getline(duelFile, duelLine))
            {
                std::istringstream duelSS(duelLine);
                std::string challengerName;
                int result;
                duelSS >> challengerName >> result;

                RealmShaper* challenger = getShaperTree().findPlayer(challengerName);
                RealmShaper* opponent = getShaperTree().getParent(challenger);

                if (challenger && opponent)
                {
                    getShaperTree().duel(challenger,result);
                }
                else
                {
                    std::cerr << "[Error] Invalid duel participants: " << challengerName << std::endl;
                }
            }
        }

        displayGameState();
    }

    while (std::getline(duelFile, line))
    {
        std::istringstream ss(line);
        std::string challengerName;
        int result;
        ss >> challengerName >> result;

        RealmShaper* challenger = getShaperTree().findPlayer(challengerName);
        RealmShaper* opponent = getShaperTree().getParent(challenger);

        if (challenger && opponent)
        {
            getShaperTree().duel(challenger, result);
        }
        else
        {
            std::cerr << "[Error] Invalid duel participants: " << challengerName << std::endl;
        }

        displayGameState();
    }

    accessFile.close();
    duelFile.close();

}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}