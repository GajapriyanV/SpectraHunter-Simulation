#include "defs.h"


/*
    Function: printHouse
    Purpose: Prints the results of the ghost hunt, including the hunters who exited due to fear or boredom, the winner, and the collected evidence.
    Params:
        Input: HouseType *house (in) - the house where the ghost hunt took place
        Output: void
*/
void printHouse(HouseType *house) {
    // Print a header for the results section
    printf("\n=== Results ===\n");

    // Print a header for the list of hunters who exited due to high fear
    printf("\nHunters with fear >= FEAR_MAX:\n");
    // If there are no hunters who exited due to high fear, print "None"
    if (house->huntersExitedFear->size == 0) {
        printf("None\n");
    } else {
        // Otherwise, iterate over the list of hunters who exited due to high fear
        for (int i = 0; i < house->huntersExitedFear->size; i++) {
            // Print the name of each hunter who exited due to high fear
            printf("[%s]\n", house->huntersExitedFear->hunters[i]->name);
        }
    }

    // Print a header for the list of hunters who exited due to high boredom
    printf("\nHunters with boredom >= BOREDOM_MAX:\n");
    // If there are no hunters who exited due to high boredom, print "None"
    if (house->huntersExitedBoredom->size == 0) {
        printf("None\n");
    } else {
        // Otherwise, iterate over the list of hunters who exited due to high boredom
        for (int i = 0; i < house->huntersExitedBoredom->size; i++) {
            // Print the name of each hunter who exited due to high boredom
            printf("[%s]\n", house->huntersExitedBoredom->hunters[i]->name);
        }
    }

    // If the total number of hunters who exited due to high fear or boredom is less than the total number of hunters
    if (house->huntersExitedBoredom->size + house->huntersExitedFear->size < NUM_HUNTERS) {
        // Print a message indicating that the hunters have won
        printf("\nThe hunters have won! The ghost has been correctly identified!\n");
    } else {
        // Otherwise, print a message indicating that the ghost has won
        printf("\nThe ghost has won! All hunters have exited because high of fear or boredom.\n");
    }

     // Print the header for the collected evidence section
    printf("\nCollected Evidence:\n");
    // Iterate over the shared evidence collected by the hunters
    for (int i = 0; i < house->sharedEvidenceCount; i++) {
        // Convert the evidence to a string representation
        char evidenceStr[16]; 
        evidenceToString(house->sharedEvidence[i], evidenceStr);
        // Print the string representation of the evidence
        printf("[%s]\n", evidenceStr);
    }

    // Initialize a string to hold the matching ghost type
    char ghostMatchStr[16] = "Unknown"; 
    // If there are at least three different pieces of evidence
    if (reviewEvidence(house) >= 3) {
        // Determine the matching ghost type based on the evidence
        enum GhostClass ghost = getGhostMatch(house);
        // Convert the matching ghost type to a string representation and print it
        ghostToString(ghost, ghostMatchStr);
        printf("\nMatching Ghost Type: %s\n", ghostMatchStr);
    } else {
        // If there are not at least three different pieces of evidence, print a message indicating that the evidence is insufficient
        printf("\nInsufficient evidence to match the ghost. Need at least three different pieces.\n");
    }

    // Get the real ghost type in the house
    GhostType *ghost = getGhostInHouse(house);
    // Convert the real ghost type to a string representation
    char ghostTypeStr[16]; 
    ghostToString(ghost->ghostType, ghostTypeStr);
    // Print the real ghost type
    printf("\nReal Ghost Type: %s\n", ghostTypeStr);
    
    // Compare the matching ghost type with the real ghost type and print whether they match
    printf("\nEvidence Matching?: %s\n", strcmp(ghostMatchStr, ghostTypeStr) == 0 ? "Correct" : "Incorrect");
}


/*
    Function: main
    Purpose: Entry point of the program. Initializes the house, places hunters and the ghost, creates threads for each hunter, and prints the results of the ghost hunt.
    Params: None
    Returns: int - status code of the program execution (C_OK)
*/
int main()
{
    // Initialize the random number generator with the current time
    srand(time(NULL));

    // Declare a house of type HouseType
    HouseType house;
    // Initialize the house
    initHouse(&house);
    // Populate the rooms in the house
    populateRooms(&house);

    // Initialize the ghost and place it randomly in the house
    initAndPlaceGhostRandomly(&house);

    // Declare an array to hold the names of the hunters
    char hunterNames[NUM_HUNTERS][MAX_STR];
    // Loop over the number of hunters
    for (int i = 0; i < NUM_HUNTERS; i++) {
        while (C_TRUE) {
            // Prompt the user to enter a name for each hunter
            printf("Enter name for hunter %d: ", i + 1);
            // Read the name entered by the user
            scanf("%s", hunterNames[i]);
            // Check if the name is a duplicate
            int j;
            // Loop over the names of the hunters that have already been entered
            for (j = 0; j < i; j++) {
                // If the name entered by the user is a duplicate, print an error message and break the loop
                if (strcmp(hunterNames[j], hunterNames[i]) == 0) {
                    printf("Huner [%s] already exists. Please enter a unique hunter name.\n", hunterNames[i]);
                    break;
                }
            }
            // If we've gone through all the names and haven't found a duplicate, break the loop
            if (j == i) break;
        }
        // Initialize the hunters and place them in the van
        initHunterAndPlaceInVan(house.rooms.head->room, hunterNames[i]);
    }

    // Create threads for the hunters and the ghost
    createThreads(&house, hunterNames, NUM_HUNTERS);
    // Print the results of the ghost hunt
    printHouse(&house);

    // Clean the allocated house memory
    cleanHouse(&house);
    // cleanHunterList(&house.hunters);

    // Return C_OK to indicate successful execution
    return C_OK;
}