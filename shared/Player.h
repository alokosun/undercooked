#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    int id;
    float x;
    float y;

public:
    Player(int playerId, float startX, float startY);

    int getId() const;

    float getX() const;
    float getY() const;

    void setPosition(float newX, float newY);
    void move(float dx, float dy);
};

#endif