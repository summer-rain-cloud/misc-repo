//
//  main.cpp
//  hockey statistic compiler
//
//  Created by Yu Xia on 1/24/23.
//

#include <iostream> // input and output for file
#include <fstream>  // file access
#include <array>    // static and should be much more compact
#include <vector>   // uncertain player size means the need for dynamic memory locations
#include <string>   // requires some string functions
#include <iomanip>  // use setprecision, left, right, setw, etc.
#include <algorithm>// may need to use sort functions

// classes
#include "institution.hpp"  // keeps information of the team/institution the players come from
#include "roster.hpp"       // keeps information of the players in the roster

//initalize function:: will be defined after main
bool isCommand(std::string command);
bool isThere(std::string name, std::string team, std::vector<roster>& players);
bool isThere(std::string name, std::vector<institution>& teams);
void benched_penalty(std::string name, std::vector<institution>& teams);
void player_penalty(std::string name, std::vector<roster>& players);
void player_goal(std::string name, std::vector<roster>& players);
void player_assist(std::string name, std::vector<roster>& players);
void winner(std::string name, std::vector<institution>& teams);
void loser(std::string name, std::vector<institution>& teams);
void tie_game(std::string name1, std::string name2, std::vector<institution>& teams);
void parse_file(std::fstream& input_f, std::vector<roster>& all_players, std::vector<institution>& teams);
void add_stats(roster& player, institution& team);
void compile_stats(std::vector<roster>& players, std::vector<institution>& teams);
void output_team(std::vector<institution>& teams, std::ofstream& file);
void output_player(std::vector<roster>& players, std::ofstream& file);
void output_custom(std::vector<institution>& teams, std::ofstream& file);
bool team_sort(institution& a, institution& b);
bool player_sort(roster& a, roster& b);

// start::
// command line format:: example.out text.txt text_out.txt --command [4 total]
// --team_stats  --player_stats  --custom_stats
int main(int argc, const char * argv[]) {
    // insert code here...
    std::fstream file_in;
    std::ofstream file_out;
    
    // basic error checking for command line arguments
    if(argc > 4){std::cerr << "ERROR! :: Extra command argument detected! \n"; exit(0);}
    file_in.open(argv[1]); file_out.open(argv[2]);
    if(!(file_in.good() && file_out.good())){
        std::cerr << "ERROR! :: Files does not exist or inaccessible from current directory! \n"; exit(0);
    }
    if(!isCommand(argv[3])){std::cerr << "ERROR! :: Command not found! \n"; exit(0);}
    
    // start of the actual program
    std::vector<roster> all_roster;
    std::vector<institution> all_teams;
    parse_file(file_in, all_roster, all_teams);
    compile_stats(all_roster, all_teams);
    std::sort(all_teams.begin(), all_teams.end(), team_sort);
    std::sort(all_roster.begin(), all_roster.end(), player_sort);
    std::string compare_str = argv[3];
    if(compare_str == "--team_stats"){output_team(all_teams, file_out);}
    if(compare_str == "--player_stats"){output_player(all_roster, file_out);}
    if(compare_str == "--custom_stats"){output_custom(all_teams, file_out);}
    
    file_in.close(); file_out.close();
    return 0;
}
// function definitions
bool isCommand(std::string command){
    if(command == "--team_stats"){return true;}
    if(command == "--player_stats"){return true;}
    if(command == "--custom_stats"){return true;}
    return false;
}
bool isThere(std::string name, std::string team, std::vector<roster>& players){
    if(name == ")"){return true;} //another corner case due to how i wrote my assist checker
    for(int i = 0; i < int(players.size()); i++){
        if(players[i].getName() == name){
            if(players[i].getTeam() != team){
                return false;
            }
            return true;
        }
    }
    return false;
}
bool isThere(std::string name, std::vector<institution>& teams){
    for(int i = 0; i < int(teams.size()); i++){
        if(name == teams[i].getName()){
            return true;
        }
    }
    return false;
}
void benched_penalty(std::string name, std::vector<institution>& teams){
    for(int i = 0; i < int(teams.size()); i++){
        if(teams[i].getName() == name){
            teams[i].addPenalty(1); // team penalty altogether has argument to avoid increment
            teams[i].addBenchedPenalty(1);
            break;
        }
    }
}
void player_penalty(std::string name, std::vector<roster>& players){
    for(int i = 0; i < int(players.size()); i++){
        if(players[i].getName() == name){
            players[i].penaltyplus(); // increments the player penalty count
            break;
        }
    }
}
void player_goal(std::string name, std::vector<roster>& players){
    for(int i = 0; i < int(players.size()); i++){
        if(players[i].getName() == name){
            players[i].goalplus();
            break;
        }
    }
}
void player_assist(std::string name, std::vector<roster>& players){
    for(int i = 0; i < int(players.size()); i++){
        if(players[i].getName() == name){
            players[i].assistsplus();
            break;
        }
    }
}
void winner(std::string name, std::vector<institution>& teams){
    for(int i = 0; i < int(teams.size()); i++){
        if(teams[i].getName() == name){
            teams[i].addWins();
            break;
        }
    }
}
void loser(std::string name, std::vector<institution>& teams){
    for(int i = 0; i < int(teams.size()); i++){
        if(teams[i].getName() == name){
            teams[i].addLoss();
            break;
        }
    }
}
void tie_game(std::string name1, std::string name2, std::vector<institution>& teams){
    for(int i = 0; i < int(teams.size()); i++){
        if(teams[i].getName() == name1){
            teams[i].addTies();
            break;
        }
    }
    for(int j = 0; j < int(teams.size()); j++){
        if(teams[j].getName() == name2){
            teams[j].addTies();
            break;
        }
    }
}
void parse_file(std::fstream& input_f, std::vector<roster>& all_players, std::vector<institution>& teams){
    std::string stow;
    std::string line_stow[8];
    while(input_f >> stow){
        if(stow.find(",") != std::string::npos){ // check for date
            input_f >> stow >> stow >> stow; // force through inputs
            input_f >> line_stow[1] >> line_stow[2] >> line_stow[3]; // save inputs
            // check for team names and constructs them if they do not exist
            if((line_stow[2] == "at") || (line_stow[2] == "vs.")){
                if(!isThere(line_stow[1], teams)){
                    institution team1(line_stow[1]);
                    teams.push_back(team1);
                }
                if(!isThere(line_stow[3], teams)){
                    institution team2(line_stow[3]);
                    teams.push_back(team2);
                }
            }
            // sub if statement break
        }
        // arc if statement break
        if(isThere(stow, teams)){ // check if it is a team name then parse the rest
            //assuming the team name has already been covered by the previous check then this
            //should run smoothly without missing crucial info necessary for class construction
            //stow contains the team, game_call has the call
            std::string game_call;
            input_f >> game_call;
            //check what the call was:: goal or penalty
            if(game_call == "penalty"){
                input_f >> line_stow[1]; //it either a name or BENCH
                if(line_stow[1] == "BENCH"){
                    benched_penalty(stow, teams);
                }
                else if(!(isThere(line_stow[1], stow, all_players))){
                    roster player1(line_stow[1], stow);
                    player1.penaltyplus();
                    all_players.push_back(player1);
                }
                else{
                    player_penalty(line_stow[1], all_players);
                }
            }
            if(game_call == "goal"){
                //stow contains the team name, line_stow[1] contains the player name
                input_f >> line_stow[1];
                if(!(isThere(line_stow[1], stow, all_players))){
                    roster player2(line_stow[1], stow);
                    player2.goalplus();
                    all_players.push_back(player2);
                    input_f >> line_stow[2]; // puts the next input after the scorer
                    if(line_stow[2] == "("){
                        input_f >> line_stow[3] >> line_stow[4]; // checks the next two inputs
                        //this if statement checks a corner case regarding empty assists
                        if(line_stow[3] == ")"){
                            if(line_stow[4] == "FINAL"){
                                stow = line_stow[4];
                            }
                        }
                        else{
                            if(!(isThere(line_stow[3], stow, all_players))){
                                roster player3(line_stow[3], stow);
                                all_players.push_back(player3);
                            }
                            if(!(isThere(line_stow[4], stow, all_players))){
                                roster player4(line_stow[4], stow);
                                all_players.push_back(player4);
                            }
                            if(line_stow[3] != ")"){
                                player_assist(line_stow[3], all_players);
                            }
                            if(line_stow[4] != ")"){
                                player_assist(line_stow[4], all_players);
                            }
                        }
                    }
                }
                else{
                    player_goal(line_stow[1], all_players);
                    input_f >> line_stow[2]; // puts the next input after the scorer
                    if(line_stow[2] == "("){
                        input_f >> line_stow[3] >> line_stow[4]; // checks the next two inputs
                        //this if statement checks a corner case regarding empty assists
                        if(line_stow[3] == ")"){
                            if(line_stow[4] == "FINAL"){
                                stow = line_stow[4];
                            }
                        }
                        else{
                            if(!(isThere(line_stow[3], stow, all_players))){
                                roster player3(line_stow[3], stow);
                                all_players.push_back(player3);
                            }
                            if(!(isThere(line_stow[4], stow, all_players))){
                                roster player4(line_stow[4], stow);
                                all_players.push_back(player4);
                            }
                            if(line_stow[3] != ")"){
                                player_assist(line_stow[3], all_players);
                            }
                            if(line_stow[4] != ")"){
                                player_assist(line_stow[4], all_players);
                            }
                        }
                    }
                }
            }
        }
        // arc if statement break
        if(stow == "FINAL"){ // check for final results and parse the rest
            //[1] team name [2] score [3] team name [4] score
            input_f >> line_stow[1] >> line_stow[2] >> line_stow[3] >> line_stow[4];
            if(std::stoi(line_stow[2]) == std::stoi(line_stow[4])){
                tie_game(line_stow[1], line_stow[3], teams);
            }
            if(std::stoi(line_stow[2]) > std::stoi(line_stow[4])){
                winner(line_stow[1], teams);
                loser(line_stow[3], teams);
            }
            if(std::stoi(line_stow[4]) > std::stoi(line_stow[2])){
                winner(line_stow[3], teams);
                loser(line_stow[1], teams);
            }
        }
        // arc if statement break
    }
    // while loop break
    // end of parse
}
void add_stats(roster& name, institution& team){
    team.add_t_goals(name.getGoals());
    team.add_t_assists(name.getAssists());
    team.addPenalty(name.getPenalties());
}
void compile_stats(std::vector<roster>& players, std::vector<institution>& teams){
    for(int i = 0; i < int(players.size()); i++){
        std::string team_name = players[i].getTeam();
        for(int j = 0; j < int(teams.size()); j++){
            if(teams[j].getName() == team_name){
                add_stats(players[i], teams[j]);
            }
        }
    }
}
void output_team(std::vector<institution>& teams, std::ofstream& file){
    int width = 0;
    for(int i = 0; i < int(teams.size()); i++){
        std::string name_ = teams[i].getName();
        if(int(name_.size()) > width){width = int(name_.size());}
    }
    width += 3;
    file << std::left << std::setw(width) << "Team Name" << std::right << std::setw(2) << "W" << std::setw(4) << "L" << std::setw(4) << "T" << std::setw(7) << "Win%" << std::setw(7) << "Goals" << std::setw(11) << "Penalties" << "\n";
    for(int j = 0; j < int(teams.size()); j++){
        file << std::left << std::setw(width) << teams[j].getName() << std::right << std::setw(2) << teams[j].getWins() << std::setw(4) << teams[j].getLoss() << std::setw(4) << teams[j].getTies() << std::setw(7) << std::fixed << std::setprecision(2) << teams[j].getWin_P() << std::setw(7) << teams[j].getGoals() << std::setw(11) << teams[j].getPenalties() << "\n";
    }
}
void output_player(std::vector<roster>& players, std::ofstream& file){
    int width = 0;
    int t_width = 0;
    for(int i = 0; i < int(players.size()); i++){
        std::string name_ = players[i].getName();
        if(int(name_.size()) > width){width = int(name_.size());}
    }
    for(int j = 0; j < int(players.size()); j++){
        std::string name__ = players[j].getTeam();
        if(int(name__.size()) > t_width){t_width = int(name__.size());}
    }
    width += 2; t_width += 3;
    file << std::left << std::setw(width) << "Player Name" << std::setw(t_width) << "Team" << std::right << std::setw(5) << "Goals" << std::setw(9) << "Assists" << std::setw(11) << "Penalties" << "\n";
    for(int k = 0; k < int(players.size()); k++){
        file << std::left << std::setw(width) << players[k].getName() << std::setw(t_width) << players[k].getTeam() << std::right << std::setw(5) << players[k].getGoals() << std::setw(9) << players[k].getAssists() << std::setw(11) << players[k].getPenalties() << "\n";
    }
}
void output_custom(std::vector<institution>& teams, std::ofstream& file){
    int t_width = 0;
    for(int j = 0; j < int(teams.size()); j++){
        std::string name__ = teams[j].getName();
        if(int(name__.size()) > t_width){t_width = int(name__.size());}
    }
    t_width += 3;
    file << std::left << std::setw(t_width) << "Team Name" << std::right << std::setw(12) << "Avg. Goals" << std::setw(24) << "Avg. Assists per Goal" << std::setw(18) << "Bench Penalties" << "\n";
    for(int i = 0; i < int(teams.size()); i++){
        file << std::left << std::setw(t_width) << teams[i].getName() << std::right << std::setw(12) << std::setprecision(2) << std::fixed << double(teams[i].getGoals()) / double(teams[i].getWins() + teams[i].getLoss() + teams[i].getTies()) << std::setw(24) << std::setprecision(2) << std::fixed << double(teams[i].getAssists()) / double(teams[i].getGoals()) << std::setw(18) << teams[i].getBenched() << "\n";
    }
}
bool team_sort(institution& a, institution& b){
    std::string name_a = a.getName();
    std::string name_b = b.getName();
    if(a.getWin_P() > b.getWin_P()){
        return true;
    }
    if(a.getWin_P() == b.getWin_P()){
        if(a.getGoals() > b.getGoals()){
            return true;
        }
        if(a.getGoals() == b.getGoals()){
            if(name_a.compare(name_b) < 0){
                return true;
            }
            else{
                return false;
            }
        }
    }
    return false;
}
bool player_sort(roster& a, roster& b){
    int t_total_a = a.getGoals() + a.getAssists();
    int t_total_b = b.getGoals() + b.getAssists();
    std::string a_name = a.getName();
    std::string b_name = b.getName();
    if(t_total_a > t_total_b){
        return true;
    }
    if(t_total_a == t_total_b){
        if(a.getPenalties() < b.getPenalties()){
            return true;
        }
        if(a.getPenalties() == b.getPenalties()){
            if(a_name.compare(b_name) < 0){
                return true;
            }
            else{
                return false;
            }
        }
    }
    return false;
}
