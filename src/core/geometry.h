
#ifndef GEOMETRY_H
#define GEOMETRY_H

class Rectangle
{
    private:
        int width, height;
        int posx, posy;
    public:
        Rectangle();
        Rectangle(int width, int height, int posx, int posy);
        float get_width() const;
        float get_height() const;
        float get_x() const;
        float get_y() const;
        explicit operator bool() const;
};

#endif //GEOMETRY_H
