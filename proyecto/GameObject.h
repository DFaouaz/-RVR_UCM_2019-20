#include "Serializable.h"

class GameObject : public Serializable
{
public:
    GameObject() {};
    virtual ~GameObject() {};

public:
    virtual void render();
    virtual void update(float deltaTime);

    virtual void to_bin() {};
    virtual int from_bin(char * data) {};
};