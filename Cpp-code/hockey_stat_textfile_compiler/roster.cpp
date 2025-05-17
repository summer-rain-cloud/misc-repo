//
//  roster.cpp
//
//  Created by Yu Xia on 1/24/23.
//

#include "roster.hpp"

roster::roster(std::string name_in, std::string team_name){
    name = name_in;
    team = team_name;
    goals = assists = penalties = 0;
}
