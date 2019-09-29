//
//  main.cpp
//  ni_prakt
//
//  Created by Ilya Popovnin on 2019-09-26.
//  Copyright © 2019 Ilya Popovnin. All rights reserved.
//

#include <iostream>
#include "task1.hpp"
#include "task2.hpp"
#include "task3.hpp"

int main(int argc, const char * argv[]) {
    if(argc > 1) {
        if(*argv[1] == '1') {
            task1();
        }
        else if(*argv[1] == '2') {
            task2();
        }
        else if(*argv[1] == '3') {
            task3();
        }
    } else {
        std::cerr << "Run with a parameter [ 1 | 2 | 3 ]" << std::endl;
    }
    return 0;
}
