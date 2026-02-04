/** A player within the game - an element within a linked list */
struct PlayerInfo {
    char* name; ///< Player's name
    int score; ///< Player's score
    struct PlayerInfo* next; ///< Pointer to next player in linked list
};

/**
 * @var int Point::x
 *      x coordinate of the point
 * @var int Point::y 
 *      y coordinate of the point
 */
/** Point (coordinate) 
 * @brief A point in 2D space
 */
typedef struct {
    int x;
    int y;
} Point;

/** Rectangle - typedef'd structs can be named but don't have to be */
typedef struct Rectangle {
    Point upperLeft;
    Point lowerRight;
} Rectangle;
