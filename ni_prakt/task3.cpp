//
//  task3.cpp
//  ni_prakt
//
//  Created by Ilya Popovnin on 2019-09-26.
//  Copyright © 2019 Ilya Popovnin. All rights reserved.
//

#include "task3.hpp"

enum class Task {Add, Remove, Edit, Exit, Save};


void task3() {
    const std::string delimiter = " ";
    std::string input;
    
    std::vector<Mission*> missions;
    
    while(true) {
        std::vector<std::string> command;
    
        std::getline(std::cin, input);
        
        size_t pos = 0;
        std::string word;
        while ((pos = input.find(delimiter)) != std::string::npos) {
            word = input.substr(0, pos);
            command.push_back(word);
            input.erase(0, pos + delimiter.length());
        }
        command.push_back(input);
        
        
        try {
            if(command.at(0) == "exit") {
                break;
            }
            if(command.at(0) == "help" || command.at(0) == "h" || command.at(0) == "?") {
                std::cout << "Help yourself" << std::endl;
            }
            if(command.at(0) == "list" || command.at(0) == "ls") {
                for(int i = 0; i < missions.size(); i++) {
                    std::cout << (i + 1) << ". " << missions.at(i)->toString() << std::endl;
                }
            }
            if(command.at(0) == "add") {
                if(command.at(1) == "dive") {
                    bool distanceAsDepth = std::find(command.begin(), command.end(), "asdepth") != command.end();
                    command.erase(std::remove(command.begin(), command.end(), "asdepth"), command.end());
                    bool spiral = std::find(command.begin(), command.end(), "spiral") != command.end();
                    command.erase(std::remove(command.begin(), command.end(), "spiral"), command.end());
                    double distance = std::stod(command.at(2));
                    double radius = -1;
                    if(spiral) {
                        if(command.size() > 3) {
                            radius = std::stod(command.at(3));
                        }
                        else {
                            radius = Dive::DEFAULT_RADIUS;
                        }
                    }
                    
                    Dive* mission = new Dive(distance, distanceAsDepth, radius);
                    missions.push_back(mission);
                }
                if(command.at(1) == "lift") {
                    bool distanceAsDepth = std::find(command.begin(), command.end(), "asdepth") != command.end();
                    command.erase(std::remove(command.begin(), command.end(), "asdepth"), command.end());
                    bool spiral = std::find(command.begin(), command.end(), "spiral") != command.end();
                    command.erase(std::remove(command.begin(), command.end(), "spiral"), command.end());
                    double distance = std::stod(command.at(2));
                    double radius = -1;
                    if(spiral) {
                        if(command.size() > 3) {
                            radius = std::stod(command.at(3));
                        }
                        else {
                            radius = Lift::DEFAULT_RADIUS;
                        }
                    }
                    
                    Lift* mission = new Lift(distance, distanceAsDepth, radius);
                    missions.push_back(mission);
                }
                if(command.at(1) == "move") {
                    bool parallel = std::find(command.begin(), command.end(), "parallel") != command.end();
                    command.erase(std::remove(command.begin(), command.end(), "parallel"), command.end());
                    bool distanceAsDepth = std::find(command.begin(), command.end(), "asdepth") != command.end();
                    command.erase(std::remove(command.begin(), command.end(), "asdepth"), command.end());
                    double x = std::stod(command.at(2));
                    double z = std::stod(command.at(3));
                    double precision = Move::DEFAULT_PRECISION;
                    if(command.size() > 4) {
                        precision = std::stod(command.at(4));
                    }
                    
                    Move* mission = new Move(x, z, precision,
                                             distanceAsDepth ? Move::DepthControl::ConstantDepth : Move::DepthControl::ConstantDistance,
                                             parallel ? Move::Type::Parallel : Move::Type::ToPoint);
                    missions.push_back(mission);
                }
                if(command.at(1) == "return") {
                    double x = std::stod(command.at(2));
                    double y = std::stod(command.at(3));
                    double z = std::stod(command.at(4));
                    
                    Return* mission = new Return(x, y, z);
                    missions.push_back(mission);
                }
            }
            if(command.at(0) == "remove") {
                int index = std::stoi(command.at(1));
                if(index < 1 or index > missions.size()) {
                    throw std::invalid_argument("wrong index");
                }
                delete missions.at(index - 1);
                missions.erase(missions.begin() + index - 1);
            }
            if(command.at(0) == "swap") {
                int index1 = std::stoi(command.at(1));
                int index2 = std::stoi(command.at(2));
                if(index1 < 1 or index1 > missions.size()) {
                    throw std::invalid_argument("wrong index");
                }
                if(index2 < 1 or index2 > missions.size()) {
                    throw std::invalid_argument("wrong index");
                }
                std::iter_swap(missions.begin() + index1 - 1, missions.begin() + index2 - 1);
            }
            if(command.at(0) == "edit") {
                // TODO: whatever
            }
            if(command.at(0) == "save") {
                std::string filename = command.at(1);
                
                std::ofstream file;
                file.open(filename);
                for(int i = 0; i < missions.size(); i++) {
                    file << "add " << missions.at(i)->toString() << std::endl;
                }
                file.close();
            }
            if(command.at(0) == "load") {
                std::cerr << "Run:\n$ cat filename.txt - | ./ni_prakt 3\nfrom shell to load file" << std::endl;
            }
        } catch(std::exception &e) {
            std::cerr << "wrong syntax" << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }
}
