#include "defs.h"


/*
    Function: initHunterAndPlaceInVan
    Purpose: Initializes the hunters and places them in the van room.
    Params:
        Input: 
            RoomType *vanRoom (in) - the van room to place the hunters in
            char hunterNames[][MAX_STR] (in) - the names of the hunters
            int numHunters (in) - the number of hunters
        Output: void
*/
void initHunterAndPlaceInVan(RoomType *vanRoom, char *name)
{
    // For each hunter, we create a new HunterType object, add it to the van room's list of hunters,
    // and initialize the hunter's log with their name and equipment type.
    HunterType *newHunter = createHunter(name, vanRoom);
    insertAtHeadHunter(&(vanRoom->hunters), newHunter);
    l_hunterInit(newHunter->name, newHunter->equipmentType);
}


/*
    Function: createHunter
    Purpose: Creates a new hunter with a given name and places them in a room.
    Params:
        Input: 
            char *name (in) - the name of the hunter
            RoomType *room (in) - the room to place the hunter in
        Output: HunterType* - the newly created hunter
*/
HunterType *createHunter(char *name, RoomType *room)
{
    HunterType *newHunter = (HunterType *)malloc(sizeof(HunterType)); // Allocate memory for a new HunterType
    newHunter->id = randInt(1, 100); // Assign a random ID to the new hunter
    strcpy(newHunter->name, name); // Set the hunter's name
    newHunter->equipmentType = randInt(0, EV_COUNT); // Assign random equipment type to the hunter
    newHunter->fear = 0; // Initialize fear level to 0
    newHunter->boredomTimer = 0; // Initialize boredom timer to 0
    newHunter->currentRoom = room; // Set the current room of the new hunter
    return newHunter; // Return the newly created hunter
}


/*
    Function: insertAtHeadHunter
    Purpose: Inserts a new hunter at the head of the hunter list.
    Params:
        Input: 
            HunterListType *list (in) - the list to insert the hunter into
            HunterType *newHunter (in) - the hunter to insert
        Output: void
*/
void insertAtHeadHunter(HunterListType *list, HunterType *newHunter)
{
    HunterNodeType *newNode = (HunterNodeType *)malloc(sizeof(HunterNodeType)); // Allocate memory for a new HunterNodeType
    newNode->hunter = newHunter; // Set the hunter for the new node
    newNode->next = NULL; // Set the next of the new node to NULL

    if (list->head == NULL) // If the list is empty
    {
        list->head = newNode; // Set the new node as head
        list->tail = newNode; // Set the new node as tail
    }
    else
    {
        newNode->next = list->head; // Set the next of the new node to the current head
        list->head = newNode; // Update the head to the new node
    }
    list->count++; // Increment the count of hunters in the list
}


/*
    Function: collectEvidence
    Purpose: Collects evidence from the current room if the hunter's equipment matches the evidence type.
    Params:
        Input: 
            HouseType *house (in) - the house where the hunter is
            HunterType *hunter (in) - the hunter who is collecting the evidence
            RoomType *currentRoom (in) - the room where the hunter is
        Output: void
*/
void collectEvidence(HouseType *house, HunterType *hunter, RoomType *currentRoom) {
    // Get the first evidence in the room
    Evidence *currentEvidence = currentRoom->evidences.head;
    // Get the type of equipment the hunter has
    enum EvidenceType hunterEquipment = hunter->equipmentType;

    // Loop through all evidence in the room
    while (currentEvidence != NULL) {
        // If the evidence type matches the hunter's equipment type
        if (currentEvidence->etype == hunterEquipment) {
            // Add the evidence to the house's shared evidence
            house->sharedEvidence[house->sharedEvidenceCount++] = currentEvidence->etype;
            // Log the collection of the evidence
            l_hunterCollect(hunter->name, currentEvidence->etype, currentRoom->name);
            // Remove the evidence from the room
            removeEvidence(&(currentRoom->evidences), currentEvidence->etype);
            // Stop looking for more evidence
            break;
        }
        // Move to the next evidence in the room
        currentEvidence = currentEvidence->next;
    }
}


/*
    Function: removeHunterFromRoom
    Purpose: Removes a hunter from a room.
    Params:
        Input: 
            RoomType *room (in) - the room to remove the hunter from
            HunterType *hunter (in) - the hunter to remove
        Output: void
*/
void removeHunterFromRoom(RoomType *room, HunterType *hunter) {
    // Get the first hunter in the room
    HunterNodeType *currentHunterNode = room->hunters.head;
    // Initialize the previous hunter node to NULL
    HunterNodeType *prevHunterNode = NULL;

    // Loop through all hunters in the room
    while (currentHunterNode != NULL) {
        // If the current hunter is the one to remove
        if (currentHunterNode->hunter == hunter) {
            // If there is a previous hunter node
            if (prevHunterNode != NULL) {
                // Set the next pointer of the previous hunter node to the next hunter node
                prevHunterNode->next = currentHunterNode->next;
            } else {
                // Set the head of the hunters list to the next hunter node
                room->hunters.head = currentHunterNode->next;
            }

            // If the current hunter node is the tail of the hunters list
            if (currentHunterNode == room->hunters.tail) {
                // Set the tail of the hunters list to the previous hunter node
                room->hunters.tail = prevHunterNode;
            }

            // Free the memory of the current hunter node
            free(currentHunterNode);            
            // Decrement the count of hunters in the room
            room->hunters.count--;
            // Stop looking for more hunters
            break;
        }

        // Move to the next hunter in the room
        prevHunterNode = currentHunterNode;
        currentHunterNode = currentHunterNode->next;
    }
}


/*
    Function: moveHunterToRandomRoom
    Purpose: Moves a hunter to a random connected room.
    Params:
        Input: 
            HunterType *hunter (in) - the hunter to move
            RoomType *currentRoom (in) - the current room of the hunter
        Output: void
*/
void moveHunterToRandomRoom(HunterType *hunter, RoomType *currentRoom) {
    // Get the list of rooms connected to the current room
    RoomListType *connectedRooms = currentRoom->connectedRooms;
    // Get the number of rooms connected to the current room
    int numConnectedRooms = connectedRooms->count;
    
    // If there are any connected rooms
    if (numConnectedRooms > 0) {
        // Generate a random index
        int randomIndex = randInt(0, numConnectedRooms);

        // Get the room at the random index
        RoomNodeType *tempNode = connectedRooms->head;
        for (int i = 0; i < randomIndex; i++) {
            tempNode = tempNode->next;
        }

        // Get the new room for the hunter
        RoomType *newRoom = tempNode->room;
        // Set the current room of the hunter to the new room
        hunter->currentRoom = newRoom;
        
        // Add the hunter to the new room
        insertAtHeadHunter(&(newRoom->hunters), hunter);        
        // Remove the hunter from the current room
        removeHunterFromRoom(currentRoom, hunter);
        // Log the movement of the hunter
        l_hunterMove(hunter->name, newRoom->name);
    }
}


/*
    Function: findHunterByName
    Purpose: Finds a hunter in the house by their name.
    Params:
        Input: 
            const HouseType *house (in) - the house to search for the hunter
            const char *name (in) - the name of the hunter to find
        Output: HunterType* - the found hunter, or NULL if no hunter is found
*/
HunterType *findHunterByName(const HouseType *house, const char *name) {
    // Get the first room in the house
    RoomNodeType *currentRoomNode = house->rooms.head;
    // Loop through all rooms in the house
    while (currentRoomNode != NULL) {
        // Get the first hunter in the current room
        HunterNodeType *currentHunterNode = currentRoomNode->room->hunters.head;
        // Loop through all hunters in the current room
        while (currentHunterNode != NULL) {
            // If the name of the current hunter matches the provided name
            if (strcmp(currentHunterNode->hunter->name, name) == 0) {
                // Hunter found, return the pointer to the hunter struct
                return currentHunterNode->hunter;
            }
            // Move to the next hunter in the room
            currentHunterNode = currentHunterNode->next;
        }
        // Move to the next room in the house
        currentRoomNode = currentRoomNode->next;
    }
    // If hunter not found, return null
    return NULL;
}


/*
    Function: findHunterByName
    Purpose: Finds a hunter in the house by their name.
    Params:
        Input: 
            const HouseType *house (in) - the house to search for the hunter
            const char *name (in) - the name of the hunter to find
        Output: HunterType* - the found hunter, or NULL if no hunter is found
*/
void removeHunterFromHouse(HouseType *house, HunterType *hunter) {
    // Get the first room in the house
    RoomNodeType *currentRoomNode = house->rooms.head;
    // Loop through all rooms in the house
    while (currentRoomNode != NULL) {
        // Get the current room
        RoomType *currentRoom = currentRoomNode->room;
        // Remove the hunter from the current room
        removeHunterFromRoom(currentRoom, hunter);
        // Move to the next room in the house
        currentRoomNode = currentRoomNode->next;
    }
}