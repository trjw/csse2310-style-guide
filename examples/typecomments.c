/// Type used to represent Student IDs
/// @ai Not Used
typedef long StudentId;

/// Coordinates of a two-dimensional point
/// @ai Wrote Code
/// @aidetails Chat GPT wrote this code and I modified the comments to be 
///  doxygen compliant
typedef struct Point {
    /// X coordinate of the point
    double x; 
    /// Y coordinate of the point
    double y;
} Point;

/// Representation of a rectangle
/// @ai Not Used
struct Rectangle {
    Point topLeft; ///< Coordinates of the top left corner
    Point bottomRight; ///< Coordinators of the bottom right corner
};

/// Exit statuses of the program
/// @ai Not Used
enum ExitCodes {
    EXIT_OK = 0, ///< Exit status on successful completion
    EXIT_USAGE = 1 ///< Exit status on usage error
};
/// @brief Convenient alias for enum ExitCodes
/// @ai Not Used
typedef enum ExitCodes ExitCodes;
