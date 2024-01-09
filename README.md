#### Program Description / Purpose

- The Paranormal Investigators Communication Simulator is a specialized tool emulating a networked communication system for ghost hunts. Built on prior developments, it simulates a map of interconnected rooms where four hunters equipped with devices navigate to collect evidence. This program replicates scenarios where encounters with ghosts escalate hunters' fear levels and generate distinct evidence types. By managing evidence data, identifying ghost types based on gathered evidence, and simulating hunter interactions, it aids in evaluating communication networks' efficacy during ghost hunts. This simulator serves as a training and analysis platform, enabling investigators to test strategies and enhance preparedness for real investigations.

#### Included Files

- **`fp`**: The compiled program file, which can be executed from the terminal.
- **`Makefile`**: A Makefile for compiling and linking the program components for ease of execution.
- **`README.md`**: This README file, which includes information specified in the assignment specification and program details.
- **`defs.h`**: A header file containing constant definitions and function prototypes.
- **`evidence.c`**: A source code file containing the implementation of functions related to evidence, including `randomEvidence()`, `insertAtHeadEvidence()`, `removeEvidence()`, `reviewEvidence()`.
- **`ghost.c`**: A source code file containing the implementation of functions related to ghosts, including `initAndPlaceGhostRandomly()`, `initGhostNode()`, `createGhost()`, `moveGhostToRandomRoom()`, `getGhostMatch()`.
- **`hunter.c`**: A source code file containing functions related to the hunters, including `initHunterAndPlaceInVan()`, `createHunter()`, `insertAtHeadHunter()`, `collectEvidence()`, `removeHunterFromRoom()`, `moveHunterToRandomRoom()`, findHunterByName()`, `removeHunterFromHouse()`.
- **`house.c`**: A source code file containing the implementation of functions related to the overall house, including `populateRooms()`, `initHouse()`, `getGhostInHouse()`.
- **`room.c`**: A source code file containing functions related to rooms within the house, including `createRoom()`, `connectRooms()`, `addRoom()`.
- **`logger.c`**: A source code file containing functions related to the logger, including `initRoomArray()`, `initRoom()`, `addRoom()`, `printRooms()`, `cleanupRoomArray()`
- **`threads.c`**: A source code file containing functions related to threading / semaphores / mutexes, including `ghostThread()`, `hunterThread()`, `performGhostAction()`, `performHunterAction()`, `createThreads()`.
- **`clean.c`**: A source code file containing functions related to cleanup operations, including `cleanRoom()`, `cleanRoomList()`, `cleanEvidenceList()`, `cleanHunterList()`, `cleanHouse()`.
- **`utils.c`**: A source code file containing utility and helper functions used throughout the program including `randInt()`, `randFloat()`, `randomGhost()`, `evidenceToString()`, `ghostToString()`, `isHunterPresent()`.
- **`main.c`**: A source code file containing the main program logic, handling user input, and printing the program's menu, including `main()`, `printHouse()`.

#### Compiling and Running

- **Compiling:** this program can be compiled using the provided Makefile.
- The Makefile specifies all the necessary compiler flags, ensuring comprehensive error checking during compilation.

  - To compile the program, navigate to the directory containing the Makefile and use the command `make`.
  - This command will automatically compile all necessary source files and create an executable file named `fp`.

- Additional Makefile commands:

  - `clean`: removes all of the object files and executables created by the make command
  - `valgrind`: runs valgrind with extra flags on the application executable
  - `build`: runs the make command, and then removes all the object files

- **Running:** after compiling the program, it can be ran by calling the file through the terminal.
  - If compiled with the command above, it can be ran using the command `./fp` in the same directory it was compiled.

#### Usage Instructions

- Given the program is correctly executed from the command line, using either the command described above in the **_#### Compiling and Running_** section, or a simillar command, it will then prompt the user to enter names for the four hunters.
  - Please enter four names, they can be any string values
- After entering four names, the simulation will begin, and you can see all of the logs of the different interactions in the house.
- After the simulation is over, the program will output the result of the simulation, including the hunters who exited because of fear, boredom, whether the ghost or hunters won, and whether or not the ghost was correctly identified.
