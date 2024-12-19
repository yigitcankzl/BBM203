    #include "RoutePlanner.h"
    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <algorithm> 

    // Array to help you out with name of the cities in order
    const std::string cities[81] = { 
        "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
        "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
        "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
        "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
        "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
        "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
        "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
        "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
    };

    // Constructor to initialize and load constraints
    RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
        : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

        // TO DO:
        // Load map data from file
        // Mark all provinces as unvisited initially

        // Load priority provinces
        // Load restricted provinces

        
        map.loadDistanceData(distance_data);
        
        for (int i = 0; i < MAX_SIZE; ++i) {
            map.visited[i] = false;
        }
        
        loadPriorityProvinces(priority_data);
        loadWeatherRestrictedProvinces(restricted_data);
    }

    // Load priority provinces from txt file to an array of indices
    void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
        // TODO: Your code here

        std::ifstream file(filename);
        std::string line;
        int provinceIndex;
        
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << filename << std::endl;
                return;
            }
            std::string provinceName;
            
            if (std::getline(iss, provinceName, '(') && iss >> provinceIndex) {
                provinceIndex = provinceIndex; 

                if (iss.peek() == ')') {
                    iss.ignore();

                }

                if (numPriorityProvinces < MAX_PRIORITY_PROVINCES) {
                    priorityProvinces[numPriorityProvinces++] = provinceIndex;

                }
            } 
            // if (iss.fail()) {
            //         std::cerr << "Failed to parse province index in line: " << line << std::endl;
            //         continue;
            //     }
        }
        file.close();

    }

    // Load weather-restricted provinces from txt file to an array of indices
    void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
        // TODO: Your code here

        std::ifstream file(filename);
        std::string line;
        int index = 0;

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string cityName;
            int cityIndex;

            if (std::getline(ss, cityName, '(') && ss >> cityIndex) {

                if (ss.peek() == ')') {
                    ss.ignore();
                }

                if (numWeatherRestrictedProvinces < MAX_WEATHER_RESTRICTED_PROVINCES) {
                    weatherRestrictedProvinces[numWeatherRestrictedProvinces++] = cityIndex;
                }
            }
            // if (ss.fail()) {
            //         std::cerr << "Failed to parse province index in line: " << line << std::endl;
            //         continue;
            //     }
        }
        file.close();
    }

    // Checks if a province is a priority province
    bool RoutePlanner::isPriorityProvince(int province) const {
        // TODO: Your code here

        for (int i = 0; i < numPriorityProvinces; ++i) {
            if (priorityProvinces[i] == province) {
                return true;
                }
        }
        return false;
    }

    // Checks if a province is weather-restricted
    bool RoutePlanner::isWeatherRestricted(int province) const {
        // TODO: Your code here

        for (int i = 0; i < numWeatherRestrictedProvinces; ++i) {
            if (weatherRestrictedProvinces[i] == province) {
                return true;
            }
        }
        return false;
    }






    // Begins the route exploration from the starting point
    void RoutePlanner::exploreRoute(int startingCity) {
        // TODO: Your code here

        exploreFromProvince(startingCity);

        while (!isExplorationComplete() || !(map.countVisitedProvinces() == MAX_SIZE - numWeatherRestrictedProvinces)) {
            
            if (allNeighborsVisited(route.back())) {

                    // if(route.size() == 49){
                    //     firstRoute = route;
                    // }

                    while(!queue.isEmpty()){
                        queue.dequeue();
                    }
                    // std::cout << "backtraking yapiliyor " << std::endl;                

                    // backtracking
                    // printRoute(); 
                    // queue.printQueue();  
                    // std::cout << stack.getSize() << std::endl;                

                    // exploreFromProvince(stack.peek());    
                    backtrack();
                    printRoute();

                    exploreFromProvince(route.back());

                    // if (stack.getSize() == 0){
                    //     for (int i = 0; i < 45; ++i) {
                    //         route.push_back(startingCity);
                    //     }
                        

                    // }
                    printRoute();
                                        std::cout << stack.getSize() << std::endl;                


                


            }else {
                int nextProvince = queue.dequeue(); 
                int lastProvince = route.back();

                
                if (map.isWithinRange(nextProvince,lastProvince,maxDistance)){

                    exploreFromProvince(nextProvince);
                }

                // if(nextProvince==-1){
                //     enqueueNeighbors(lastProvince);
                //     // queue.printQueue();


                // }
            }

        }
       
        // for (int i = 0; i < numPriorityProvinces; ++i) {
        //     map.markAsVisited(priorityProvinces[i] );
            
        // }     
        displayResults();

    }

    // Helper function to explore from a specific province
    void RoutePlanner::exploreFromProvince(int province) {
        // TODO: Your code here 

            
        if (isWeatherRestricted(province)) {
            std::cout << "Province " << cities[province] << " is weather-restricted. Skipping." << std::endl;
            return; 
        }

        if (!route.empty() && route.back() == province) {
            return; 
        }

        if (route.empty()) {
            route.push_back(province);

            map.markAsVisited(province);
            enqueueNeighbors(province);
            stack.push(province); 

        } else {
            int lastProvince = route.back();
            int distance = map.getDistance(province, lastProvince);

            if (map.isWithinRange(lastProvince,province,maxDistance)){
                totalDistanceCovered += distance;

                route.push_back(province);  
                
                map.markAsVisited(province);  // true luyor =dogru
                enqueueNeighbors(province);  //q ya ekliyor=dogru
                stack.push(province); 

            }

        }
    }

    void RoutePlanner::enqueueNeighbors(int province) {
        // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints

            for (int i = 0; i < MAX_SIZE; ++i) {
            if (map.isWithinRange(province, i, maxDistance) && !map.isVisited(i)) {
                    
                bool found = false;
                for (const auto& province : route) {
                    if (province == i) {
                        found = true;
                        break;
                    }
                }

                if (found) {
                    continue;
                }

                if (queue.contains(i)) {
                    continue;
                }

                if (map.isVisited(i)){
                    continue;
                }

                if (isPriorityProvince(i)) {
                    queue.enqueuePriority(i);

                } else {
                    queue.enqueue(i);

                }
            }
        }
    }

    void RoutePlanner::backtrack() {
        // If you reach a dead-end province 
        // TODO: Your code here


        if (stack.isEmpty()) {
            return;
        }

        int currentProvince = stack.pop();

        if (!route.empty() && route.back() == currentProvince) {

            route.pop_back();
        }
        // map.reMarkAsVisited(currentProvince);
        
        if (!stack.isEmpty()) {
            int previousProvince = stack.peek();
            int distance = map.getDistance(previousProvince, currentProvince);
            totalDistanceCovered -= distance;


            enqueueNeighbors(previousProvince);
            
            // queue.printQueue();
            // exploreFromProvince(previousProvince);
            // map.reMarkAsVisited(currentProvince);

        }
        
    }

    bool RoutePlanner::isExplorationComplete() const {
        // TODO: Your code here

        if (queue.isEmpty() && stack.isEmpty() ) {
            return true;
        }
        return false;
    }





    void RoutePlanner::displayResults() const {
        // TODO: Your code here
        // Display "Journey Completed!" message
        // Display the total number of provinces visited
        // Display the total distance covered
        // Display the route in the order visited
        // Priority Province Summary

        std::cout << "----------------------------\n";
        std::cout << "Journey Completed!\n";
        std::cout << "----------------------------\n";

        std::cout << "Total Number of Provinces Visited: " << route.size() << std::endl;
        std::cout << "Total Distance Covered: " << totalDistanceCovered << " km" << std::endl;

        std::cout << "Route Taken:\n";
        for (size_t i = 0; i < route.size(); ++i) {
            std::cout << cities[route[i]];
            if (i != route.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n";

        std::cout << "Priority Provinces Status:\n";
        int visitedPriorityCount = 0;
        for (int i = 0; i < numPriorityProvinces; ++i) {
            bool visited = false;
            for (const auto& province : route) {
                if (province == priorityProvinces[i]) {
                    visited = true;
                    break;
                }
            }

            std::cout << "- " << cities[priorityProvinces[i]] << " (" << (visited ? "Visited" : "Not Visited") << ")\n";            if (visited) {
                ++visitedPriorityCount;
            }
        }

        std::cout << "Total Priority Provinces Visited: " 
                << visitedPriorityCount << " out of " 
                << numPriorityProvinces << std::endl;

        if (visitedPriorityCount == numPriorityProvinces) {
            std::cout << "Success: All priority provinces were visited." << std::endl;
        } else {
            std::cout << "Warning: Not all priority provinces were visited." << std::endl;
        }
    }









    void RoutePlanner::printRoute() const {
        std::cout << "Current Route: ";
        for (int province : route) {
            std::cout << cities[province] << " -> ";
        }
        std::cout << "END" << std::endl;
    }


    bool RoutePlanner::allNeighborsVisited(int province) const {
        for (int i = 0; i < MAX_SIZE; ++i) {
            if (map.isWithinRange(province, i, maxDistance) && !map.isVisited(i) && !isWeatherRestricted(i)) {

                return false;  
            }
        }
        return true;  
    }