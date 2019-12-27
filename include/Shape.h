#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <utility>
#include <cmath>
#include <vector>

typedef std::pair<double, double> DoublePoint;



//---------------------------------------------------------------------
// Necessary for friend declaration
//---------------------------------------------------------------------
template <typename T>
class Shape;

template <typename T>
std::ostream& operator << (std::ostream& os, const Shape<T>& shape);
//---------------------------------------------------------------------

template <typename T>
class Shape {
public:
    friend std::ostream& operator << <T> (std::ostream& os, const Shape& shape);
    typedef std::pair<T, T> Point;
    
    DoublePoint getCentre() const
    { return centre; }

    double getArea() const
    { return area; }

    bool getAbstract() const
    { return abstract; }

    int getType() const
    { return type; }

    std::vector<Point> getPoints() const
    { return p; }
 protected:
    DoublePoint centre;
    double area = 0;
    bool abstract = false;
    int type;
    std::vector<Point> p;

private:
    virtual void Centre() = 0;
    virtual void Area() = 0;
    virtual std::ostream& print(std::ostream&) const = 0;

};

template <typename T>
class Triangle : public Shape<T> {
public:
    using typename Shape<T>::Point;

    using Shape<T>::getCentre;
    using Shape<T>::getArea;

    using Shape<T>::centre;
    using Shape<T>::area;
    using Shape<T>::abstract;
    using Shape<T>::type;
    using Shape<T>::p;

    Triangle(Point a, Point b, Point c);
    Triangle(std::istream& is);

    void Centre() override;
    void Area() override;
    std::ostream& print(std::ostream&) const override;
};

template <typename T>
class Rectangle : public Shape<T> {
public:
    using typename Shape<T>::Point;

    using Shape<T>::getCentre;
    using Shape<T>::getArea;

    using Shape<T>::centre;
    using Shape<T>::area;
    using Shape<T>::abstract;
    using Shape<T>::type;
    using Shape<T>::p;

    Rectangle(Point a, Point b, Point c, Point d);
    Rectangle(std::istream& is);

    void Centre() override;
    void Area() override;
    std::ostream& print(std::ostream&) const override;
};


template <typename T>
class Square : public Shape<T> {
public:
    using typename Shape<T>::Point;

    using Shape<T>::getCentre;
    using Shape<T>::getArea;

    using Shape<T>::centre;
    using Shape<T>::area;
    using Shape<T>::abstract;
    using Shape<T>::type;
    using Shape<T>::p;

    Square(Point a, Point b, Point c, Point d);
    Square(std::istream& is);

    void Centre() override;
    void Area() override;
    std::ostream& print(std::ostream&) const override;
};

//-----------------------------------------------------------------------------------------------------------
// Geometrical correctness
//-----------------------------------------------------------------------------------------------------------

bool DoubleEqual(double lhs, double rhs)
{
    const double EPS = 0.00001;
    if (lhs > rhs)
        return (lhs - rhs) < EPS ? true : false;
    else
        return (rhs - lhs) < EPS ? true : false;
}

bool IsRight(std::pair<double, double> a, std::pair<double, double> b, std::pair<double, double> c)
{
    std::pair<double, double> vec1 = { b.first - a.first, b.second - a.second };
    std::pair<double, double> vec2 = { c.first - a.first, c.second - a.second };

    double result = vec1.first * vec2.first + vec1.second * vec2.second;
    if (DoubleEqual(result, 0)) return true;
    return false;
}

bool IsRight(std::pair<int, int> a, std::pair<int, int> b, std::pair<int, int> c)
{
    std::pair<int, int> vec1 = { b.first - a.first, b.second - a.second };
    std::pair<int, int> vec2 = { c.first - a.first, c.second - a.second };

    int result = vec1.first * vec2.first + vec1.second * vec2.second;
    if (result == 0) return true;
    return false;
}

//-----------------------------------------------------------------------------------------------------------
// Check if shape is a rectangle
//-----------------------------------------------------------------------------------------------------------

template <class T>
bool IsTriangle(std::vector< std::pair<T, T> > &p)
{
    return !DoubleEqual
        ( ((p[1].first - p[0].first) * (p[2].second - p[0].second) - (p[1].second - p[0].second) * (p[2].first - p[0].first)), 0 );
}

//-----------------------------------------------------------------------------------------------------------
// Check if shape is a rectangle
//-----------------------------------------------------------------------------------------------------------

template <class T>
bool IsRectangle(const std::vector< std::pair<T, T> > &p)
{
    std::pair<T, T> null(0, 0);
    if (p[0] == null && p[1] == null && p[2] == null && p[3] == null) return false;

    if (
            IsRight(p[2], p[1], p[3]) &&
            IsRight(p[3], p[2], p[0]) &&
            IsRight(p[1], p[0], p[2]) &&
            IsRight(p[0], p[3], p[1])
        ) return true;
    return false;
}

//-----------------------------------------------------------------------------------------------------------
// Check if shape is a square
//-----------------------------------------------------------------------------------------------------------

template <class T>
double distance(const std::pair<T, T> a, std::pair<T, T> b)
{
    return (b.first - a.first) * (b.first - a.first) + (b.second - a.second) * (b.second - a.second);
}

template <class T>
bool IsSquare(const std::vector< std::pair<T, T> > &p)
{
    if (
            IsRectangle(p) &&
            DoubleEqual(distance(p[0], p[1]), distance(p[1], p[2])) &&
            DoubleEqual(distance(p[1], p[2]), distance(p[2], p[3])) &&
            DoubleEqual(distance(p[2], p[3]), distance(p[3], p[0])) &&
            DoubleEqual(distance(p[3], p[0]), distance(p[0], p[1]))
        ) return true;
    return false;
}

//-----------------------------------------------------------------------------------------------------------
// Polymorphism safe out operator overload
//-----------------------------------------------------------------------------------------------------------

template <class T>
std::ostream& operator << (std::ostream& os, const Shape<T>& shape)
{
    shape.print(os);
    return os;
}

template <class T>
std::ostream& operator << (std::ostream& os, const std::pair<T,T>& p)
{
    os << '(' << p.first << " , " << p.second << ')';
    return os;
}

//-----------------------------------------------------------------------------------------------------------
// Triangle functions
//-----------------------------------------------------------------------------------------------------------

template <class T>
Triangle<T>::Triangle(Point a, Point b, Point c)
{
    p.push_back(a);
    p.push_back(b);
    p.push_back(c);

    if (!IsTriangle(p)) { abstract = true; }

    Centre();
    Area();
    type = 0;
}

template <class T>
Triangle<T>::Triangle(std::istream& is)
{
    Point points[3];

    std::cout << "Input point coordinates\nA: ";
    is >> points[0].first >> points[0].second;
    std::cout << "B: ";
    is >> points[1].first >> points[1].second;
    std::cout << "C: ";
    is >> points[2].first >> points[2].second;

    p.push_back(points[0]);
    p.push_back(points[1]);
    p.push_back(points[2]);

    if (!IsTriangle(p)) { abstract = true; }

    Centre();
    Area();
}

template <class T>
void Triangle<T>::Centre()
{
    for (int i = 0; i < 3; ++i)
    {
        centre.first += p[i].first;
        centre.second += p[i].second;
    }
    centre.first /= 3;
    centre.second /= 3;
}

template <class T>
void Triangle<T>::Area()
{
    area = (double) (
                          p[0].first * (p[1].second - p[2].second)
                        + p[1].first * (p[2].second - p[0].second)
                        + p[2].first * (p[0].second - p[1].second)
                    ) / 2;
    if (area < 0) area = -area;
}

template <class T>
std::ostream& Triangle<T>::print(std::ostream& os) const
{
    if (abstract)
    {
        os << "Abstract: ";
    }
    else
    {
        os << "Triangle: ";
    }

    os << p[0] << "; " << p[1] << "; " << p[2]
       << "; Centre = " << getCentre()
       << "; Area = " << getArea();
    return os;
}

//-----------------------------------------------------------------------------------------------------------
// Rectangle functions
//-----------------------------------------------------------------------------------------------------------

template <class T>
Rectangle<T>::Rectangle(Point a, Point b, Point c, Point d)
{
    p.push_back(a);
    p.push_back(b);
    p.push_back(c);
    p.push_back(d);

    if (!IsRectangle(p)) { abstract = true; }

    Centre();
    Area();
    type = 1;
}

template <class T>
Rectangle<T>::Rectangle(std::istream& is)
{
    Point points[4];

    std::cout << "Input point coordinates clockwise or counter clockwise\nA: ";
    is >> points[0].first >> points[0].second;
    std::cout << "B: ";
    is >> points[1].first >> points[1].second;
    std::cout << "C: ";
    is >> points[2].first >> points[2].second;
    std::cout << "D: ";
    is >> points[3].first >> points[3].second;

    p.push_back(points[0]);
    p.push_back(points[1]);
    p.push_back(points[2]);
    p.push_back(points[3]);

    if (!IsRectangle(p)) { abstract = true; }
    
    Centre();
    Area();
    type = 1;
}

template <class T>
void Rectangle<T>::Centre()
{
    for (int i = 0; i < 4; ++i)
    {
        centre.first += p[i].first;
        centre.second += p[i].second;
    }
    centre.first /= 4;
    centre.second /= 4;
}

template <class T>
void Rectangle<T>::Area()
{
    area = sqrt( distance(p[0], p[1]) * distance(p[1], p[2]) );
}

template <class T>
std::ostream& Rectangle<T>::print(std::ostream& os) const
{
    if (abstract)
    {
        os << "Abstract: ";
    }
    else
    {
        os << "Rectangle: ";
    }
    
    os << p[0] << "; " << p[1] << "; " << p[2] << "; " << p[3]
       << "; Centre = " << getCentre()
       << "; Area = " << getArea();
    return os;
}

//-----------------------------------------------------------------------------------------------------------
// Square functions
//-----------------------------------------------------------------------------------------------------------

template <class T>
Square<T>::Square(Point a, Point b, Point c, Point d)
{
    p.push_back(a);
    p.push_back(b);
    p.push_back(c);
    p.push_back(d);

    if (!IsSquare(p)) { abstract = true; }

    Centre();
    Area();
    type = 2;
}

template <class T>
Square<T>::Square(std::istream& is)
{
    Point points[4];

    std::cout << "Input point coordinates clockwise or counter clockwise\nA: ";
    is >> points[0].first >> points[0].second;
    std::cout << "B: ";
    is >> points[1].first >> points[1].second;
    std::cout << "C: ";
    is >> points[2].first >> points[2].second;
    std::cout << "D: ";
    is >> points[3].first >> points[3].second;

    p.push_back(points[0]);
    p.push_back(points[1]);
    p.push_back(points[2]);
    p.push_back(points[3]);

    if (!IsSquare(p)) { abstract = true; }

    Centre();
    Area();
    type = 2;
}

template <class T>
void Square<T>::Centre()
{
    for (int i = 0; i < 4; ++i)
    {
        centre.first += p[i].first;
        centre.second += p[i].second;
    }
    centre.first /= 4;
    centre.second /= 4;
}

template <class T>
void Square<T>::Area()
{
    area = sqrt( distance(p[0], p[1]) * distance(p[1], p[2]) );
}

template <class T>
std::ostream& Square<T>::print(std::ostream& os) const
{
    if (abstract)
    {
        os << "Abstract: ";
    }
    else
    {
        os << "Square: ";
    }

    os << p[0] << "; " << p[1] << "; " << p[2] << "; " << p[3]
       << "; Centre = " << getCentre()
       << "; Area = " << getArea();
    return os;
}

//-----------------------------------------------------------------------------------------------------------
// Compare points
//-----------------------------------------------------------------------------------------------------------

bool operator == (std::pair<int, int> lhs, std::pair<int, int> rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

bool operator == (std::pair<double, double> lhs, std::pair<double, double> rhs)
{
    return DoubleEqual(lhs.first, rhs.first) && DoubleEqual(lhs.second, rhs.second);
}


#endif
