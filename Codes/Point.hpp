#ifndef Point_hpp
#define Point_hpp

class Point {

public:
    int x;
    int y;
    bool operator<(const Point) const;
    bool operator==(const Point) const;
    bool operator!=(const Point) const;
    Point(int, int);
    Point();
};

#endif /* Point_hpp */
