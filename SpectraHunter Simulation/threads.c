#include "defs.h"


/*
    Function: ghostThread
    Purpose: Represents the thread for the ghost's actions in the house.
    Params:
        Input: void *arg (in) - the house where the ghost is
        Output: void
*/
void *ghostThread(void *arg){
    // Cast the argument to HouseType
    HouseType *house = (HouseType *)arg;

    // Infinite loop for the ghost's actions
    while (C_TRUE) {
        // Perform the ghost's action, passing in the house
        performGhostAction(house);
        // Sleep for a while before the next action
        usleep(10000);
    }
}


/*
    Function: hunterThread
    Purpose: Represents the thread for a hunter's actions in the house.
    Params:
        Input: void *arg (in) - the thread data containing the house and the hunter
        Output: void
*/
void *hunterThread(void *arg){
    // Cast the argument to ThreadDataType
    ThreadDataType *threadData = (ThreadDataType *)arg;
    // Get the house and the hunter from the thread data
    HouseType *house = threadData->house;
    HunterType *hunter = threadData->hunter;
    // Free the memory allocated for thread data
    free(threadData);

    // Infinite loop for the hunter's actions
    while (C_TRUE) {
        // Perform the hunter's action, passing in the house and the hunter
        performHunterAction(house, hunter);
        // Sleep for a while before the next action
        usleep(10000);
    }
}


/*
    Function: performGhostAction
    Purpose: Performs all of the simulation actions for the ghost in the house.
    Params:
        Input: HouseType *house (in) - the house where the ghost is
        Output: void
*/
void performGhostAction(HouseType *house) {
    // Get the first room in the house
    RoomNodeType *currentRoomNode = house->rooms.head;
    // Loop through all rooms
    while (currentRoomNode != NULL) {
        RoomType *currentRoom = currentRoomNode->room;

        // Wait for the semaphore to be available
        sem_wait(&(currentRoom->roomSem));

        // If there's a ghost in the room
        if (currentRoom->ghost != NULL) {
            GhostType *currentGhost = currentRoom->ghost;
            // Increase the ghost's boredom timer
            currentGhost->boredomTimer++;

            // If there's a hunter in the room
            if (isHunterPresent(currentRoom) == C_TRUE) {
                // Reset the ghost's boredom timer
                currentGhost->boredomTimer = 0;
                int action = randInt(0, 1);
                // If the action is 0, leave evidence
                if (action == 0) {
                    enum EvidenceType evidence = randomEvidence(currentGhost->ghostType);
                    insertAtHeadEvidence(&(currentRoom->evidences), evidence);
                    l_ghostEvidence(evidence, currentRoom->name);
                }
            } else {
                int action = randInt(0, 2);
                // If the action is 0, move the ghost to a random room
                if (action == 0) {
                    moveGhostToRandomRoom(currentGhost, currentRoom);
                    l_ghostMove(currentRoom->name);
                } else if (action == 1) {
                    // If the action is 1, leave evidence
                    enum EvidenceType evidence = randomEvidence(currentGhost->ghostType);
                    insertAtHeadEvidence(&(currentRoom->evidences), evidence);
                    l_ghostEvidence(evidence, currentRoom->name);
                }
            }

            // If the ghost's boredom timer reaches the maximum
            if (currentGhost->boredomTimer >= BOREDOM_MAX) {
                l_ghostExit(LOG_BORED);
                // Release the semaphore before exiting the thread
                sem_post(&(currentRoom->roomSem));
                pthread_exit(NULL);
            }
        }

        // Release the semaphore
        sem_post(&(currentRoom->roomSem));
        // Move to the next room
        currentRoomNode = currentRoomNode->next;
    }
}


/*
    Function: performHunterAction
    Purpose: Performs all of the simulation actions for a given hunter in the house.
    Params:
        Input: 
            HouseType *house (in) - the house where the hunter is
            HunterType *hunter (in) - the hunter to perform the action
        Output: void
*/
void performHunterAction(HouseType *house, HunterType *hunter) {
    // Get the current room of the hunter
    RoomType *currentRoom = hunter->currentRoom;

    // Lock the room to prevent other threads from accessing it
    sem_wait(&(currentRoom->roomSem));

    // If there is a ghost in the room and the ghost's room is the current room
    if (currentRoom->ghost != NULL && currentRoom->ghost->room == currentRoom) {
        // Increase the hunter's fear
        hunter->fear += 1;
        // Reset the hunter's boredom timer
        hunter->boredomTimer = 0;
    } else {
        // Increase the hunter's boredom timer
        hunter->boredomTimer += 1;
    }

    // Generate a random action for the hunter
    int action = randInt(0, 3);
    // If the action is 0
    if (action == 0) {
        // Collect evidence in the room
        collectEvidence(house, hunter, currentRoom);
    } else if (action == 1) {
        // Move the hunter to a random room
        moveHunterToRandomRoom(hunter, currentRoom);
    } else {
        // Review the evidence in the house
        int uniqueEvidenceCount = reviewEvidence(house);
        // If there are at least 3 unique pieces of evidence
        if (uniqueEvidenceCount >= 3) {
            // Log the hunter's exit due to evidence
            l_hunterExit(hunter->name, LOG_EVIDENCE);
            // Remove the hunter from the house
            removeHunterFromHouse(house, hunter);
            // Free the memory allocated for the hunter
            free(hunter);
            // Unlock the room before exiting the thread
            sem_post(&(currentRoom->roomSem));
            pthread_exit(NULL);
        }   
    }

    // If the hunter's fear or boredom reaches the maximum
    if (hunter->fear >= FEAR_MAX || hunter->boredomTimer >= BOREDOM_MAX) {
        // If the hunter's fear reaches the maximum
        if (hunter->fear >= FEAR_MAX) {
            // Add the hunter to the list of hunters who exited due to fear
            house->huntersExitedFear->hunters[house->huntersExitedFear->size++] = hunter;
            // Log the hunter's exit due to fear
            l_hunterExit(hunter->name, LOG_FEAR);
        } else {
            // Add the hunter to the list of hunters who exited due to boredom
            house->huntersExitedBoredom->hunters[house->huntersExitedBoredom->size++] = hunter;
            // Log the hunter's exit due to boredom
            l_hunterExit(hunter->name, LOG_BORED);
        }
        // Remove the hunter from the house
        removeHunterFromHouse(house, hunter);
        // Unlock the room before exiting the thread
        sem_post(&(currentRoom->roomSem));
        pthread_exit(NULL);
    }

    // Unlock the room
    sem_post(&(currentRoom->roomSem));
}


/*
    Function: createThreads
    Purpose: Creates threads for each hunter and the ghost in the house.
    Params:
        Input: 
            HouseType *house (in) - the house where the hunters and the ghost are
            char hunterNames[][MAX_STR] (in) - the names of the hunters
            int numHunters (in) - the number of hunters
        Output: void
*/
void createThreads(HouseType *house, char hunterNames[][MAX_STR], int numHunters) {
    // Create an array of pthread_t for the hunter threads
    pthread_t hunterThreads[numHunters];
    // Create a pthread_t for the ghost thread
    pthread_t ghostPThread;

    // Loop through all hunters
    for (int i = 0; i < numHunters; i++) {
        // Find the current hunter by name
        HunterType *currentHunter = findHunterByName(house, hunterNames[i]);
        // Allocate memory for the thread data
        ThreadDataType *threadData = malloc(sizeof(ThreadDataType));
        // Set the house and hunter in the thread data
        threadData->house = house;
        threadData->hunter = currentHunter;
        // Create a new thread for the current hunter
        pthread_create(&hunterThreads[i], NULL, hunterThread, (void *)threadData);
    }

    // Create a new thread for the ghost
    pthread_create(&ghostPThread, NULL, ghostThread, (void *)house);

    // Wait for all hunter threads to finish
    for (int i = 0; i < NUM_HUNTERS; i++) {
        pthread_join(hunterThreads[i], NULL);
    }

    // Wait for the ghost thread to finish
    pthread_join(ghostPThread, NULL);
}