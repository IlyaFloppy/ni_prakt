//
//  task1.cpp
//  ni_prakt
//
//  Created by Ilya Popovnin on 2019-09-26.
//  Copyright © 2019 Ilya Popovnin. All rights reserved.
//

#include "task1.hpp"

uint32_t hexCharToInt[] = {
    // ASCII
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
    0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    
    // non-ASCII
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

uint32_t readCharToInt(std::string::iterator& it, const std::string::iterator& end) {
    if(it == end) {
        throw std::invalid_argument("trying to read from iterator that has already ended; probably input data is corrupted?");
        return 0;
    }
    return hexCharToInt[*it++];
}


Float::Float() {
    this->value = 0.0;
}
Float::Float(float value) {
    this->value = value;
}
Float::~Float() {
}
std::string Float::toString() {
    return std::to_string(this->value);
}
void Float::read(std::string::iterator& it, const std::string::iterator& end) {
    uint32_t val = 0;
    val += (readCharToInt(it, end) * 16 + readCharToInt(it, end)) << 24;
    val += (readCharToInt(it, end) * 16 + readCharToInt(it, end)) << 16;
    val += (readCharToInt(it, end) * 16 + readCharToInt(it, end)) << 8;
    val += (readCharToInt(it, end) * 16 + readCharToInt(it, end)) << 0;
    this->value = *((float*)&val);
}


Char::Char() {
    this->value = 0;
}
Char::Char(char value) {
    this->value = value;
}
Char::~Char() {
}
std::string Char::toString() {
    return std::to_string(this->value);
}
void Char::read(std::string::iterator& it, const std::string::iterator& end) {
    uint32_t val = 0;
    val += readCharToInt(it, end) * 16 + readCharToInt(it, end);
    this->value = *((char*)&val);
}


Boolean::Boolean() {
    this->value = false;
}
Boolean::Boolean(bool value) {
    this->value = value;
}
Boolean::~Boolean() {
}
std::string Boolean::toString() {
    return this->value ? "+" : "-";
}
void Boolean::read(std::string::iterator& it, const std::string::iterator& end) {
    uint32_t val = 0;
    val += readCharToInt(it, end) * 16 + readCharToInt(it, end);
    this->value = val > 0;
}

Quaternion::Quaternion() {
    this->w = 0.0;
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}
Quaternion::Quaternion(float w, float x, float y, float z) {
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}
Quaternion::Quaternion(float value[]) {
    this->w = value[0];
    this->x = value[1];
    this->y = value[2];
    this->z = value[3];
}
Quaternion::~Quaternion() {
}
std::string Quaternion::toString() {
    return std::to_string(this->w) + " + " +
           std::to_string(this->x) + "i + " +
           std::to_string(this->y) + "j + " +
           std::to_string(this->z) + "k";
}
void Quaternion::read(std::string::iterator& it, const std::string::iterator& end) {
    Float reader;
    reader.read(it, end);
    this->w = reader.value;
    reader.read(it, end);
    this->x = reader.value;
    reader.read(it, end);
    this->y = reader.value;
    reader.read(it, end);
    this->z = reader.value;
}




class Parcel {
public:
    enum class Field {Boolean, Char, Float, Quaternion};
    
private:
    std::map<int, Object*> data;
    std::map<int, std::string> names;
    std::map<int, std::string> units;
    std::map<int, Field> types;

public:
    Parcel() {
    }
    
    ~Parcel() {
        for(auto it = data.begin(); it != data.end(); ++it) {
            delete it->second;
        }
    }

    void addField(int id, std::string name, std::string unit, Field type) {
        this->names[id] = name;
        this->units[id] = unit;
        this->types[id] = type;
    }

    void readId(int id, std::string::iterator& it, const std::string::iterator& end) {
        Object* field;
        switch(types[id]) {
            case Field::Boolean:
                field = new Boolean();
                break;
            case Field::Char:
                field = new Char();
                break;
            case Field::Float:
                field = new Float();
                break;
            case Field::Quaternion:
                field = new Quaternion();
                break;
        }
        field->read(it, end);
        if(data.find(id) != data.end()) {
            delete data[id];
        }
        this->data[id] = field;
    }
    
    std::string toString() {
        std::string str = "id   name        value\n";
        for(auto const& pair : data) {
            int id = pair.first;
            std::string value = pair.second->toString();
            std::string name = names[id];
            std::string unit = units[id];
            str += std::to_string(id) + "   " + name + "    " + value + " " + unit + "\n";
        }
        return str;
    }
};

class PNITraxParcel : public Parcel {
public:
    PNITraxParcel() : Parcel() {
        this->addField(5, "kHeading", "º", Field::Float);
        this->addField(24, "kPitch", "º", Field::Float);
        this->addField(25, "kRoll", "º", Field::Float);
        this->addField(79, "kHeadingStatus", "", Field::Char);
        this->addField(77, "kQuaternion", "", Field::Quaternion);
        this->addField(7, "kTemperature", "ºC", Field::Float);
        this->addField(8, "kDistortion", "", Field::Boolean);
        this->addField(9, "kCalStatus", "", Field::Boolean);
        this->addField(21, "kAccelX", "g", Field::Float);
        this->addField(22, "kAccelY", "g", Field::Float);
        this->addField(23, "kAccelZ", "g", Field::Float);
        this->addField(27, "kMagX", "mT", Field::Float);
        this->addField(28, "kMagY", "mT", Field::Float);
        this->addField(29, "kMagZ", "mT", Field::Float);
        this->addField(74, "kGyroX", "rad/s", Field::Float);
        this->addField(75, "kGyroY", "rad/s", Field::Float);
        this->addField(76, "kGyroZ", "rad/s", Field::Float);
    }
};


void task1() {
    Parcel* parcel = new PNITraxParcel();
    
    std::string parcelAsString;
    std::cin >> parcelAsString;
    
    try {
        for(std::string::iterator it = parcelAsString.begin(); it != parcelAsString.end();) {
            int id = readCharToInt(it, parcelAsString.end()) * 16 + readCharToInt(it, parcelAsString.end());
            parcel->readId(id, it, parcelAsString.end());
        }
    } catch(const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
    }
    
    std::cout << parcel->toString();
    
    delete parcel;
}
