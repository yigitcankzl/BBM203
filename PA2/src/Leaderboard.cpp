#include "Leaderboard.h"
#include <fstream>  
#include <iostream> 
#include <sstream>  
#include <iomanip> 

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    // TODO: Your code here
    
    ifstream infile(filename);

    unsigned long score;
    time_t lastPlayed;
    string playerName;

    while (infile >> score >> lastPlayed >> ws && getline(infile, playerName)) {
        auto *new_entry = new LeaderboardEntry(score, lastPlayed, playerName);
        insert(new_entry); 
    }

}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    // TODO: Your code here

    ofstream outfile(filename);
    
    LeaderboardEntry* current = head_leaderboard_entry;
    while (current) {
        outfile << current->score << " " << current->last_played << " " << current->player_name << endl;
        current = current->next;
    }
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    // TODO: Your code here

    cout << "Leaderboard\n-----------" << endl;

    LeaderboardEntry* current = head_leaderboard_entry;
    int rank = 1;
    while (current) {
        time_t time = current->last_played;
        struct tm* time_info = localtime(&time);
        stringstream formatted_time;
        formatted_time << put_time(time_info, "%H:%M:%S/%d.%m.%Y");
        
        cout << rank << ". " << current->player_name << " " << current->score << " " << formatted_time.str() << endl;
        
        current = current->next;
        rank++;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    // TODO: Your code here
if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        // Traverse the list to find the appropriate position
        LeaderboardEntry* current = head_leaderboard_entry;

        // Move to the point where the next entry has a lower score
        while (current->next && current->next->score >= new_entry->score) {
            current = current->next;
        }

        // Insert the new entry
        new_entry->next = current->next;
        current->next = new_entry;
    }

    // Enforce a maximum size for the leaderboard
    LeaderboardEntry* current = head_leaderboard_entry;
    int count = 1;
    while (current && count < MAX_LEADERBOARD_SIZE) {
        current = current->next;
        count++;
    }
    if (current && current->next) {
        LeaderboardEntry* to_delete = current->next;
        current->next = nullptr;
        while (to_delete) {
            LeaderboardEntry* temp = to_delete;
            to_delete = to_delete->next;
            delete temp;
        }
    }
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    // TODO: Your code here

    while (head_leaderboard_entry) {
        LeaderboardEntry* temp = head_leaderboard_entry;
        head_leaderboard_entry = head_leaderboard_entry->next;
        delete temp;
    }
}
