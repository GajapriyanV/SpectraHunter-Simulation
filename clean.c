#include "defs.h"


/*
    Function: cleanHunterList
    Purpose: Cleans the list of hunters in a room.
    Params:
        Input: HunterListType *list (in) - the list of hunters to clean
        Output: void
*/
void cleanHunterList(HunterListType *list) {
    // Initialize pointers to the current and next nodes
    HunterNodeType *current = list->head;
    HunterNodeType *next;

    // Loop through the list
    while (current != NULL) {
        next = current->next;
        // Clean the hunter
        free(current->hunter);
        // Free the current node
        free(current);
        current = next;
    }
}


/*
    Function: cleanRoom
    Purpose: Cleans a given room
    Params:
        Input: RoomType *room (in) - the room to clean
        Output: void
*/
void cleanRoom(RoomType *room) {
    // Clean the evidence in the room
    cleanEvidenceList(&(room->evidences));
    // Free the connected rooms list
    cleanRoomList(room->connectedRooms);
    // Clean the hunters in the room
    cleanHunterList(&(room->hunters));
    // Unlock the semaphore before freeing the room
    sem_post(&(room->roomSem));
    // Finally, free the room itself
    free(room);
}


/*
    Function: cleanRoomList
    Purpose: Cleans the list of rooms in a house.
    Params:
        Input: RoomListType *list (in) - the list of rooms to clean
        Output: void
*/
void cleanRoomList(RoomListType *list) {
    // Initialize pointers to the current and next nodes
    RoomNodeType *current = list->head;
    RoomNodeType *next;

    // Loop through the list
    while (current != NULL) {
        next = current->next;
        // Free the current node
        free(current);
        current = next;
    }

    // Finally, free the list itself
    free(list);
}


/*
    Function: cleanEvidenceList
    Purpose: Cleans the list of evidence in a room.
    Params:
        Input: EvidenceListType *list (in) - the list of evidence to clean
        Output: void
*/
void cleanEvidenceList(EvidenceListType *list) {
    // Initialize pointers to the current and next nodes
    Evidence *current = list->head;
    Evidence *next;

    // Loop through the list
    while (current != NULL) {
        next = current->next;
        // Assuming semaphores need to be destroyed before freeing evidence
        sem_destroy(&(current->evidenceSem));
        free(current);
        current = next;
    }
}


/*
    Function: cleanHouse
    Purpose: Cleans the overall house
    Params:
        Input: HouseType *house (in) - the house to clean
        Output: void
*/
void cleanHouse(HouseType *house) {
    // Get the ghost in the house
    GhostType *ghost = getGhostInHouse(house);

    // Clean the rooms in the house
    RoomNodeType *current = house->rooms.head;
    while (current != NULL) {
        RoomNodeType *next = current->next;
        cleanRoom(current->room);
        free(current);
        current = next;
    }

    // Clean the hunters who exited due to fear
    for (int i = 0; i < house->huntersExitedFear->size; i++) {
        free(house->huntersExitedFear->hunters[i]);
    }
    // Clean the hunters who exited due to boredom
    for (int i = 0; i < house->huntersExitedBoredom->size; i++) {
        free(house->huntersExitedBoredom->hunters[i]);
    }

    // free the ghost in the house
    free(ghost);

    // Destroy the semaphore
    sem_destroy(&(house->evidenceSem));
}