//
//  task3.cpp
//  ni_prakt
//
//  Created by Ilya Popovnin on 2019-09-26.
//  Copyright © 2019 Ilya Popovnin. All rights reserved.
//

#include "task3.hpp"

enum class Task {Add, Remove, Edit, Exit, Save};

void add(std::vector<Mission*> &missions, std::vector<std::string> &command, int index = -1, bool replace = false) {
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
        
        if (index == -1) {
            missions.push_back(mission);
        } else if (replace) {
            Mission *old = missions[index];
            delete old;
            missions[index] = mission;
        } else {
            missions.insert(missions.begin() + index, mission);
        }
        
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
        
        if (index == -1) {
            missions.push_back(mission);
        } else if (replace) {
            Mission *old = missions[index];
            delete old;
            missions[index] = mission;
        } else {
            missions.insert(missions.begin() + index, mission);
        }
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
        
        if (index == -1) {
            missions.push_back(mission);
        } else if (replace) {
            Mission *old = missions[index];
            delete old;
            missions[index] = mission;
        } else {
            missions.insert(missions.begin() + index, mission);
        }
    }
    if(command.at(1) == "return") {
        double x = std::stod(command.at(2));
        double y = std::stod(command.at(3));
        double z = std::stod(command.at(4));
        
        Return* mission = new Return(x, y, z);
        
        if (index == -1) {
            missions.push_back(mission);
        } else if (replace) {
            Mission *old = missions[index];
            delete old;
            missions[index] = mission;
        } else {
            missions.insert(missions.begin() + index, mission);
        }
    }
}

void save(std::vector<Mission*> &missions, std::string filename) {
    std::ofstream file;
    file.open(filename);
    for(int i = 0; i < missions.size(); i++) {
        file << "add " << missions.at(i)->toString() << std::endl;
    }
    file.close();
}

bool apply(std::vector<Mission*> &missions) {
    auto reg = Region(10000, 10000, 100);
    auto pos = Vector(0, 0, 0);
    try {
        for (int i = 0; i < missions.size(); i++) {
            missions.at(i)->apply(pos, reg);
        }
        if (dynamic_cast<Return*>(missions.at(missions.size() - 1)) == nullptr) {
            throw std::invalid_argument("list has to end with return");
        }
    } catch (std::exception &e) {
        std::cerr << "error applying to vector" << std::endl;
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

std::vector<Mission*> readFromStream(std::istream &stream) {
    const std::string delimiter = " ";
    std::vector<Mission*> missions;
    std::string input;
    
    while (true) {
        std::vector<std::string> command;
        
        if(!std::getline(stream, input))
            break;
        
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
            if(command.at(0) == "list" || command.at(0) == "ls") {
                for(int i = 0; i < missions.size(); i++) {
                    std::cout << (i + 1) << ". " << missions.at(i)->toString() << std::endl;
                }
            }
            if(command.at(0) == "add") {
                add(missions, command);
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
                int index = std::stoi(command.at(1));
                command.erase(std::remove(command.begin(), command.end(), "edit"), command.end());
                if(index < 1 or index > missions.size()) {
                    throw std::invalid_argument("wrong index");
                }
                add(missions, command, index - 1, true);
            }
            if(command.at(0) == "insert") {
                int index = std::stoi(command.at(1));
                command.erase(std::remove(command.begin(), command.end(), "insert"), command.end());
                if(index < 1 or index > missions.size()) {
                    throw std::invalid_argument("wrong index");
                }
                add(missions, command, index - 1, false);
            }
            if(command.at(0) == "save") {
                std::string filename = command.at(1);
                
                // check if mission are ok
                if (apply(missions)) {
                    save(missions, filename);
                }
            }
            if(command.at(0) == "load") {
                std::string filename = command.at(1);
                
                std::ifstream fileStream(filename);
                if (fileStream.is_open()) {
                    auto newMissions = readFromStream(fileStream);
                    
                    for(int i = 0; i < missions.size(); i++) {
                        delete missions.at(i);
                    }
                    
                    missions = newMissions;
                }
            }
            
            apply(missions);
            // autosave always
            save(missions, "autosave.txt");
            
        } catch(std::exception &e) {
            std::cerr << "wrong syntax" << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }
    
    return missions;
}

void task3() {
    readFromStream(std::cin);
}
