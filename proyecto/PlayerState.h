#include <SFML/Graphics.hpp>

class PlayerState
{
public:
    PlayerState();
    ~PlayerState();

    int index;
    int xDirection;
    int yDirection;

    float xAim;
    float yAim;

    bool shooting;

    bool handleEvent(sf::Event &event);

    bool operator==(const PlayerState &other) const
    {
        return index == other.index &&
               xDirection == other.xDirection &&
               yDirection == other.yDirection &&
               xAim == other.xAim &&
               yAim == other.yAim &&
               shooting == other.shooting;
    }
};
