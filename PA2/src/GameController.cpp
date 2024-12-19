#include "GameController.h"
#include <fstream>  
#include <iostream> 
#include <sstream>  

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    // TODO: Your code here, if you want to perform extra initializations
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    // TODO: Your code here

    ifstream file(commands_file);
    string command;
    
    while (getline(file, command)) {
        int current_tick = game->get_tick_count();
        
        if (command == "SHOOT") {
            game->shoot(); 
        } else if (command == "MOVE_LEFT") {
            game->player->move_left();
        } else if (command == "MOVE_RIGHT") {
            game->player->move_right(game->space_grid[0].size()); 
        } else if (command == "MOVE_UP") {
            game->player->move_up();
        } else if (command == "MOVE_DOWN") {
            game->player->move_down(game->space_grid.size());
        } else if (command == "PRINT_GRID") {
            cout << "Tick: " << current_tick << endl;                               
            cout << "Lives: " << game->player->lives << endl;
            cout << "Ammo: " << game->player->current_ammo  << endl;
            cout << "Score: " << game->current_score<< endl;
            cout << "High Score: " << endl;
            game->print_space_grid();
        }
        
        game->update_space_grid();
        game->print_space_grid();


        if (game->player->lives <= 0) {
            cout << "GAME OVER!" << endl;
            break;
        }
    }


    if (file.eof()) {
        cout << "GAME FINISHED ! No more commands!" << endl;
    } 

    // Check if the score is a new high score

}


// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    // TODO: Your code here
}
