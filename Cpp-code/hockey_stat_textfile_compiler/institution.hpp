//
//  institution.hpp
//
//  Created by Yu Xia on 1/24/23.
//

#ifndef institution_hpp
#define institution_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>

class institution{
public:
    // constructor
    institution(std::string team_name);
    // simple functions :: 1-2 lines
    std::string getName() const{return name;}
    int getWins() const{return wins;}
    int getLoss() const{return loss;}
    int getTies() const{return ties;}
    int getGoals() const{return t_goals;}
    int getAssists() const{return t_assists;}
    int getPenalties() const{return penalties;}
    int getBenched() const{return bench_penalties;}
    void setPercent(){win_percent = double(wins + (0.5*double(ties))) / double(wins + loss + ties);}
    void addWins(){++wins; setPercent();}
    void addLoss(){loss++; setPercent();}
    void addTies(){ties++; setPercent();}
    void addPenalty(int bad){penalties += bad;}
    void addBenchedPenalty(int too_many_players){bench_penalties += too_many_players;}
    void add_t_goals(int goals){t_goals += goals;}
    void add_t_assists(int assists){t_assists += assists;}
    double getWin_P() const{return win_percent;}
    
    
private:
    std::string name;
    int wins, loss, ties, penalties;
    int t_goals, t_assists; 
    int bench_penalties;
    double win_percent;
};

#endif /* institution_hpp */
