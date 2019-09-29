//
//  task2.cpp
//  ni_prakt
//
//  Created by Ilya Popovnin on 2019-09-26.
//  Copyright © 2019 Ilya Popovnin. All rights reserved.
//

#include "task2.hpp"

void task2() {
    LinkedList<int64_t> list;
    
    for(int i = 0; i < 10; i++) {
        int64_t* random = new int64_t(std::rand() % 100);
        list.addLast(random);
    }
    
    
    list.print();
    
    list.mergeSort();
    std::cout << std::endl << "sorted: " << std::endl << std::endl;
    
    list.print();
    
    std::cout << std::endl << "swapped: " << std::endl << std::endl;
    list.swap(0, list.getSize() - 1);
    
    list.print();
    
    for(int i = 0; i < list.getSize(); i++) {
        delete list.popFirst();
    }
}
