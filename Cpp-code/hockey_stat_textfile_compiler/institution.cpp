//
//  institution.cpp
//
//  Created by Yu Xia on 1/24/23.
//

#include "institution.hpp"

institution::institution(std::string team_name){
    name = team_name;
    wins = loss = ties = penalties = bench_penalties = win_percent = 0;
    t_goals = t_assists = 0;
}
