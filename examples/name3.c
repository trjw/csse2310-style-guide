/* A player within the game - an element within a linked list */
struct PlayerInfo {
    char* name;
    int score;
    struct PlayerInfo* next;
};

/* Point (coordinate) */
typedef struct {
    int x;
    int y;
} Point;

/* Rectangle - typedef'd structs can be named but don't have to be */
typedef struct Rectangle {
    Point upperLeft;
    Point lowerRight;
} Rectangle;
