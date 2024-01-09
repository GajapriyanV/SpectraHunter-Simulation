#include "defs.h"


/*
    Dynamically allocates several rooms and populates the provided house.
    Note: You may modify this as long as room names and connections are maintained.
        out: house - the house to populate with rooms. Assumes house has been initialized.
*/
void populateRooms(HouseType *house)
{
    // First, create each room

    // createRoom assumes that we dynamically allocate a room, initializes the values, and returns a RoomType*
    // create functions are pretty typical, but it means errors are harder to return aside from NULL
    struct Room *van = createRoom("Van");
    struct Room *hallway = createRoom("Hallway");
    struct Room *master_bedroom = createRoom("Master Bedroom");
    struct Room *boys_bedroom = createRoom("Boy's Bedroom");
    struct Room *bathroom = createRoom("Bathroom");
    struct Room *basement = createRoom("Basement");
    struct Room *basement_hallway = createRoom("Basement Hallway");
    struct Room *right_storage_room = createRoom("Right Storage Room");
    struct Room *left_storage_room = createRoom("Left Storage Room");
    struct Room *kitchen = createRoom("Kitchen");
    struct Room *living_room = createRoom("Living Room");
    struct Room *garage = createRoom("Garage");
    struct Room *utility_room = createRoom("Utility Room");

    // This adds each room to each other's room lists
    // All rooms are two-way connections
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    // Add each room to the house's room list
    addRoom(&house->rooms, van);
    addRoom(&house->rooms, hallway);
    addRoom(&house->rooms, master_bedroom);
    addRoom(&house->rooms, boys_bedroom);
    addRoom(&house->rooms, bathroom);
    addRoom(&house->rooms, basement);
    addRoom(&house->rooms, basement_hallway);
    addRoom(&house->rooms, right_storage_room);
    addRoom(&house->rooms, left_storage_room);
    addRoom(&house->rooms, kitchen);
    addRoom(&house->rooms, living_room);
    addRoom(&house->rooms, garage);
    addRoom(&house->rooms, utility_room);
}


/*
    Function: initHouse
    Purpose: Initializes the house with empty rooms and no shared evidence.
    Params:
        Input: HouseType *house (in) - the house to initialize
        Output: void
*/
void initHouse(HouseType *house)
{
    // Initialize the head and tail of the rooms list to NULL
    house->rooms.head = NULL;
    house->rooms.tail = NULL;
    house->rooms.count = 0; // Initialize the count of rooms to 0
    house->sharedEvidenceCount = 0; // Initialize the count of shared evidence to 0
    // Initialize the size of the lists of hunters who exited due to fear and boredom to 0
    house->huntersExitedFear->size = 0; 
    house->huntersExitedBoredom->size = 0;
    // Initialize the semaphore for evidence with an initial value of 1
    sem_init(&(house->evidenceSem), 0, 1);
}


/*
    Function: getGhostInHouse
    Purpose: Returns the first ghost found in the house.
    Params:
        Input: HouseType* house (in) - the house to search for the ghost
        Output: GhostType* - the first ghost found in the house, or NULL if no ghost is found
*/
GhostType *getGhostInHouse(HouseType* house) {
    // Initialize the ghost pointer to NULL
    GhostType *ghost = NULL;
    // Get the first room in the house
    RoomNodeType *currentRoomNode = house->rooms.head;
    // Loop through all rooms in the house
    while (currentRoomNode != NULL) {
        // If the current room has a ghost
        if (currentRoomNode->room->ghost != NULL) {
            // Set the ghost pointer to the ghost in the current room
            ghost = currentRoomNode->room->ghost;
            // Stop looking for more ghosts
            break;
        }
        // Move to the next room in the house
        currentRoomNode = currentRoomNode->next;
    }
    // Return the found ghost, or NULL if no ghost was found
    return ghost;
}