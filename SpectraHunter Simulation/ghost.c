#include "defs.h"


/*
    Function: initAndPlaceGhostRandomly
    Purpose: Initializes a ghost and places it in a random room in the given house.
    Params:
        Input: HouseType *house (in) - the house to place the ghost in
        Output: void
*/
void initAndPlaceGhostRandomly(HouseType *house)
{
    // Generate a random index
    int randIndex = randInt(0, house->rooms.count - 1);
    // Get the first room in the house
    RoomNodeType *currentRoomNode = house->rooms.head;

    // Move to the room at the random index
    for (int i = 0; i < randIndex && currentRoomNode != NULL; i++)
    {
        currentRoomNode = currentRoomNode->next;
    }

    // Get the room at the random index
    RoomType *randomRoom = currentRoomNode->room;

    // If the room is the "Van" room, move to the next room
    while (strcmp(randomRoom->name, "Van") == 0)
    {
        currentRoomNode = currentRoomNode->next;
        randomRoom = currentRoomNode->room;
    }

    GhostType *newGhost;
    // Create a new ghost by calling createGhost with a random ghost type and the random room
    createGhost(&newGhost, 1, randomGhost(), randomRoom);
    // Place the new ghost in the random room
    randomRoom->ghost = newGhost;
    // Log the ghost's initialization
    l_ghostInit(newGhost->ghostType, randomRoom->name);
}


/*
    Function: initGhostNode
    Purpose: Initializes a NodeType structure with a reference to a GhostType.
    Params:
        Input: GhostType *ghost (in) - a pointer to the ghost to be associated with the node
        Output: NodeType * - a pointer to the new NodeType structure
*/
NodeType *initGhostNode(GhostType *ghost)
{
    // first allocate some space in memory for the new ghost we're about to add
    NodeType *newGhost = (NodeType *)malloc(sizeof(NodeType));
    // then set the ghost data of the newGhost equal to the ghost we passed in
    newGhost->ghost = ghost;
    // set the next ghost as NULL by default
    newGhost->next = NULL;
    // then return the newGhost we just made
    return newGhost;
}


/*
    Function: createGhost
    Purpose: Creates a new GhostType structure and initializes it with the given parameters.
    Params:
        Input: 
            int id (in) - the id of the ghost
            GhostClass ghostType (in) - the type of ghost
            RoomType *room (in) - the room the ghost is in
        Output: GhostType * - a pointer to the new GhostType structure
*/
GhostType *createGhost(GhostType **ghost, int id, GhostClass ghostType, RoomType *room)
{
    // Allocate memory for a new ghost
    *ghost = (GhostType *)malloc(sizeof(GhostType));
    // Initialize the ghost's id, type, room, and boredom timer with the params we passed in
    (*ghost)->id = id;
    (*ghost)->ghostType = ghostType;
    (*ghost)->room = room;
    (*ghost)->boredomTimer = 0;
    // Return the created ghost
    return *ghost;
}


/*
    Function: moveGhostToRandomRoom
    Purpose: Moves the given ghost to a random room connected to the given room.
    Params:
        Input: 
            GhostType *ghost (in) - the ghost to move
            RoomType *currentRoom (in) - the room the ghost is currently in
        Output: void
*/
void moveGhostToRandomRoom(GhostType *ghost, RoomType *currentRoom) {
    // If the ghost and the current room are not NULL
    if (ghost != NULL && currentRoom != NULL) {
        // Get the list of rooms connected to the current room
        RoomListType *connectedRooms = currentRoom->connectedRooms;
        // Get the number of rooms connected to the current room
        int numConnectedRooms = connectedRooms->count;
        
        // If there are rooms connected to the current room
        if (numConnectedRooms > 0) {
            // Generate a random index
            int randomIndex = randInt(0, numConnectedRooms);
            // Get the first room in the list of connected rooms
            RoomNodeType *tempNode = connectedRooms->head;
            
            // Move to the room at the random index
            for (int i = 0; i < randomIndex; i++) {
                tempNode = tempNode->next;
            }

            // Get the room at the random index
            RoomType *newRoom = tempNode->room;
            // Remove the ghost from the current room
            ghost->room->ghost = NULL;
            // Move the ghost to the new room
            ghost->room = newRoom;
            // Place the ghost in the new room
            newRoom->ghost = ghost;
            // Log the ghost's move
            l_ghostMove(newRoom->name);
        }
    }
}


/*
    Function: getGhostMatch
    Purpose: Returns the ghost class that matches the given evidence.
    Params:
        Input: HouseType *house (in) - the house the ghost is in
        Output: enum GhostClass - the ghost class that matches the given evidence
*/
enum GhostClass getGhostMatch(HouseType* house) {
    // Initialize a 2D array to count the evidence for each ghost
    int evidenceCounts[GHOST_COUNT][EV_COUNT] = {0};
    // Get the count of shared evidence
    int evidenceCount = house->sharedEvidenceCount;

    // declare variables to check if each piece of evidence is present
    int isEMFPresent = 0;
    int isTemperaturePresent = 0;
    int isFingerprintsPresent = 0;
    int isSoundPresent = 0;

    // Loop for each piece of evidence and count the occurrences of each evidence for each ghost
    for (int i = 0; i < evidenceCount; i++) {
        // Loop for each ghost
        for (int j = 0; j < GHOST_COUNT; ++j) {
            // Increment the count of the current piece of evidence for the current ghost
            evidenceCounts[j][house->sharedEvidence[i]]++;
        }
    }

    // Loop through each ghost and check if each ghost type matches evidence
    for (int i = 0; i < GHOST_COUNT; i++) {
        // Check if EMF evidence is present for the current ghost
        isEMFPresent = evidenceCounts[i][EMF] >= 1 ? 1 : 0;
        // Check if Temperature evidence is present for the current ghost
        isTemperaturePresent = evidenceCounts[i][TEMPERATURE] >= 1 ? 1 : 0;
        // Check if Fingerprints evidence is present for the current ghost
        isFingerprintsPresent = evidenceCounts[i][FINGERPRINTS] >= 1 ? 1 : 0;
        // Check if Sound evidence is present for the current ghost
        isSoundPresent = evidenceCounts[i][SOUND] >= 1 ? 1 : 0;   
    }

    // Check the combination of evidences and return the corresponding ghost type
    if (isEMFPresent != 0 && isTemperaturePresent != 0 && isFingerprintsPresent != 0) {
        // If EMF, Temperature, and Fingerprints are present, it's ghost type 0
        return 0;
    } else if (isEMFPresent != 0 && isTemperaturePresent != 0 && isSoundPresent != 0) {
        // If EMF, Temperature, and Sound are present, it's ghost type 1
        return 1;
    } else if (isEMFPresent != 0 && isFingerprintsPresent != 0 && isSoundPresent != 0) {
        // If EMF, Fingerprints, and Sound are present, it's ghost type 2
        return 2;
    } else if (isTemperaturePresent != 0 && isFingerprintsPresent != 0 && isSoundPresent != 0) {
        // If Temperature, Fingerprints, and Sound are present, it's ghost type 3
        return 3;
    }

    // If no ghost class matches the given evidence, return unknown
    return GH_UNKNOWN;
}