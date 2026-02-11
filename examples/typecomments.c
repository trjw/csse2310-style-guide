/// Type used to represent Student IDs
typedef long StudentId;

/// Coordinates of a two-dimensional point
typedef struct Point {
    /// X coordinate of the point
    double x; 
    /// Y coordinate of the point
    double y;
} Point;

/// Representation of a rectangle
struct Rectangle {
    Point topLeft; ///< Coordinates of the top left corner
    Point bottomRight; ///< Coordinators of the bottom right corner
};

/// Exit statuses of the program
enum ExitCodes {
    EXIT_OK = 0, ///< Exit status on successful completion
    EXIT_USAGE = 1 ///< Exit status on usage error
};
/// Convenient alias for enum ExitCodes
typedef enum ExitCodes ExitCodes;
