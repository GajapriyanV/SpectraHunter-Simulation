#include "defs.h"


/*
    Function: createRoom
    Purpose: Creates a new room with a given name.
    Params:
        Input: char *name (in) - the name of the room
        Output: RoomType* - the newly created room
*/
RoomType *createRoom(char *name)
{
    // Allocate memory for a new room
    RoomType *newRoom = (RoomType *)malloc(sizeof(RoomType));
    // Copy the provided name into the new room's name
    strcpy(newRoom->name, name);
    // Initialize the ghost in the room to NULL
    newRoom->ghost = NULL;
    // Initialize the head and tail of the evidence list to NULL
    newRoom->evidences.head = NULL;
    newRoom->evidences.tail = NULL;
    // Initialize the head, tail, and count of the hunters list to NULL and 0 respectively
    newRoom->hunters.head = NULL;
    newRoom->hunters.tail = NULL;
    newRoom->hunters.count = 0;

    // Allocate memory for the list of connected rooms
    newRoom->connectedRooms = (RoomListType *)malloc(sizeof(RoomListType));
    // Initialize the head, tail, and count of the connected rooms list to NULL and 0 respectively
    newRoom->connectedRooms->head = NULL;
    newRoom->connectedRooms->tail = NULL;
    newRoom->connectedRooms->count = 0;

    // Initialize the semaphore for the room
    sem_init(&(newRoom->roomSem), 0, 1);

    // Return the newly created room
    return newRoom;
}


/*
    Function: connectRooms
    Purpose: Connects two rooms together.
    Params:
        Input: 
            RoomType *room1 (in) - the first room to connect
            RoomType *room2 (in) - the second room to connect
        Output: void
*/
void connectRooms(RoomType *room1, RoomType *room2) {
    // Allocate memory for the nodes to add to each room's connected rooms list
    RoomNodeType *roomNode1 = (RoomNodeType*)malloc(sizeof(RoomNodeType));
    RoomNodeType *roomNode2 = (RoomNodeType*)malloc(sizeof(RoomNodeType));
    // Set the room of each node to the other room
    roomNode1->room = room1;
    roomNode2->room = room2;
    // Initialize the next pointer of each node to NULL
    roomNode1->next = NULL;
    roomNode2->next = NULL;

    // Add each room to the other's connected rooms list
    addRoom(room1->connectedRooms, roomNode2->room);
    addRoom(room2->connectedRooms, roomNode1->room);

    // Free the temp nodes used
    free(roomNode1);
    free(roomNode2);
}


/*
    Function: addRoom
    Purpose: Adds a room to a room list.
    Params:
        Input: 
            RoomListType *list (in) - the list to add the room to
            RoomType *r (in) - the room to add
        Output: void
*/
void addRoom(RoomListType *list, RoomType *r)
{
    // Allocate memory for a new node
    RoomNodeType *newNode = (RoomNodeType *)malloc(sizeof(RoomNodeType));
    // Set the room of the new node to the provided room
    newNode->room = r;
    // Initialize the next pointer of the new node to NULL
    newNode->next = NULL;

    // If the list is empty
    if (list->head == NULL)
    {
        // Set the head and tail of the list to the new node
        list->head = newNode;
        list->tail = newNode;
    }
    else
    {
        // Insert the new node at the end of the list
        list->tail->next = newNode;
        list->tail = newNode;
    }
    // Increment the count of rooms in the list
    list->count++;
}