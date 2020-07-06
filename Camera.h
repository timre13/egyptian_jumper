#ifndef CAMERA_H_
#define CAMERA_H_

class Camera final
{
private:
    int m_x;
    int m_y;

public:
    Camera(int x, int y);

    int getX() const { return m_x; }
    int getY() const { return m_y; }

    void setX(int newX) { m_x = newX; }
    void setY(int newY) { m_y = newY; }
};



#endif /* CAMERA_H_ */
