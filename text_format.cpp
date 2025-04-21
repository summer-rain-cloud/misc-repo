//
//  main.cpp
//  text formatter
//
//  Created by Yu Xia on 1/17/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//initalized here to define later because justify and f_output needs it for something
void into_file(std::ofstream& output, std::vector<std::string>& lines, std::string format, int width);
void f_output(std::ofstream& out, std::vector<std::string>& lines, int width){
    std::string border = "";
    for(int i = 0; i < width + 4; i++){
        border = border + "-";
    }
    //border here
    border = border + "\n";
    out << border;
    //input lines here
    for(int j = 0; j < int(lines.size()); j++){
        out << lines[j] << "\n";
    }
    //border here
    out << border;
}

void space_set(int capacity, int width, std::vector<std::string>& format_space){
    //checks if the line has two or less words because writing an universal space concatenator
    //if hell-ish and also time hefty, something I should've planned around on saturday
    if(format_space.size() <= 2){
        //this +1 is here because issues arise if it isn't due to the nature of lesser word lines
        int spaces = width - capacity + 1;
        for(int i = 1; i < int(format_space.size()); i++){
            format_space[i] = " " + format_space[i];
        }
        int counter = 0;
        while(spaces > 0){
            format_space[counter] = format_space[counter] + " ";
            spaces--;
            if(counter == width){counter = 1;}
        }
    }
    else{
        //the default if there are more words, allows the spaces between to be evenly distributed as
        //possible from the left most priority.
        
        //btw, not perfect spacing because of course it isn't
        //i needed another check for the spacing that looks into the occupied space of the
        //line to its capacity at a ratio of which it'll just left flush but
        //darn, don't got time
        int spaces = width - capacity;
        for(int i = 1; i < int(format_space.size()); i++){
            format_space[i] = " " + format_space[i];
        }
        int counter = 1;
        while(spaces > 0){
            format_space[counter] = " " + format_space[counter];
            spaces--; counter++;
            if(counter == int(format_space.size())){counter = 1;}
        }
    }
}

void justify(std::fstream& input, std::ofstream& output, int width){
    std::vector<std::string> format_space, lines;
    int capacity = -1; int wordcounted = 0; std::string unload;
    while(input >> unload){
        //checks for the capacity if the word is added
        int temp_cap = capacity + int(unload.size()) + 1;
        if(int(unload.size()) > width){
            if(format_space.size() > 0){
                space_set(capacity, width, format_space);
                
                std::string temp_line = "| ";
                for(int i = 0; i < int(format_space.size()); i++){
                    temp_line = temp_line + format_space[i];
                }
                temp_line = temp_line + " |";
                //pushes back previous line, clears the storage line and resets capacity
                lines.push_back(temp_line); format_space.clear(); capacity = 0;
            }
        
            while(int(unload.size()) >= width){
                std::string temp_word = "";
                for(int i = 0; i < width-1; i++){
                    temp_word = temp_word + unload[i];
                }
                temp_word = temp_word + "-"; lines.push_back("| " + temp_word + " |"); temp_word = "";
                for(int j = width-1; j < int(unload.size()); j++){
                    temp_word = temp_word + unload[j];
                }
                unload = temp_word; wordcounted++;
            }
            //the cleaning was done before hand so this section is left untouched
        }
        temp_cap = capacity + int(unload.size()) + 1;
        //reset the capacity due to previous if check
        //recheck if the capacity would over flow
        if(temp_cap > width){
            space_set(capacity, width, format_space);
            std::string temp_line = "| ";
            for(int i = 0; i < int(format_space.size()); i++){
                temp_line = temp_line + format_space[i];
            }
            temp_line = temp_line + " |";
            lines.push_back(temp_line); format_space.clear(); capacity = 0;
        }
        format_space.push_back(unload); capacity = capacity + int(unload.size()) + 1; wordcounted++;
    }
    //once end of file is reached, clean up the rest of the words and put them in a line
    space_set(capacity, width, format_space);
    std::string temp_line = "| ";
    for(int i = 0; i < int(format_space.size()); i++){
        temp_line = temp_line + format_space[i];
    }
    temp_line = temp_line + " |";
    lines.push_back(temp_line);
    //puts the lines in the output file
    if(wordcounted == 1){
        //... corner case on the occasion its a single word
        into_file(output, lines, "left", width);
    }
    f_output(output, lines, width);
}

void into_file(std::ofstream& output, std::vector<std::string>& lines, std::string format, int width){
    std::string border = "";
    //creates a border, reusable
    for(int i = 0; i < width + 4; i++){
        border += "-";
    }
    border = border + "\n";
    output << border;
    //formats the lines correctly based on left or right formatting
    for(int i = 0; i < int(lines.size()); i++){
        std::string formatted = "";
        int spaces = width - int(lines[i].size());
        //if left, put spaces at the end
        if(format == "left"){
            formatted = "| " + lines[i];
            if(spaces > 0){
                for(int i = 0; i < spaces; i++){formatted = formatted + " ";}
            }
            formatted = formatted + " |\n";
            output << formatted;
        }
        //if right, put spaces at the beginning
        if(format == "right"){
            formatted = "| ";
            if(spaces > 0){
                for(int i = 0; i < spaces; i++){formatted = formatted + " ";}
            }
            formatted = formatted + lines[i] + " |\n";
            output << formatted;
        }
    }
    //other border
    output << border;
}

void getLines(std::fstream& input_file, std::vector<std::string>& lines, std::vector<std::string>& stow, int width, int& wordcounted){
    //uses the input file to stow lines and then make lines
    //capacity is current line capacity, word_count counts total words, unload is temp word stowage
    int capacity, word_count; capacity = 0; word_count = 0; std::string unload;
    //capacity is set finnicky
    while(input_file >> unload){
        //temp_cap checks for capacity if the current word is to be added
        int temp_cap = capacity + int(unload.size()) + 1;
        //if the word is larger than the width, hyphenate
        if(int(unload.size()) > width){
            //check for anything in stow, if there is, end line and clear then make room for hyphenate
            if(stow.size()!=0){
                std::string temp_line = stow[0];
                for(unsigned int i = 1; i < stow.size(); i++){
                    temp_line = temp_line + " " + stow[i];
                }
                stow.clear(); capacity = 0; lines.push_back(temp_line);
            }
            //heyphenate and pass on to next line, while still larger than width, hyphenate again
            while(int(unload.size())>=width){
                std::string temp_load = "";
                for(int i = 0; i < width-1; i++){
                    temp_load = temp_load + unload[i];
                }
                temp_load = temp_load + "-"; lines.push_back(temp_load); temp_load = "";
                //after hyphenating, set unload as the leftover
                for(unsigned int j = width-1; j < unload.size(); j++){
                    temp_load = temp_load + unload[j];
                }
                unload = temp_load;
            }
            capacity = int(unload.size());
            word_count++;
        }
        //if over capacity, end the line and clear away stow and reset capacity to -1
        temp_cap = capacity + int(unload.size()) + 1;
        if(temp_cap > width){
            std::string temp_line = stow[0];
            for(unsigned int i = 1; i < stow.size(); i++){
                temp_line = temp_line + " " + stow[i];
            }
            lines.push_back(temp_line); capacity = -1; stow.clear();
        }
        //if none of the above is true ergo not over capacity and not singuarly over width
        //complete the process as normal
        stow.push_back(unload); capacity = capacity + int(unload.size()) + 1; word_count++;
    }
    //once end of file is met complete the rest of the words in stow and do so accordingly
    //check if there exists any items in vector then proceed if there is
    if(int(stow.size()) != 0){
        std::string temp_line = stow[0];
        for(unsigned int i = 1; i < stow.size(); i++){
            temp_line = temp_line + " " + stow[i];
        }
        lines.push_back(temp_line);
    }
    wordcounted = word_count;
}

void format_type(std::string format, int width, std::fstream& input_file, std::ofstream& output_file){
    //sets the format type into bools
    std::string flush = format;
    //two vectors, stow keeps individual words temporarily and lines keep whole lines until formattting
    std::vector<std::string> stow, lines; int wordcount = 0;
    //call to return a filled lines vector
    getLines(input_file, lines, stow, width, wordcount);
    //corner case check for single word
    if(wordcount == 1){format = "left";}
    into_file(output_file, lines, flush, width);
    //pass on lines to another function to be put into the txt file
}

int main(int argc, const char * argv[]) { /*argv is [input, output, width, command]*/
    // initialize values
    std::fstream input_f;
    std::ofstream output_f;
    int width;
    bool left, right, center;
    left = right = center = false;
    std::string command;
    // simple error checking for command line arguments not over 4
    if(argc > 5){
        std::cerr << "ERROR! :: Detected Extra Argument(s). \n"; exit(0);
    }
    // check the filenames and opens them, subsequently check if they're "good"
    input_f.open(argv[1]);
    output_f.open(argv[2]);
    if(!(input_f.good())){
        std::cerr << "ERROR! :: One of your files cannot be accessed. \n"; exit(0);
    }
    // check for valid width input
    std::string temp = argv[3];
    for(unsigned int i = 0; i < temp.size(); i++){
        if(isalpha(temp[i]) != 0){
            std::cerr << "ERROR! :: Invalid width argument. \n"; exit(0);
        }
    }
    width = std::stoi(temp);
    // check for valid command input
    command = argv[4];
    if(command.compare("flush_left") == 0){left = true;}
    else if(command.compare("flush_right") == 0){right = true;}
    else if(command.compare("full_justify") == 0){center = true;}
    else{std::cerr << "ERROR! :: Invalid/Unknown command. \n"; exit(0);}
    // start of formatting process
    if(left){format_type("left", width, input_f, output_f);}
    if(right){format_type("right", width, input_f, output_f);}
    if(center){justify(input_f, output_f, width);}
    // end of code...
    input_f.close(); output_f.close();
    return 0;
}
