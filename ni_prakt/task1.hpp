//
//  task1.hpp
//  ni_prakt
//
//  Created by Ilya Popovnin on 2019-09-03.
//  Copyright © 2019 Ilya Popovnin. All rights reserved.
//

#ifndef task1_hpp
#define task1_hpp

#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

class Object {
public:
    virtual std::string toString() = 0;
    virtual void read(std::string::iterator& it, const std::string::iterator& end) = 0;
    
    virtual ~Object() = default;
};

class Float : public Object {
public:
    float value;
    Float();
    Float(float value);
    ~Float();
    std::string toString() override;
    void read(std::string::iterator& it, const std::string::iterator& end) override;
};

class Char : public Object {
public:
    char value;
    Char();
    Char(char value);
    ~Char();
    std::string toString() override;
    void read(std::string::iterator& it, const std::string::iterator& end) override;
};

class Boolean : public Object {
public:
    bool value;
    Boolean();
    Boolean(bool value);
    ~Boolean();
    std::string toString() override;
    void read(std::string::iterator& it, const std::string::iterator& end) override;
};

class Quaternion : public Object {
public:
    float w, x, y, z;
    Quaternion();
    Quaternion(float w, float x, float y, float z);
    Quaternion(float value[]);
    ~Quaternion();
    std::string toString() override;
    void read(std::string::iterator& it, const std::string::iterator& end) override;
};

void task1();


#endif /* task1_hpp */
