#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // for usleep function

#define MAX_STR 64
#define MAX_RUNS 50
#define BOREDOM_MAX 100
#define C_TRUE 1
#define C_FALSE 0
#define C_OK 2
#define HUNTER_WAIT 5000
#define GHOST_WAIT 600
#define NUM_HUNTERS 4
#define FEAR_MAX 10
#define MAX_ARR 128
#define LOGGING  C_TRUE

// Enums for Evidence and Ghost types
typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

// Structures for different types used in the application
typedef struct Ghost GhostType;
typedef struct Node NodeType;
typedef struct HunterNode HunterNodeType;
typedef struct Room RoomType;
typedef struct RoomArray RoomArrayType;
typedef struct HunterArray HunterArrayType;
typedef struct Evidence Evidence;
typedef struct House HouseType;
typedef struct Hunter HunterType;
typedef struct HunterList HunterListType;
typedef struct RoomNode RoomNodeType;
typedef struct EvidenceList EvidenceListType;
typedef struct RoomList RoomListType;
typedef struct ThreadData ThreadDataType;

enum EvidenceType
{
    EMF,
    TEMPERATURE,
    FINGERPRINTS,
    SOUND,
    EV_COUNT,
    EV_UNKNOWN
};
enum GhostClass
{
    POLTERGEIST,
    BANSHEE,
    BULLIES,
    PHANTOM,
    GHOST_COUNT,
    GH_UNKNOWN
};
enum LoggerDetails
{
    LOG_FEAR,
    LOG_BORED,
    LOG_EVIDENCE,
    LOG_SUFFICIENT,
    LOG_INSUFFICIENT,
    LOG_UNKNOWN
};

// Structure representing a Ghost
struct Ghost
{
    int id;               // Unique identifier for the ghost
    GhostClass ghostType; // Type of the ghost
    struct Room *room;    // The room where the ghost is located
    int boredomTimer;     // Timer representing the ghost's boredom
};

// Structure representing a Node in the Room list
struct RoomNode
{
    RoomType *room;          // Pointer to the room
    struct RoomNode *next;   // Pointer to the next RoomNode
    struct RoomNode *prev;   // Pointer to the previous RoomNode
};

// Structure representing a list of Rooms
struct RoomList
{
    RoomNodeType *head;      // Pointer to the head RoomNode of the list
    RoomNodeType *tail;      // Pointer to the tail RoomNode of the list
    int count;               // Count of rooms in the list
};

// Structure representing an array of Rooms
struct RoomArray
{
    RoomType *rooms[MAX_ARR];  // Array of RoomType pointers
    int size;                  // Current size of the array
};

// Structure representing an array of Hunters
struct HunterArray
{
    HunterType *hunters[MAX_ARR]; // Array of HunterType pointers
    int size;                     // Current size of the array
};

// Structure representing a Node in the Ghost list
struct Node
{
    GhostType *ghost;       // Pointer to the ghost
    NodeType *next;         // Pointer to the next Node
};

// Structure representing a Node in the Hunter list
struct HunterNode
{
    HunterType *hunter;      // Pointer to the hunter
    HunterNodeType *next;    // Pointer to the next HunterNode
};

// Structure representing a piece of Evidence
struct Evidence
{
    EvidenceType etype;   // Type of evidence
    struct Evidence *next; // Pointer to the next evidence node
    sem_t evidenceSem;    // Semaphore for evidence access control
};

// Structure representing the list of Evidence
struct EvidenceList
{
    Evidence *head; // Head of the evidence list
    Evidence *tail; // Tail of the evidence list
    int count;      // Number of elements in the evidence list
};


// Structure representing a Hunter
struct Hunter
{
    int id;                             // Unique identifier for the hunter
    char name[MAX_STR];                 // Name of the hunter
    enum EvidenceType equipmentType;    // Type of equipment the hunter possesses
    struct Evidence *collectedEvidence; // Evidence collected by the hunter
    int fear;                           // Fear level of the hunter
    int boredomTimer;                   // Timer representing the hunter's boredom
    struct Room *currentRoom;           // Current room of the hunter
};

// Structure representing the list of Hunters
struct HunterList
{
    HunterNodeType *head;  // Head of the hunter list
    HunterNodeType *tail;  // Tail of the hunter list
    int count;             // Number of elements in the hunter list
};

// Structure representing a Room
struct Room
{
    int id;                        // Unique identifier for the room
    char name[MAX_STR];            // Name of the room
    struct Ghost *ghost;           // Ghost present in the room
    struct EvidenceList evidences; // List of evidence in the room
    struct HunterList hunters;     // List of hunters in the room
    RoomListType* connectedRooms;  // List of rooms connected to this room
    pthread_t roomPThread;         // Thread for the room
    sem_t roomSem;                 // Semaphore for room access control
};

// Structure representing the House
struct House
{
    RoomListType rooms;                            // List of rooms in the house
    HunterListType hunters;                        // List of hunters in the house
    enum EvidenceType sharedEvidence[MAX_ARR];     // Array for shared evidence
    HunterArrayType huntersExitedFear[MAX_ARR];    // Array for hunters who exited due to fear
    HunterArrayType huntersExitedBoredom[MAX_ARR]; // Array for hunters who exited due to boredom
    int sharedEvidenceCount;                       // Count of shared evidence
    sem_t evidenceSem;                             // Semaphore for evidence access control
};

// Structure representing the thread data
struct ThreadData {
    HouseType *house;   // Pointer to the house
    HunterType *hunter; // Pointer to the hunter
};

int randInt(int, int);                            // Pseudo-random number generator function
float randFloat(float, float);                    // Pseudo-random float generator function
enum GhostClass randomGhost();                    // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char *);      // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char *); // Convert an evidence type to a string, stored in output parameter

// Logging Utilities
void l_hunterInit(char *name, enum EvidenceType equipment);
void l_hunterMove(char *name, char *room);
void l_hunterReview(char *name, enum LoggerDetails reviewResult);
void l_hunterCollect(char *name, enum EvidenceType evidence, char *room);
void l_hunterExit(char *name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char *room);
void l_ghostMove(char *room);
void l_ghostEvidence(enum EvidenceType evidence, char *room);
void l_ghostExit(enum LoggerDetails reason);

// House Initialization and Management Functions
void initHouse(HouseType *house);
RoomType *createRoom(char *name);
void connectRooms(RoomType *room1, RoomType *room2);
void initHunterAndPlaceInVan(RoomType *vanRoom, char *name);
void initAndPlaceGhostRandomly(HouseType *house);
void populateRooms(HouseType *house);
void addRoom(RoomListType *list, RoomType *r);
GhostType *createGhost(GhostType **ghost, int id, GhostClass ghostType, RoomType *room);
HunterType *createHunter(char *name, RoomType *room);
void createThreads(HouseType *house, char hunterNames[][MAX_STR], int numHunters);
void performHunterAction(HouseType *house, HunterType *hunter);
void performGhostAction(HouseType *house);

// Hunter and Ghost Manipulation Functions
void insertAtHeadHunter(HunterListType *list, HunterType *newHunter);
void insertAtHeadEvidence(EvidenceListType *list, enum EvidenceType newEvidence);
void removeHunterFromRoom(RoomType *room, HunterType *hunter);
void moveHunterToRandomRoom(HunterType *hunter, RoomType *currentRoom);
void removeEvidence(EvidenceListType *list, enum EvidenceType targetEvidence);
void moveGhostToRandomRoom(GhostType *ghost, RoomType *currentRoom);
void removeHunterFromHouse(HouseType *house, HunterType *hunter);
void collectEvidence(HouseType *house, HunterType *hunter, RoomType *currentRoom);

// Evidence and Investigation Functions
int reviewEvidence(HouseType *house);
int isHunterPresent(RoomType *room);
GhostClass getGhostMatch(HouseType *house);
GhostType *getGhostInHouse(HouseType *house);
HunterType *findHunterByName(const HouseType *house, const char *name);
EvidenceType randomEvidence(enum GhostClass ghostType);
void printHouse(HouseType *house);

// Functions for cleanup
void cleanHunterList(HunterListType *list);
void cleanRoom(RoomType *room);
void cleanRoomList(RoomListType *list);
void cleanEvidenceList(EvidenceListType *list);
void cleanHouse(HouseType *house);