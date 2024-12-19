#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <queue>


ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
    for (auto shaper : realmShapers)
    {
        delete shaper;
    }

}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert innitial shapers to the tree
    for (auto shaper : shapers)
    {
        insert(shaper);
    }
}

int ShaperTree::getSize()
{
    // TODO: Return number of shapers in the tree
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{

    // TODO: Check if the index is valin in the tree

    std::vector<RealmShaper*>::size_type unsignedIndex = static_cast<std::vector<RealmShaper*>::size_type>(index);
    return unsignedIndex < realmShapers.size();
}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else

    int index = findIndex(shaper);
    if (index == -1) 
        return -1;

    for (std::vector<RealmShaper*>::size_type i = index; i < realmShapers.size() - 1; ++i) {
        realmShapers[i] = realmShapers[i + 1];
    }

    realmShapers.pop_back();

    delete shaper; 

    return index;
}


int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else

    for (size_t i = 0; i < realmShapers.size(); ++i)
    {
        if (*realmShapers[i] == *shaper)
            return i;
    }

    return -1;
}
//tam2li agac icin yaptim
int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else
    int index = findIndex(shaper); 
    if (index == -1)             
        return -1;

    int depth = 0;
    while (index > 0) 
    {
        index = (index - 1) / 2; 
        depth++;                 
    }
    return depth;
}
//tam2li agac icin yaptim
int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree

    int size = realmShapers.size(); 
    if (size == 0)
        return 0; 

    // Logaritmik hesaplama
    int depth = std::log2(size); 
    return depth    ;

}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    // TODO: Implement duel logic, return the victor
    // Use   std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    // Use   std::cout << "[Honour] " << "New honour points: ";
    // Use   std::cout << challengerName << "-" << challengerHonour << " ";
    // Use   std::cout << opponentName << "-" << opponentHonour << std::endl;
    // Use   std::cout << "[Duel] " << loserName << " lost all honour, delete" << std::endl;

    RealmShaper *parent = getParent(challenger);

    if (!parent)
    {
        std::cout << "[Error] Challenger has no parent to duel." << std::endl;
        return *challenger;
    }

    std::string challengerName = challenger->getName();

    std::string parentName = parent->getName();

    if (result)
    {
        challenger->gainHonour();
        parent->loseHonour();

        int challengerHonour = challenger->getHonour();
        int parentHonour = parent->getHonour();

        replace(parent, challenger);  


        std::cout << "[Duel] " << challengerName << " won the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: " << challengerName << "-" << challengerHonour << " ";
        std::cout << parentName << "-" << parentHonour << std::endl;
    }
    else
    {
        challenger->loseHonour();
        parent->gainHonour();

        int challengerHonour = challenger->getHonour();
        int parentHonour = parent->getHonour();

        std::cout << "[Duel] " << challengerName << " lost the duel" << std::endl;
        std::cout << "[Honour] " << "New honour points: " << challengerName << "-" << challengerHonour << " ";
        std::cout << parentName << "-" << parentHonour << std::endl;
    }

    if (challenger->getHonour() <= 0)
    {
        std::cout << "[Duel] " << challengerName << " lost all honour, delete" << std::endl;
        remove(challenger);  
    }

    if (parent->getHonour() <= 0)
    {
        std::cout << "[Duel] " << parentName << " lost all honour, delete" << std::endl;
        remove(parent); 
    }

    return result ? *challenger : *parent;

}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{

    // TODO: return parent of the shaper
    int index = findIndex(shaper);
    if (index <= 0)
        return nullptr;

    int parentIndex = (index - 1) / 2;
    return isValidIndex(parentIndex) ? realmShapers[parentIndex] : nullptr;

}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    // TODO: Change player_low and player_high's positions on the tree
    int lowIndex = findIndex(player_low);
    int highIndex = findIndex(player_high);

    if (lowIndex != -1 && highIndex != -1)
        std::swap(realmShapers[lowIndex], realmShapers[highIndex]);
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (auto& currentShaper : realmShapers)
    {
        if (*currentShaper == shaper) 
        {
            foundShaper = currentShaper;
            break; 
        }
    }

    return foundShaper;

}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (auto& currentShaper : realmShapers)
    {
        if (currentShaper->getName() == name) 
        {
            foundShaper = currentShaper;
            break; 
        }
    }

    return foundShaper;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well

    if (!isValidIndex(index))
        return result;

    auto leftResult = inOrderTraversal(getLeftChildIndex(index));
    result.insert(result.end(), leftResult.begin(), leftResult.end());

    result.push_back(realmShapers[index]->getName());

    auto rightResult = inOrderTraversal(getRightChildIndex(index));
    result.insert(result.end(), rightResult.begin(), rightResult.end());

    return result;
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    if (!isValidIndex(index))
        return result;

    result.push_back(realmShapers[index]->getName());

    auto leftResult = preOrderTraversal(getLeftChildIndex(index));
    result.insert(result.end(), leftResult.begin(), leftResult.end());

    auto rightResult = preOrderTraversal(getRightChildIndex(index));
    result.insert(result.end(), rightResult.begin(), rightResult.end());

    return result;
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    if (!isValidIndex(index))
        return result;

    auto leftResult = postOrderTraversal(getLeftChildIndex(index));
    result.insert(result.end(), leftResult.begin(), leftResult.end());

    auto rightResult = postOrderTraversal(getRightChildIndex(index));
    result.insert(result.end(), rightResult.begin(), rightResult.end());

    result.push_back(realmShapers[index]->getName());

    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file

    // Define and implement as many helper functions as necessary for recursive implementation

    if (!isValidIndex(index)) {
        return;
    }

    outFile << *realmShapers[index] << std::endl; 

    int left = 2 * index + 1;  
    preOrderTraversal(left, outFile);

    int right = 2 * index + 2; 
    preOrderTraversal(right, outFile);


}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file

    // Define and implement as many helper functions as necessary

    if (realmShapers.empty())
        return;

    std::queue<int> nodeQueue;
    nodeQueue.push(0);

    while (!nodeQueue.empty())
    {
        int index = nodeQueue.front();
        nodeQueue.pop();

        if (isValidIndex(index))
        {
            outFile << realmShapers[index]->getName() << std::endl;

            int leftChild = getLeftChildIndex(index);
            int rightChild = getRightChildIndex(index);

            if (isValidIndex(leftChild))
                nodeQueue.push(leftChild);
            if (isValidIndex(rightChild))
                nodeQueue.push(rightChild);
        }
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
  // TODO: Calculate left index
 // TODO: Calculate right index
    int left = getLeftChildIndex(index);
    int right = getRightChildIndex(index);

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;

    std::ofstream outFile(filename);
    if (!outFile)
    {
        std::cerr << "[Error] Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    breadthFirstTraversal(outFile);

    outFile.close();
    std::cout << "[Output] Shapers have been written to " << filename << " according to rankings." << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;

    std::ofstream outFile(filename);
    if (!outFile)
    {
        std::cerr << "[Error] Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    auto preOrderResult = preOrderTraversal(0);
    for (const auto &name : preOrderResult)
    {
        outFile << name << std::endl;
    }

    outFile.close();
    std::cout << "[Output] Tree have been written to " << filename << " in pre-order." << std::endl;
}



int ShaperTree::getLeftChildIndex(int index)
{
    return 2 * index + 1;
}

int ShaperTree::getRightChildIndex(int index)
{
    return 2 * index + 2;
}

