#include "Map.h"
#include <algorithm>

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // TODO: Free any dynamically allocated memory if necessary

    clearTree(root);
}

int Map::rebalanceCount = 0;

void Map::clearTree(MapNode* node)
{
    if (!node) return;

    clearTree(node->left);
    clearTree(node->right);

    if (node->isle)
    {
        delete node->isle;
        node->isle = nullptr; 
    }

    if (node != nullptr)
    {
        delete node;  
    }
}


void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert innitial isles to the tree
    // Then populate with Goldium and Einstainium items

    for (Isle* isle : isles)
    {

        insert(isle); 
        if (rebalanceCount != 0)
        {
            rebalanceCount--;
        }

    }
    populateWithItems();

}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;

    if (!current || !current->left) 
    {
        std::cerr << "[Right Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    root = current->left; 
    current->left = root->right;   
    root->right = current;        

    current->height = std::max(height(current->left), height(current->right)) + 1;
    root->height = std::max(height(root->left), height(root->right)) + 1;
    return root; 
}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;

    if (!current || !current->right)
    {
        std::cerr << "[Left Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    root = current->right; 
    current->right = root->left;    
    root->left = current;           

    current->height = std::max(height(current->left), height(current->right)) + 1;
    root->height = std::max(height(root->left), height(root->right)) + 1;
    return root;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node
    if (!node)
        return 0;

    int leftHeight = height(node->left);
    int rightHeight = height(node->right);

    return std::max(leftHeight, rightHeight) + 1;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{

    // TODO: Recursively insert isle to the tree
    // returns inserted node
    if (node == nullptr)
        return new MapNode(isle);
        
    if (*isle < *node->isle)  
        node->left = insert(node->left, isle);
    else if (*isle > *node->isle) 
        node->right = insert(node->right, isle);
    else 
        return node;

    node->height = std::max(height(node->left), height(node->right)) + 1;

    int balanceFactor = height(node->left) - height(node->right);

    if(balanceFactor>1 || balanceFactor< -1)
    {
        rebalanceCount++;

    }


    if (balanceFactor > 1 && *isle < *node->left->isle)

        return rotateRight(node);
    if (balanceFactor < -1 && *isle > *node->right->isle) 

        return rotateLeft(node);
    if (balanceFactor > 1 && *isle > *node->left->isle) 
    {
        node->left = rotateLeft(node->left);
        
        return rotateRight(node);
    }
    if (balanceFactor < -1 && *isle < *node->right->isle) 
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;

}

void Map::insert(Isle *isle)
{
    root = insert((root), isle);

    // you might need to insert some checks / functions here depending on your implementation
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    // TODO: Recursively delete isle from the tree
    // Will be called if there is overcrowding
    // returns node
    // Use std::cout << "[Remove] " << "Tree is Empty" << std::endl;
    
    if (!node) 
    {
        std::cout << "[Remove] Tree is Empty" << std::endl;
        return nullptr;
    }
    if (node && *isle < *node->isle)
        node->left = remove(node->left, isle); 
    else if (node && *isle > *node->isle)
        node->right = remove(node->right, isle); 
    else
    {

        if (!node) return nullptr;


        if (!node->left || !node->right) 
        {
            MapNode* temp = node->left ? node->left : node->right;
            if (node && node->isle) 
            {
                delete node->isle;
                node->isle = nullptr;
            }


            delete node; 
            return temp; 
        }
        else
        {
            MapNode* temp = minNode(node->right); 
            delete node->isle; 
            node->isle = temp->isle;
            node->right = remove(node->right, temp->isle);
        }
    }

    node->height = std::max(height(node->left), height(node->right)) + 1;

    int leftHeight = node->left ? node->left->height : 0;
    int rightHeight = node->right ? node->right->height : 0;
    int balanceFactor = leftHeight - rightHeight;

    if(balanceFactor>1 || balanceFactor< -1)
    {
        rebalanceCount++;

    }

    if (balanceFactor > 1 && height(node->left->left) >= height(node->left->right)) 
        return rotateRight(node);
    if (balanceFactor < -1 && height(node->right->right) >= height(node->right->left)) 
        return rotateLeft(node);
    if (balanceFactor > 1 && height(node->left->left) < height(node->left->right)) 
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balanceFactor < -1 && height(node->right->right) < height(node->right->left)) 
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;

}

void Map::remove(Isle *isle)
{
    root = remove((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;

    if (!current) return;

    count++;
    if (count % 5 == 0)
    {
        current->isle->setItem(Item::EINSTEINIUM);
        std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }

    preOrderItemDrop(current->left, count);

    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (!current) return;

    postOrderItemDrop(current->left, count);

    postOrderItemDrop(current->right, count);

    count++;
    if (count % 3 == 0)
    {
        current->isle->setItem(Item::GOLDIUM);
        std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }

}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // TODO: Find first Isle with no item

    if (node == nullptr)
        return nullptr;

    if (node->isle->getItem() == EMPTY)
        return node;

    MapNode *left = findFirstEmptyIsle(node->left);
    if (left != nullptr)
        return left;

    return findFirstEmptyIsle(node->right);
}

void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;

    if (rebalanceCount % 3 != 0) 
    {
        return; 
    }

    std::queue<MapNode*> q;
    q.push(root);

    while (!q.empty())
    {
        MapNode* current = q.front();
        q.pop();

        if (!current->isle->getItem())
        {
            current->isle->setItem(Item::AMAZONITE);
            std::cout << "[BFS Drop] AMAZONITE dropped on Isle: " << current->isle->getName() << std::endl;
            return;
        }

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    // TODO: Return node depth if found, else
    if (!node)
        return -1; 

    int leftDepth = getDepth(node->left);
    int rightDepth = getDepth(node->right);

    return std::max(leftDepth, rightDepth) + 1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else
    return getIsleDepth(root, isle, 0);
}

int Map::getIsleDepth(MapNode* node, Isle* isle, int depth)
{
    if (!node)
        return -1;

    if (*node->isle == *isle)
        return depth;

    int leftDepth = getIsleDepth(node->left, isle, depth + 1);
    if (leftDepth != -1)
        return leftDepth;

    return getIsleDepth(node->right, isle, depth + 1);
}

int Map::getDepth()
{
    // TODO: Return max|total depth of tree
    return getDepth(root);
    
}

void Map::populateWithItems()
{
    // TODO: Distribute fist GOLDIUM than EINSTEINIUM

    int count = 0;
    postOrderItemDrop(root, count); 
    count = 0;
    preOrderItemDrop(root, count); 



}

Isle *Map::findIsle(Isle isle)
{
    // TODO: Find isle by value
    return findIsle(root, isle);
}

Isle* Map::findIsle(MapNode* node, Isle isle)
{
    if (!node)
        return nullptr;  

  
    if (*node->isle == isle)
        return node->isle;

    if (*node->isle > isle)
        return findIsle(node->left, isle);  
    else
        return findIsle(node->right, isle); 
}


Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name
    return findIsle(root, name);
}


Isle* Map::findIsle(MapNode* node, std::string name)
{
    if (!node)
        return nullptr;  

    if (node->isle->getName() == name)
        return node->isle;

    if (name < node->isle->getName())
        return findIsle(node->left, name);  
    else
        return findIsle(node->right, name); 
}


MapNode *Map::findNode(Isle isle)
{
    // TODO: Find node by value
    return findNode(root, isle);
}


MapNode* Map::findNode(MapNode* node, Isle isle)
{
    if (!node)
        return nullptr;  

    
    if (*node->isle == isle)
        return node;

    if (*node->isle > isle)
        return findNode(node->left, isle);  
    else
        return findNode(node->right, isle);
}


MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    return findNode(root, name);
}


MapNode* Map::findNode(MapNode* node, std::string name)
{
    if (!node)
        return nullptr;  

    if (node->isle->getName() == name)
        return node;

    if (name < node->isle->getName())
        return findNode(node->left, name);  
    else
        return findNode(node->right, name); 
}


void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output level by level

    std::ofstream outfile(filename);

    if (!outfile)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::queue<MapNode*> q;
    q.push(root);

    while (!q.empty())
    {
        int levelSize = q.size();

        while (levelSize--)
        {
            MapNode* current = q.front();
            q.pop();
            outfile << current->isle->getName() << " "; 

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }

        outfile << std::endl; 
    }

    outfile.close();
}

void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;

std::ofstream outfile(filename);

    if (!outfile)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::vector<Isle*> isles;
    inOrderTraversal(root, isles);

    std::sort(isles.begin(), isles.end(), [](Isle* a, Isle* b) {
        return a->getName() < b->getName(); 
    });

    for (Isle* isle : isles)
    {
        outfile << isle->getName() << std::endl;
    }

    outfile.close();

    std::cout << "[Output] " << "Isles have been written to " << filename << " in alphabetical order." << std::endl;
}






MapNode* Map::minNode(MapNode* node)
{
    if (!node) 
        return nullptr; 

    MapNode* current = node;

    while (current && current->left != nullptr)
    {
        current = current->left;
    }

    return current; 
}




void Map::inOrderTraversal(MapNode* node, std::vector<Isle*>& isles)

{
    if (!node)
        return;

    inOrderTraversal(node->left, isles);

    isles.push_back(node->isle);

    inOrderTraversal(node->right, isles);
}


