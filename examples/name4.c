/* Card Suits */
enum CardSuits { CLUBS, DIAMONDS, HEARTS, SPADES };

// Program exit codes
enum ExitCodes { EXIT_SUCCESS = 0, EXIT_ARGS = 1, EXIT_FAILURE = 2 };

// Program exit codes - the definitions don't all fit on one line so they must
// be set out as one per line
enum ExtendedExitCodes {
    EXIT_SUCCESS = 0,
    EXIT_ARGS = 1,
    EXIT_FAILURE = 2,
    EXIT_FILE_NONEXISTENT = 3
};
