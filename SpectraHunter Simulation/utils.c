#include "defs.h"

/*
    Returns a pseudo randomly generated number, in the range min to (max - 1), inclusively
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated integer in the range [0, max-1) 
*/
int randInt(int min, int max)
{
    // Convert the result of randFloat to an integer and return it
    return (int) randFloat(min, max);
}


/*
    Returns a pseudo randomly generated floating point number.
    A few tricks to make this thread safe, just to reduce any chance of issues using random
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated floating point number in the range [min, max)
*/
float randFloat(float min, float max) {
    // Initialize seed for random number generation
    static __thread unsigned int seed = 0;
    if (seed == 0) {
        // Use current time and thread ID to seed the random number generator
        seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
    }

    // Generate a random float between 0 and 1
    float random = ((float) rand_r(&seed)) / (float) RAND_MAX;
    // Calculate the range of the random number
    float diff = max - min;
    // Scale the random number by the range
    float r = random * diff;
    // Shift the scaled number into the desired range and return it
    return min + r;
}


/* 
    Returns a random enum GhostClass.
*/
enum GhostClass randomGhost() {
    // Generate a random integer in the range [0, GHOST_COUNT) and cast it to GhostClass
    return (enum GhostClass) randInt(0, GHOST_COUNT);
}


/*
    Returns the string representation of the given enum EvidenceType.
        in: type - the enum EvidenceType to convert
        out: str - the string representation of the given enum EvidenceType, minimum 16 characters
*/
void evidenceToString(enum EvidenceType type, char* str) {
    // Use a switch statement to convert the enum to a string
    switch (type) {
        case EMF:
            // If the type is EMF, copy the string "EMF" to str
            strcpy(str, "EMF");
            break;
        case TEMPERATURE:
            // If the type is TEMPERATURE, copy the string "TEMPERATURE" to str
            strcpy(str, "TEMPERATURE");
            break;
        case FINGERPRINTS:
            // If the type is FINGERPRINTS, copy the string "FINGERPRINTS" to str
            strcpy(str, "FINGERPRINTS");
            break;
        case SOUND:
            // If the type is SOUND, copy the string "SOUND" to str
            strcpy(str, "SOUND");
            break;
        default:
            // If the type is not recognized, copy the string "UNKNOWN" to str
            strcpy(str, "UNKNOWN");
            break;
    }
}


/* 
    Returns the string representation of the given enum GhostClass.
        in: ghost - the enum GhostClass to convert
        out: buffer - the string representation of the given enum GhostClass, minimum 16 characters
*/
void ghostToString(enum GhostClass ghost, char* buffer) {
    // Use a switch statement to convert the enum to a string
    switch(ghost) {
        case BANSHEE:
            // If the ghost class is BANSHEE, copy the string "Banshee" to buffer
            strcpy(buffer, "Banshee");
            break;
        case BULLIES:
            // If the ghost class is BULLIES, copy the string "Bullies" to buffer
            strcpy(buffer, "Bullies");
            break;
        case PHANTOM:
            // If the ghost class is PHANTOM, copy the string "Phantom" to buffer
            strcpy(buffer, "Phantom");
            break;
        case POLTERGEIST:
            // If the ghost class is POLTERGEIST, copy the string "Poltergeist" to buffer
            strcpy(buffer, "Poltergeist");
            break;
        default:
            // If the ghost class is not recognized, copy the string "Unknown" to buffer
            strcpy(buffer, "Unknown");
            break;
    }
}


/*
    Function: isHunterPresent
    Purpose: Checks if there is a hunter in the given room.
    Params:
        Input: 
            RoomType *room (in) - the room to check
        Output: int - 1 if there is a hunter in the room, 0 otherwise
*/
int isHunterPresent(RoomType *room) {
    // Check if the count of hunters in the room is greater than 0
    // If it is, return 1, otherwise return 0
    return room->hunters.count > 0;
}