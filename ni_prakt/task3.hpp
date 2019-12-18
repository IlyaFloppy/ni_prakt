//
//  task3.hpp
//  ni_prakt
//
//  Created by Ilya Popovnin on 2019-09-20.
//  Copyright © 2019 Ilya Popovnin. All rights reserved.
//

#ifndef task3_hpp
#define task3_hpp

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <fstream>

class Region {
public:
    double width, length, depth;
    Region(double width, double length, double depth) {
        this->width = width;
        this->length = length;
        this->depth = depth;
    }
};


class Vector {
public:
    double x, y, z;
    
    Vector() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    
    Vector(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};


class Mission {
public:
    
    virtual ~Mission() {};
    
    virtual Vector & apply(Vector &vector, Region &region) {
        return vector;
    }
    
    virtual std::string toString() = 0;
    
    std::string dtos(double d) {
        std::ostringstream s;
        s << d;
        return s.str();
    }
};

class Dive : public Mission {
public:
    constexpr const static double DEFAULT_RADIUS = 1;
    enum class Type {Spiral, Vertical};
protected:
    double radius = 0;
    double distance;
    bool distanceAsDepth;
    Type type = Type::Vertical;
    
public:
    
    Dive(double distance, bool distanceAsDepth, double radius = -1) {
        this->distance = distance;
        this->distanceAsDepth = distanceAsDepth;
        this->type = radius > 0 ? Type::Spiral : Type::Vertical;
        this->radius = radius;
    }
    
    Vector & apply(Vector &vector, Region &region) override {
        vector.y -= distance;
        if(
           std::abs(vector.x) + this->radius > region.length / 2 ||
           std::abs(vector.z) + this->radius > region.width / 2) {
            throw std::invalid_argument("uuv has gone out of region");
        }
        if(vector.y < -region.depth) {
            std::cerr << vector.y << std::endl;
            throw std::invalid_argument("uuv is six feet under ground");
        }
        if(vector.y > 0) {
            throw std::invalid_argument("uuvs don't fly, do they?");
        }
        return vector;
    }
    
    std::string toString() override {
        return std::string("dive") +
               std::string(this->distanceAsDepth ? " asdepth" : "") +
               std::string(this->type == Type::Spiral ? " spiral" : "") +
               std::string(" ") +
               dtos(this->distance) +
               std::string(this->type == Type::Spiral ? " " : "") +
               (this->type == Type::Spiral ? dtos(this->radius) : std::string(""));
    }
};


class Lift : public Dive {
public:
    Lift(double distance, bool distanceAsDepth, double radius) : Dive(-distance, distanceAsDepth, radius) {}
    
    std::string toString() override {
        return std::string("lift") +
               std::string(this->distanceAsDepth ? " asdepth" : "") +
               std::string(this->type == Type::Spiral ? " spiral" : "") +
               std::string(" ") +
               dtos(-this->distance) +
               std::string(this->type == Type::Spiral ? " " : "") +
               (this->type == Type::Spiral ? dtos(this->radius) : std::string(""));
    }
    
    Vector & apply(Vector &vector, Region &region) override {
        vector.y += distance;
        if(
           std::abs(vector.x) + this->radius > region.length / 2 ||
           std::abs(vector.z) + this->radius > region.width / 2) {
            throw std::invalid_argument("uuv has gone out of region");
        }
        if(vector.y < -region.depth) {
            std::cerr << vector.y << std::endl;
            throw std::invalid_argument("uuv is six feet under ground");
        }
        if(vector.y > 0) {
            throw std::invalid_argument("uuvs don't fly, do they?");
        }
        return vector;
    }
};


class Move: public Mission {
public:
    constexpr const static double DEFAULT_PRECISION = 1;
    enum class Type {ToPoint, Parallel};
    enum class DepthControl {ConstantDepth, ConstantDistance};
private:
    double x, z, precision = 1;
    Type type = Type::ToPoint;
    DepthControl depthControl = DepthControl::ConstantDepth;
public:
    
    Move(double x, double z, double precision, DepthControl control, Type type) {
        this->x = x;
        this->z = z;
        this->precision = precision;
        this->depthControl = control;
        this->type = type;
    }
    
    Vector & apply(Vector &vector, Region &region) override {
        vector.x = this->x;
        vector.z = this->z;
        if(
           std::abs(vector.x) > region.length / 2 ||
           std::abs(vector.z) > region.width / 2) {
            throw std::invalid_argument("uuv has gone out of region");
        }
        return vector;
    }
    
    std::string toString() override {
        auto constDepth = this->depthControl == DepthControl::ConstantDepth;
        auto parallel = this->type == Type::Parallel;
        return std::string("move") +
        std::string(constDepth ? " asdepth" : "") +
        std::string(parallel ? " parallel " : " ") +
        dtos(this->x) +
        std::string(" ") +
        dtos(this->z) +
        std::string(" ") +
        dtos(this->precision);
    }
};


class Return: public Mission {
private:
    double x, y, z;
public:
    Return(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    Vector & apply(Vector &vector, Region &region) override {
        vector.x = this->x;
        vector.y = this->y;
        vector.z = this->z;
        return vector;
    }
    
    std::string toString() override {
        return std::string("return ") +
               dtos(this->x) +
               std::string(" ") +
               dtos(this->y) +
               std::string(" ") +
               dtos(this->z);
    }
};

void task3();

#endif /* task3_hpp */
