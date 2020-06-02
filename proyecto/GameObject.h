class GameObject
{
public:
    GameObject() {};
    virtual ~GameObject() {};

public:
    virtual void render() = 0;
    virtual void update(float deltaTime) = 0;
}