class Position {
private:
    uint8_t x;
    uint8_t y;

public:
    Position(uint8_t x = 0, uint8_t y = 0) : x(x), y(y) {}

    uint8_t getX() const { return x; }
    uint8_t getY() const { return y; }
    void setX(uint8_t x) { this->x = x; }
    void setY(uint8_t y) { this->y = y; }
};