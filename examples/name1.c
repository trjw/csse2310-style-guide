int count, wordLength, cursor, fileHandle;
char* htmlMethodName; // Note - lowercase even for a normally capitalised
                      // acryonym
struct Book book; // struct Book is the type name, book is the variable name
const char* courseName = "CSSE2310"; // constants follow the same convention

/// Type to represent a person - storing their name and home address
struct Person {
    char* name; // struct members have camelCase names
    char* homeAddress;
};
