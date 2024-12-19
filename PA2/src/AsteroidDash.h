#ifndef ASTEROIDDASH_H
#define ASTEROIDDASH_H

#include <vector>
#include <string>
#include <iostream>

#include "CelestialObject.h"
#include "LeaderboardEntry.h"
#include "Leaderboard.h"
#include "Player.h"

#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"

using namespace std;


class Shoot {
public:
    int position_row;
    int position_col;
    int time_of_appearance;

    Shoot(int row, int col, int tick_count) 
        : position_row(row), position_col(col), time_of_appearance(tick_count) {}


    bool operator==(const Shoot& other) const {
    // Compare relevant attributes of Shoot
    return this->position_row == other.position_row &&
           this->position_col == other.position_col;
    // Add any other attributes as necessary
}

};

class AsteroidDash {
    private:
    int tick_count; 

public:

    // Constructor to initialize the game
    AsteroidDash(const string &space_grid_file_name, const string &celestial_objects_file_name,
                 const string &leaderboard_file_name, const string &player_file_name, const string &player_name);

    // Destructor. Remove dynamically allocated member variables here
    virtual ~AsteroidDash();

    // 2D space_grid
    vector<vector<int> > space_grid;

    // Pointer to track the player instance
    Player *player = nullptr;

    // A reference to the head of the celestial objects linked list
    CelestialObject *celestial_objects_list_head = nullptr;

    // Current score of the game
    unsigned long current_score = 0;

    // Current game tick
    unsigned long game_time = 0;

    // Leaderboard file name to store and read the leaderboard from
    string leaderboard_file_name;

    // Leaderboard member variable
    Leaderboard leaderboard;

    // True if the game is over
    bool game_over = false;

    // Function to print the space_grid
    void print_space_grid() const;

    // Function to read the space grid from a file
    void read_space_grid(const string &input_file);

    // Function to read the player from a file
    void read_player(const string &player_file_name, const string &player_name);

    // Function to read celestial objects from a file
    // Reads the input file and calls the read_celestial_object() function for each celestial_object;
    void read_celestial_objects(const string &input_file);

    // Updates the grid based on player and celestial object states
    void update_space_grid();

    // Corresponds to the SHOOT command.
    void shoot();

    CelestialObject* create_rotation(const CelestialObject* original, const vector<vector<bool>>& new_shape,CelestialObject* next_obj );

    vector<vector<bool>> rotate_right(const vector<vector<bool>>& shape);

    bool are_shapes_equal(const vector<vector<bool>> &shape1, const vector<vector<bool>> &shape2);

    void clear_grid();

    void place_player_on_grid();

    void place_celestial_object(CelestialObject* current_object, int tick_count);

    void place_shoot(vector<Shoot> &projectile);

    void collision_celestial_object(int tick_count);

    bool check_collision_celectial_object(Player* player, CelestialObject* object, int tick_count); 

    std::vector<Shoot> active_projectiles;

    void collision_projectile(int tick_count);
    
    bool check_collision_projectile(CelestialObject* object, Shoot &projectile, int tick_count);

    void increment_tick() { tick_count++; }  
    
    int get_tick_count() const { return tick_count; }

    void handle_asteroid_removal(CelestialObject* target);

    bool is_shape_empty(const vector<vector<bool>>& shape);



    

};


#endif // ASTEROIDDASH_H
