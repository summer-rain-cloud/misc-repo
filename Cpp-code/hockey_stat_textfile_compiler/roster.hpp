//
//  roster.hpp
//
//  Created by Yu Xia on 1/24/23.
//

#ifndef roster_hpp
#define roster_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>

class roster{
public:
    // constructor
    roster(std::string name, std::string team_name);
    // simple functions :: 1-2 lines
    std::string getName() const{return name;}
    std::string getTeam() const{return team;}
    int getGoals() const{return goals;}
    int getAssists() const{return assists;}
    int getPenalties() const{return penalties;}
    void goalplus(){goals++;}
    void assistsplus(){assists++;}
    void penaltyplus(){penalties++;}
    
private:
    // name and part of which team
    std::string name, team;
    // stats and penalty time
    int goals, assists, penalties;
};

#endif /* roster_hpp */
