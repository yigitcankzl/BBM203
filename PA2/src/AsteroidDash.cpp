#include "AsteroidDash.h"
#include <fstream>  
#include <iostream> 
#include <sstream>  
#include <algorithm>


// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                        const string &celestial_objects_file_name,
                        const string &leaderboard_file_name,
                        const string &player_file_name,
                        const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    // TODO: Your code here

    ifstream file(input_file);
    string line;
    
    while (getline(file, line)) {
        vector<int> row;
        for (char c : line) {
            if (c != ' ') {  
                row.push_back(c == '1' ? 1 : 0);
            }  
        }

        space_grid.push_back(row);
    }

    file.close();
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    // TODO: Your code here

    ifstream file(player_file_name);

    int row, col;
    file >> row >> col; 
    file.ignore(); 

    vector<vector<bool>> shape;
    string line;
    while (getline(file, line)) {
        vector<bool> row_shape;
        for (char c : line) {
            if (c == '1') {
                row_shape.push_back(true);
            } else if (c == '0') {
                row_shape.push_back(false);
            }

        }
        shape.push_back(row_shape);
    }
    
    player = new Player(shape, row, col, player_name);
    file.close();
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    // TODO: Your code here

    ifstream file(input_file);
    string line;
    vector<vector<bool>> shape;
    int start_row;
    int time_of_appearance;
    ObjectType object_type;
    string effect;


    while (getline(file, line)) {
        if (line.empty()) {
            continue;  
        }

        if (line[0] == '[' || line[0] == '{') {
            shape.clear();
            if (line[0]== '[') object_type = ASTEROID;
            
            while (!line.empty() && line[0] != 's') {
                vector<bool> row;
                for (char c : line) {
                    if (c == '1') row.push_back(true);
                    else if (c == '0') row.push_back(false);
                }
                shape.push_back(row);
                getline(file, line);
            }
        }

        if (line[0] == 's') {
            start_row = stoi(line.substr(2));
            getline(file, line);
        }
        
        if (line[0] == 't') {
            time_of_appearance = stoi(line.substr(2));
            getline(file, line);
        }

        if (line[0] == 'e') {
            
            effect = line.substr(2);

            if (effect == "life") object_type = LIFE_UP;
            else if (effect =="ammo") object_type = AMMO;
    
            getline(file, line);
        }

        CelestialObject *new_object = new CelestialObject(shape, object_type, start_row, time_of_appearance);


        if (!celestial_objects_list_head) {
            celestial_objects_list_head = new_object;
        } else {
            CelestialObject *current = celestial_objects_list_head;
            while (current->next_celestial_object) {
                current = current->next_celestial_object;
            }
            current->next_celestial_object = new_object;
        }


        CelestialObject* next_obj = new_object->next_celestial_object;
        CelestialObject *rotation_90 = create_rotation(new_object,rotate_right(new_object->shape),next_obj);
        CelestialObject *rotation_180 = create_rotation(new_object,rotate_right(rotation_90->shape),next_obj);
        CelestialObject *rotation_270 = create_rotation(new_object,rotate_right(rotation_180->shape),next_obj);

        if (are_shapes_equal(new_object->shape, rotation_90->shape)){
            new_object->right_rotation = new_object;
            new_object->left_rotation = new_object;


            delete rotation_90;
            delete rotation_180;
            delete rotation_270;

        } else if (are_shapes_equal(new_object->shape, rotation_180->shape)) {
            new_object->right_rotation = rotation_90;
            rotation_90->right_rotation = new_object;

            new_object->left_rotation = rotation_90;
            rotation_90->left_rotation = new_object;

            delete rotation_180;
            delete rotation_270;

        } else {
            new_object->right_rotation = rotation_90;
            rotation_90->right_rotation = rotation_180;
            rotation_180->right_rotation = rotation_270;
            rotation_270->right_rotation = new_object;

            new_object->left_rotation = rotation_270;
            rotation_270->left_rotation = rotation_180;
            rotation_180->left_rotation = rotation_90;
            rotation_90->left_rotation = new_object;

        }
            

        
    }       

        CelestialObject *current = celestial_objects_list_head;
        while (current) {
            CelestialObject *current_next = current->next_celestial_object;

            int rotation_count = 0; 
            while (rotation_count < 4) { 
                CelestialObject *current_left = current->left_rotation; 
                current_left->next_celestial_object = current_next; 
                current = current_left; 
                rotation_count++;
            }

            current = current->next_celestial_object;
        }



    file.close();


}

// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    // TODO: Your code here

    for (const auto &row : space_grid) {
        for (int cell : row) {
            if (cell == 1) cout << "██";
            else if (cell == 0) cout << "▒▒";
        }
        cout << endl;
    }
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    // TODO: Your code here
    increment_tick();

    clear_grid();  
    
    place_player_on_grid();

    collision_celestial_object(tick_count);

    collision_projectile(tick_count);

    
    place_shoot(active_projectiles);


    CelestialObject* current_object = celestial_objects_list_head;  
    while (current_object) {
        if (tick_count >= current_object->time_of_appearance ) { 
            place_celestial_object(current_object, tick_count);
        }
        current_object = current_object->next_celestial_object;
    }

    // print_space_grid();
    
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    // TODO: Your code here

    if (player->current_ammo > 0) {
        player->current_ammo--;  

        int center_row = player->spacecraft_shape.size() / 2;  
        int center_col = player->spacecraft_shape[0].size() / 2; 
        
        int projectile_row = player->position_row + center_row;  
        int projectile_col = player->position_col + center_col + 1;  

        Shoot new_projectile(projectile_row, projectile_col, tick_count);
        active_projectiles.push_back(Shoot(projectile_row, projectile_col, tick_count));
    }
}


// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    // TODO: Your code here

    CelestialObject *current = celestial_objects_list_head;
    while (current) {
        CelestialObject *next = current->next_celestial_object;
        delete current;  
        current = next;
    }

    delete player;
}


std::vector<std::vector<bool>> AsteroidDash::rotate_right(const std::vector<std::vector<bool>>& shape) {
    int rows = shape.size();
    int cols = shape[0].size();
    std::vector<std::vector<bool>> rotated(cols, std::vector<bool>(rows)); 

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            rotated[j][rows - 1 - i] = shape[i][j];  
        }
    }

    return rotated;
}


CelestialObject* AsteroidDash::create_rotation(const CelestialObject* original, const std::vector<std::vector<bool>>& new_shape, CelestialObject *next_obj ) {
    CelestialObject* rotation = new CelestialObject(*original); 
    rotation->shape = new_shape;  
    return rotation;
}


bool AsteroidDash::are_shapes_equal(const std::vector<std::vector<bool>>& shape1, const std::vector<std::vector<bool>>& shape2) {
    if (shape1.size() != shape2.size()) return false;
    for (size_t i = 0; i < shape1.size(); ++i) {
        if (shape1[i] != shape2[i]) return false;
    }
    return true;
}


void AsteroidDash::clear_grid() {
    for (auto& row : space_grid) {
        std::fill(row.begin(), row.end(), 0);
    }
}


void AsteroidDash::place_player_on_grid() {
    int start_row = player->position_row;
    int start_col = player->position_col;
    const vector<vector<bool>>& shape = player->spacecraft_shape;

    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            int grid_row = start_row + i;
            int grid_col = start_col + j;

            if (grid_row >= 0 && grid_row < space_grid.size() &&
                grid_col >= 0 && grid_col < space_grid[0].size()) {

                space_grid[grid_row][grid_col] = shape[i][j] ? 1 : 0;
            }
        }
    }
}


void AsteroidDash::place_shoot(vector<Shoot> &projectiles) {
    for (auto &projectile : projectiles) {
        int projectile_col = projectile.position_col + (tick_count - projectile.time_of_appearance);
        // std:: cout << tick_count << "tick";


        if (projectile.position_row >= 0 && projectile.position_row < space_grid.size() &&
            projectile_col >= 0 && projectile_col < space_grid[0].size()) {

            space_grid[projectile.position_row][projectile_col] = 1;
        }else {
        // Erase the projectile using an iterator
        active_projectiles.erase(std::remove_if(active_projectiles.begin(), active_projectiles.end(), 
            [&projectile](const Shoot& s) { return &s == &projectile; }), active_projectiles.end());
        }
    }
}


void AsteroidDash::place_celestial_object(CelestialObject* current_object, int tick_count) {
    int start_row = current_object->starting_row;
    int start_col = space_grid[0].size() - (tick_count - current_object->time_of_appearance);
    
    for (int i = 0; i < current_object->shape.size(); ++i) {
        for (int j = 0; j < current_object->shape[i].size(); ++j) {
            if (current_object->shape[i][j]) {
                int grid_row = start_row + i;
                int grid_col = start_col + j;

                if (grid_row >= 0 && grid_row < space_grid.size() &&
                    grid_col >= 0 && grid_col < space_grid[0].size()) {
                    
                    space_grid[grid_row][grid_col] = current_object->shape[i][j] ? 1 : 0;
                }
            }
        }
    }
}


void AsteroidDash::collision_celestial_object(int tick_count) {
    CelestialObject* current_object = celestial_objects_list_head;
    CelestialObject* prev_object = nullptr;  

    while (current_object) {
        if (check_collision_celectial_object(player, current_object, tick_count)) {
            if (current_object->object_type == ASTEROID) {
                player->lives--; 

                if (prev_object) {
                    prev_object->next_celestial_object = current_object->next_celestial_object;
                } else {
                    celestial_objects_list_head = current_object->next_celestial_object;
                }
                delete current_object;  
                break; 
            }
            else if (current_object->object_type == LIFE_UP) {
                player->lives++; 

                if (prev_object) {
                    prev_object->next_celestial_object = current_object->next_celestial_object;
                } else {
                    celestial_objects_list_head = current_object->next_celestial_object;
                }
                delete current_object;  
                break; 
            }
            else if (current_object->object_type == AMMO) {
                player->current_ammo = player->max_ammo; 

                if (prev_object) {
                    prev_object->next_celestial_object = current_object->next_celestial_object;
                } else {
                    celestial_objects_list_head = current_object->next_celestial_object;
                }
                delete current_object;  
                break; 
            }
        }

        prev_object = current_object;
        current_object = current_object->next_celestial_object;
    }
}


bool AsteroidDash::check_collision_celectial_object(Player* player, CelestialObject* object, int tick_count) {
    int object_col = space_grid[0].size() - (tick_count - object->time_of_appearance);
    
    for (int i = 0; i < player->spacecraft_shape.size(); ++i) {
        for (int j = 0; j < player->spacecraft_shape[i].size(); ++j) {
            int player_cell_row = player->position_row + i;
            int player_cell_col = player->position_col + j;

            if (player->spacecraft_shape[i][j] == 1) {
                for (int k = 0; k < object->shape.size(); ++k) {
                    for (int l = 0; l < object->shape[k].size(); ++l) {
                        if (object->shape[k][l] == 1) {
                            int object_cell_row = object->starting_row + k;
                            int object_cell_col = object_col + l;

                            if (player_cell_row == object_cell_row && player_cell_col == object_cell_col) {
                                return true;  
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}


void AsteroidDash::collision_projectile(int tick_count) {
    for (auto &projectile : active_projectiles) {
        CelestialObject* current_object = celestial_objects_list_head;

        while (current_object) {
            // Check if the object is valid and has appeared
            if (current_object == nullptr) {
                break; // Ensure we are not working with a null pointer
            }

            if (tick_count >= current_object->time_of_appearance) {

                if (check_collision_projectile(current_object, projectile, tick_count)) {
                    current_score += 10;

                    if (current_object->object_type == ASTEROID) {
                        int obj_col = space_grid[0].size() - (tick_count - current_object->time_of_appearance);

                        // Ensure the position is valid
                        if (projectile.position_row >= current_object->starting_row && 
                            projectile.position_row < current_object->starting_row + current_object->shape.size() &&
                            projectile.position_col >= obj_col && 
                            projectile.position_col < obj_col + current_object->shape[0].size()) {

                            current_object->shape[projectile.position_row - current_object->starting_row][projectile.position_col - obj_col] = 0;

                            int asteroid_height = current_object->shape.size();
                            int mid_section = asteroid_height / 2;

                            if (projectile.position_row < mid_section) {
                                current_object->shape = rotate_right(current_object->shape);
                            } else if (projectile.position_row > mid_section) {
                                current_object->shape = rotate_right(rotate_right(rotate_right(current_object->shape)));
                            }

                            current_object->delete_rotations(current_object);

                            CelestialObject *rotation_90 = create_rotation(current_object, rotate_right(current_object->shape), current_object->next_celestial_object);
                            CelestialObject *rotation_180 = create_rotation(current_object, rotate_right(rotation_90->shape), current_object->next_celestial_object);
                            CelestialObject *rotation_270 = create_rotation(current_object, rotate_right(rotation_180->shape), current_object->next_celestial_object);

                            // Rotation comparison logic
                            if (current_object->shape == rotation_90->shape) {
                                current_object->right_rotation = current_object;
                                current_object->left_rotation = current_object;

                                delete rotation_90;
                                delete rotation_180;
                                delete rotation_270;
                            } else if (current_object->shape == rotation_180->shape) {
                                current_object->right_rotation = rotation_90;
                                rotation_90->right_rotation = current_object;

                                current_object->left_rotation = rotation_90;
                                rotation_90->left_rotation = current_object;

                                delete rotation_180;
                                delete rotation_270;
                            } else {
                                current_object->right_rotation = rotation_90;
                                rotation_90->right_rotation = rotation_180;
                                rotation_180->right_rotation = rotation_270;
                                rotation_270->right_rotation = current_object;

                                current_object->left_rotation = rotation_270;
                                rotation_270->left_rotation = rotation_180;
                                rotation_180->left_rotation = rotation_90;
                                rotation_90->left_rotation = current_object;
                            }

                            handle_asteroid_removal(current_object); // Ensure it doesn't cause problems if the object is deleted
                        }
                    }

                    // Remove the projectile from the active list
                    if (!active_projectiles.empty() && active_projectiles.back() == projectile) {
                        active_projectiles.erase(std::remove_if(active_projectiles.begin(), active_projectiles.end(), 
                            [&projectile](const Shoot& s) { return &s == &projectile; }), active_projectiles.end());
                    }

                    break;
                }
            }

            // Move to the next object
            current_object = current_object->next_celestial_object;
        }
    }
}



bool AsteroidDash::check_collision_projectile(CelestialObject* object, Shoot &projectile, int tick_count) {
    if (object == nullptr) {
        return false; 
    }
    if (projectile.position_row < 0 || projectile.position_col < 0) {
        return false;
    }

    int object_col = space_grid[0].size() - (tick_count - object->time_of_appearance);
    int projectile_col = projectile.position_col + (tick_count - projectile.time_of_appearance);

    for (int i = 0; i < object->shape.size(); ++i) {
        for (int j = 0; j < object->shape[i].size(); ++j) {
            if (object->shape[i][j] == 1) {
                int object_row = object->starting_row + i;
                int object_col1 = object_col + j;

                if (projectile.position_row == object_row && projectile_col == object_col1) {
                    return true; 
                }
            }
        }
    }

    return false; 
}


void AsteroidDash::handle_asteroid_removal(CelestialObject* target) {
    if (target == nullptr) {
        return;
    }

    if (is_shape_empty(target->shape)) {
        current_score += 100; 
        if (celestial_objects_list_head == target) {
            celestial_objects_list_head = target->next_celestial_object;  
        } else {
            CelestialObject* current = celestial_objects_list_head;
            while (current != nullptr && current->next_celestial_object != target) {
                current = current->next_celestial_object;
            }

            if (current != nullptr) {
                current->next_celestial_object = target->next_celestial_object;
            }
        }
    }
    CelestialObject::delete_rotations(target); 

    delete target;
}

bool AsteroidDash::is_shape_empty(const vector<vector<bool>>& shape) {
    for (const auto& row : shape) {
        for (bool element : row) {
            if (element != 0) {  
                return false;
            }
        }
    }
    return true; 
}


