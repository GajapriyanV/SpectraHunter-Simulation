#include "defs.h"


/*
    Function: randomEvidence
    Purpose: Generates a random evidence based on the ghost type.
    Params:
        Input: 
            enum GhostClass ghostType (in) - the type of the ghost
        Output: EvidenceType - the generated evidence
*/
EvidenceType randomEvidence(enum GhostClass ghostType) {
    // Generate a random number between 0 and the count of evidence types - 1
    int random = randInt(0, EV_COUNT - 1);
    // If the ghost type is Poltergeist
    if (ghostType == POLTERGEIST) {
        // Return a random evidence type
        if (random == 0) return EMF;
        else if (random == 1) return TEMPERATURE;
        else if (random == 2) return FINGERPRINTS;
    } 
    // If the ghost type is Banshee
    else if (ghostType == BANSHEE) {
        // Return a random evidence type
        if (random == 0) return EMF;
        else if (random == 1) return TEMPERATURE;
        else if (random == 2) return SOUND;
    } 
    // If the ghost type is Bullies
    else if (ghostType == BULLIES) {
        // Return a random evidence type
        if (random == 0) return EMF;
        else if (random == 1) return FINGERPRINTS;
        else if (random == 2) return SOUND;
    } 
    // If the ghost type is Phantom
    else if (ghostType == PHANTOM) {
        // Return a random evidence type
        if (random == 0) return TEMPERATURE;
        else if (random == 1) return FINGERPRINTS;
        else if (random == 2) return SOUND;
    }
    // If the ghost type is unknown, return unknown evidence
    return EV_UNKNOWN;
}


/*
    Function: insertAtHeadEvidence
    Purpose: Inserts a new evidence at the head of the evidence list.
    Params:
        Input: 
            EvidenceListType *list (in) - the list to insert the evidence into
            enum EvidenceType newEvidence (in) - the evidence to insert
*/
void insertAtHeadEvidence(EvidenceListType *list, enum EvidenceType newEvidence)
{
    // Allocate memory for a new evidence node
    Evidence *newNode = (Evidence *)malloc(sizeof(Evidence));
    // Set the evidence type of the new node to the provided evidence
    newNode->etype = newEvidence;
    // Set the next node to NULL by default
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
        // Insert the new node at the head of the list
        newNode->next = list->head;
        list->head = newNode;
    }
    // Increment the count of nodes in the list
    list->count++;
}


/*
    Function: removeEvidence
    Purpose: Removes a specific evidence from the evidence list.
    Params:
        Input: 
            EvidenceListType *list (in) - the list to remove the evidence from
            enum EvidenceType targetEvidence (in) - the evidence to remove
*/
void removeEvidence(EvidenceListType *list, enum EvidenceType targetEvidence)
{
    // Initialize pointers to the current and previous nodes
    Evidence *current = list->head;
    Evidence *prev = NULL;

    // Traverse the list
    while (current != NULL)
    {
        // If the current node's evidence matches the target evidence
        if (current->etype == targetEvidence)
        {
            // If the current node is the head of the list
            if (prev == NULL)
            {
                // Remove the head
                list->head = current->next;
                // If the list is now empty, also update the tail
                if (list->head == NULL) {
                    list->tail = NULL;
                }
            }
            else
            {
                // Remove the current node from the list
                prev->next = current->next;
                // If the removed node was the tail, update the tail
                if (prev->next == NULL) {
                    list->tail = prev;
                }
            }

            // Free the memory of the removed node
            free(current);
            // Decrement the count of nodes in the list
            list->count--;
            // Exit the loop
            break;
        }
        // Move to the next node
        prev = current;
        current = current->next;
    }
}


/*
    Function: reviewEvidence
    Purpose: Reviews the shared evidence in the house and counts the unique evidences.
    Params:
        Input: 
            HouseType *house (in) - the house to review the evidence from
        Output: int - the count of unique evidences
*/
int reviewEvidence(HouseType *house) {
    // Initialize the count of unique evidences
    int uniqueEvidenceCount = 0;
    // Initialize an array to store the checked evidences
    EvidenceType checkedEvidence[3];

    // Loop for each piece of shared evidence
    for (int i = 0; i < house->sharedEvidenceCount; i++) {
        // Get the current evidence
        EvidenceType currentEvidence = house->sharedEvidence[i];

        // Assume the current evidence is unique
        int isUnique = C_TRUE;
        // Check if the current evidence is already in the checked evidences
        for (int j = 0; j < uniqueEvidenceCount; j++) {
            if (checkedEvidence[j] == currentEvidence) {
                // If it is, mark it as not unique and break the loop
                isUnique = C_FALSE;
                break;
            }
        }
        // If the current evidence is unique
        if (isUnique) {
            // Add it to the checked evidences
            checkedEvidence[uniqueEvidenceCount] = currentEvidence;
            // Increment the count of unique evidences
            uniqueEvidenceCount++;
        }
    }

    // Return the count of unique evidences
    return uniqueEvidenceCount;
}