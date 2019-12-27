#ifndef FACTORY_H
#define FACTORY_H

#include <iostream>
#include "Shape.h"

template <typename T>
class Factory {
private:
    enum Type { triangle, rectangle, square };

public:
    static std::shared_ptr<Shape<T>> create(int num, std::istream &is)
    {
        switch (num)
        {
        case triangle:
            return std::make_shared< Triangle<T> >(is);
            break;
        
        case rectangle:
            return std::make_shared< Rectangle<T> >(is);
            break;
        
        case square:
            return std::make_shared< Square<T> >(is);
            break;

        default:
            throw std::logic_error("No such number");
            break;
        }
    }
    static std::shared_ptr<Shape<T>> create(int num, std::pair<T, T> (&p)[4])
    {
        switch (num)
        {
        case triangle:
            return std::make_shared< Triangle<T> >(p[0], p[1], p[2]);
            break;
        
        case rectangle:
            return std::make_shared< Rectangle<T> >(p[0], p[1], p[2], p[3]);
            break;
        
        case square:
            return std::make_shared< Square<T> >(p[0], p[1], p[2], p[3]);
            break;

        default:
            throw std::logic_error("No such number");
            break;
        }
    }
};

#endif