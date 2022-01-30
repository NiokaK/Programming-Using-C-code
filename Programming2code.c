#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Declaration of Patient structure 
typedef struct { 
	char idNum[10];
	char patName[30];
	char covStat[15];
	char admitStat[15];
	int yearBirth;	
}Patient;

// Declaration of Room structure 
typedef struct {
	int roomNum;
	char respNeeded[15];
	int numBed;
	char persSign[15];
	char patientAssigned[35][8];
  	int totPatIDs;
}Room;

// Function prototype.  
int searchPatFile(Patient *);
void dischargedPatFile(Patient *, Room *);
void getData(Patient *, Room *);
void addPatient(Patient *);
void addRoom(Room *);
void changePatStat(Patient *);
void writePatFile(Patient *pati);
void writeRoomFile(Room *room);
void aiPatFile(Patient *, Room *);
void ageReport(Patient *, Room *);
void barGraphMaker(char *, int [], char *[], int);
void covidReport(Patient *);
void admitPatientList(Patient *pati, Room *room);
int menu(char *[], int);
void logAudit(char *);

// Declaration of varibale to count the number of patients and rooms
// entered and already in the system.
int totPatients, totRooms;
// Declaration of covid status in a array.
char *covStats[] = {"none", "mild", "average", "severe", "critical"};
char *respNeededs[] = {"resiprator", "none"};
char *persSigns[] = {"none", "nurse", "doctor"};

// Declaration of patient file and room file.
char patientFile[200], roomFile[200];

int main() 
{
	int choice, i; // Declaration of variables.
	char *mainMenuList[] = {"Add patient", "Add room", "Change patient status", "Discharge", "AI Assignment", "Room Admit By Age Report", "Covid-19 Status Report", "Admitted Patients List", "Save and Exit", "Exit Without Save"};
	Patient pati[35];
	Room room[21];
	char list;
	int totItems;
	char logStr[100];
	// Function Call Used to find patient and rooms file to access the data 
  	getData(pati, room);
  	
	do {
	    printf("Welcome to Conglomerate Hospice Ltd Database\n\n");
	    choice = menu(mainMenuList, 10);
	    switch (choice) {
			case 1:
				addPatient(pati); 
				break;
			case 2:
				addRoom(room);
				break;
			case 3:
				changePatStat(pati);
				break;
			case 4:
				dischargedPatFile(pati, room);
				break;
			case 5:
				aiPatFile(pati, room);
				break;
			case 6:
				ageReport(pati, room);
				break;
			case 7:
				covidReport(pati);
				break;
			case 8:
				admitPatientList(pati, room);
				break;
			case 9:
				writePatFile(pati);
				writeRoomFile(room);
				printf("Have a good day my friend! (Updates were saved)\n");
				logAudit("Updates were saved and program was closed");
				break;
			case 10:
				printf("Leave and don't come back!\n");
				logAudit("No updates were saved and program was closed");
				break;	
	    }
  } while(choice != 9 && choice != 10);
  return 0;
}
// Function Used to enter and validate menu selection.
int menu(char *list[], int totItems) {
	int i, choice = 0;
	while (true) {
		for (i = 0; i < totItems; i++) {
			printf("%d %s\n", i+1, list[i]);
		}
		printf("\n\nEnter code for the options above: ");
	    scanf("%d", &choice);
	    fflush(stdin);
        // if statement used to validate choice if choice is incorrect the user if asked to enter the number again until the correct number 
        // is accepted.
	    if (choice < 1 || choice > totItems) {
	    	printf("\nIncorect code!! please try again\n\n");
		} else {
			printf("_____________________________\n\n");
			return choice;
		}
	}
}
// Function used to locate and read patient and rooms flat files.
void getData(Patient *pati, Room *room) {
    // Declaration of variable 
  char fileName[50], fileLocation[150], patIDs[60], *strToken;
  int j, i;
  FILE *ptr;
  // Intiazile total patient to zero
  totPatients = 0;
  do {
      printf("Name of Patient File (something.txt): ");
      scanf("%[^\n]s",fileName);
      fflush(stdin);
      printf("Location (C:\\Users\\Name\\): ");
      scanf("%[^\n]s", fileLocation);
      fflush(stdin);
      // Used to copy the file and location and intiazile is as the patient file and location.
      // Strcat is used to join the filelocation and filename togather as one string 
      strcpy(patientFile, strcat(fileLocation, fileName));
      fflush(stdin);
      ptr = fopen(patientFile, "r");
      if (ptr == NULL) {
        printf("File not on record!\n");
      } else {
      	printf("Patient File Found!\n\n");
        while (!feof(ptr)) {
          fscanf(ptr,"%s ", pati[totPatients].idNum);
          fgets(pati[totPatients].patName, 25, ptr);
          // This section is used to validate the patient name to make sure the patient name is no more that 25bytes
          for (i = 0; i < 24; i++) {
            if (pati[totPatients].patName[i] == ' ' && pati[totPatients].patName[i+1] == ' ') {
              break;
            }
          }
          pati[totPatients].patName[i] = '\0';
          fscanf(ptr, "%s %d %s", pati[totPatients].covStat, &pati[totPatients].yearBirth, pati[totPatients].admitStat);
		  ++totPatients;
        }
      }
      // Program shall run while ptr is equal to NULL
    } while (ptr == NULL);
    fclose(ptr);
    strcpy(fileLocation, "");
    strcpy(fileName, "");
	do {
		printf("Name of Rooms File (something.txt): ");
		scanf("%[^\n]s", fileName);
		fflush(stdin);
		printf("Location (C:\\Users\\Name\\): ");
		scanf("%[^\n]s", fileLocation);
		fflush(stdin);
        // Used to copy the file and location and intiazile is as the rooms file and location.
       // Strcat is used to join the filelocation and filename togather as one string
		strcpy(roomFile, strcat(fileLocation, fileName));
		fflush(stdin);
		ptr = fopen(roomFile, "r");
		
		if (ptr == NULL) {
			printf("File not on record!\n");
		} else {
			printf("Room File Found!\n\n");
			totRooms = 0;
			while (!feof(ptr)) {
				fscanf(ptr, "%d %s %d %s %s", &room[totRooms].roomNum, room[totRooms].respNeeded, &room[totRooms].numBed, room[totRooms].persSign, patIDs);
				//strToken is used to separate the patient ID with a comma.
                strToken = strtok(patIDs, ",");
				room[totRooms].totPatIDs = 0;
                // If the strToken is compare is equal to none there a empty space is created 
				if (strcmp(strToken, "none") == 0) {
					strcpy(room[totRooms].patientAssigned[0], "");
				} else {
					do {
                        // If the strToken is not equal to NULL information is then added to the rooms file
						strcpy(room[totRooms].patientAssigned[room[totRooms].totPatIDs], strToken);
						++room[totRooms].totPatIDs;
						strToken = strtok(NULL, ",");
					} while (strToken != NULL);
				}
                // Used to count the rooms as there are entered 
				++totRooms;
			}
		}
        // Program shall run while ptr is equal to NULL
	} while (ptr == NULL);
  fclose(ptr);
}

void writePatFile(Patient *pati) {
  int i;
  //Creating a file pointer and fopen to write to the file 
  FILE *ptr; 
  
  ptr = fopen(patientFile, "w");

  for (i = 0; i < totPatients; i++) {
      // Negative is used to move number a space to the left 
    fprintf(ptr, "%-7s %-25s %-9s %d %s", pati[i].idNum, pati[i].patName, pati[i].covStat, pati[i].yearBirth, pati[i].admitStat);
    // If totPatients reach negative one then a new line created this means that once the space or full a new 
    // line should be created.
    if (i < totPatients-1) {
      fprintf(ptr, "\n");
    }
  }
  fclose(ptr);
}

void writeRoomFile(Room *room) {
  int i, j;
  //Creating a file pointer
  FILE *ptr = fopen(roomFile, "w");

  for (i = 0; i < totRooms; i++) {
    fprintf(ptr, "%d %-10s %d %-6s ", room[i].roomNum, room[i].respNeeded, room[i].numBed, room[i].persSign);
    if (room[i].totPatIDs > 0) {
	    for (j = 0; j < room[i].totPatIDs; j++) {
	      fprintf(ptr, "%s", room[i].patientAssigned[j]);
	      if (j < room[i].totPatIDs-1) {
	        fprintf(ptr, ",");
	      }
	    }
	} else {
		fprintf(ptr, "none");
	}
	if (i < totRooms-1) {
      fprintf(ptr, "\n");
    }
  }
  fclose(ptr);
}
// Function used to add a new patient
void addPatient(Patient *pati) {
	int choice = 0, i;
	char logStr[100];
	time_t curTime;
	struct tm *timeInfo;
	time(&curTime);
	timeInfo = localtime(&curTime);
	int year = timeInfo->tm_year % 100 + 2000;
	
	printf("New Patient Form\n----------------------------\n\n");
	do {
		printf("ID Number: ");
		scanf("%[^\n]s",pati[totPatients].idNum);
		fflush(stdin);
		if (strlen(pati[totPatients].idNum) != 7) {
		  printf("Invalid ID! Must be 7 digits\n\n");
		} else {
			for (i = 0; i < totPatients; i++) {
				if (strcmpi(pati[i].idNum, pati[totPatients].idNum) == 0) {
					break;
				}
			}
			if (i == totPatients) {
				break;
			} else {
				printf("ID is already taken!\n\n");
			}
		}
	} while (true);
	
	do {
		printf("\nPatient Full Name: ");
		scanf("%[^\n]s",pati[totPatients].patName);
		fflush(stdin);
		if (strlen(pati[totPatients].patName) > 25) {
		  printf("Invalid Name! Must be less than 25 characters\n");
		}
	} while (strlen(pati[totPatients].patName) > 25);
	
	printf("\nCovid Status:\n------------------------------\n");
  	choice = menu(covStats, 5);
  	strcpy(pati[totPatients].covStat, covStats[choice-1]);

	do {
		printf("Year of Birth: ");
		scanf("%d", &pati[totPatients].yearBirth);
		fflush(stdin);
		if (year - pati[totPatients].yearBirth <= 0 || year - pati[totPatients].yearBirth > 120) {
		  printf("Invalid Birth Year! Must be between 1 and 120\n\n");
		}
	} while (year - pati[totPatients].yearBirth <= 0 || year - pati[totPatients].yearBirth > 120);
	strcpy(pati[totPatients].admitStat, "admitted");
	printf("\n%s was added to the list!\n\n", pati[totPatients].patName);
	// This creates log message for add patient
	sprintf(logStr, "%s was added to the database", pati[totPatients].patName);
	logAudit(logStr);
	++totPatients;
}

// Function Used to add rooms
void addRoom(Room *room){
	int choice = 0, i;
	char logStr[100];
	printf("New Room Form\n-----------------------\n\n");
	do {
		printf("Enter room number: ");
		scanf("%d", &room[totRooms].roomNum);
		fflush(stdin);
		if (room[totRooms].roomNum < 201 || room[totRooms].roomNum > 235) {
			printf("Invalid Room Number! Must be between 200 and 235\n\n");
		} else {
			for (i = 0; i < totRooms; i++) {
				if (room[i].roomNum == room[totRooms].roomNum) {
					break;
				}
			}
			if (i == totRooms) {
				break;
			} else {
				printf("Room Number is already taken!\n\n");
			}
		}
	} while (true);
	printf("\nSelect whether the room has a respirator or not:\n\n");
	choice = menu(respNeededs, 2);
	strcpy(room[totRooms].respNeeded, respNeededs[choice-1]);

	do {
		printf("Enter number of beds: ");
		scanf("%d", &room[totRooms].numBed);
		if (room[totRooms].numBed < 1 || room[totRooms].numBed > 5){
		  printf("Invalid AMOUNT!! Must be 5 bed or less\n\n");
		}
	} while(room[totRooms].numBed < 1 || room[totRooms].numBed > 5);
	
	printf("Select a Personnel for the room:\n\n");
	choice = menu(persSigns, 3);
  	strcpy(room[totRooms].persSign, persSigns[choice-1]);
  	room[totRooms].totPatIDs = 0;
  	strcpy(room[totRooms].patientAssigned[0], "");
  	printf("\nRoom %d was added to the list!\n", room[totRooms].roomNum);
  	// This creates log message for add room.
	sprintf(logStr, "room %d was added to the database", room[totRooms].roomNum);
	logAudit(logStr);
  	++totRooms;
}

// Function Used to change patient status
void changePatStat(Patient *pati) {
  int index, choice = 0;
  char logStr[100];
  if ((index = searchPatFile(pati)) != -1) {
  	choice = menu(covStats, 5);
    printf("%s's Status was changed from %s to %s!\n", pati[index].patName, pati[index].covStat, covStats[choice-1]);
    // This creates log message for change patient status.
	sprintf(logStr, "%s's status was changed from %s to %s", pati[index].patName, pati[index].covStat, covStats[choice-1]);
	logAudit(logStr);
    strcpy(pati[index].covStat, covStats[choice-1] );
  }
}

// Function used to search  for the patients ID
int searchPatFile(Patient *pati) {
  int i;
  char patID[10];
  printf("Patient ID Number: ");
  scanf("%[^\n]s",patID);
  fflush(stdin);
  // For to search the patients ID number and strcmp is used to compare
  // what is on file to what is enter 
  for (i = 0; i < totPatients; i++) {
  	// if the id number is correct it would return that the id number is found 
    if (strcmp(patID, pati[i].idNum) == 0) {
      return i;
    }
  }
  // if the id num is not found this print statment would populate 
  printf("Patient record not found!\n\n");
  return -1;
}

void dischargedPatFile(Patient *pati, Room *room) {
	int i, j, k, l; 
	char patID[10];
	char logStr[100];
	printf("Patient ID Number: ");
	scanf("%[^\n]s",patID);
	fflush(stdin);
	// for loop used to search the patient file 
	for (i = 0; i < totPatients; i++) {
		// Compares if the id number enter matches the id number in the patient file.
		if (strcmp(patID, pati[i].idNum) == 0) {
			
			if (strcmpi(pati[i].admitStat, "admitted") == 0) {
				for (j = 0; j < totRooms; j++) {
					for (k = 0; k < room[j].totPatIDs; k++) {
						if (strcmpi(pati[i].idNum, room[j].patientAssigned[k]) == 0) {
							break;
						}
					}
					if (k != room[j].totPatIDs) {
						break;
					}
				}
				if (strcmpi(pati[i].admitStat, "admitted") == 0) {
					strcpy(pati[i].covStat, "none");
					strcpy(pati[i].admitStat, "discharged"); 
					sprintf(logStr, "%s was discharged from room %d", pati[i].patName, room[j].roomNum);
					if (j != totRooms) {
						for (l = k; l < room[j].totPatIDs-1; l++) {
							strcpy(room[j].patientAssigned[l], room[j].patientAssigned[l+1]);
						}
						strcpy(room[j].patientAssigned[l], "");
						--room[j].totPatIDs;
					}
				} else {
					sprintf(logStr, "Unsucessful Discharge");
				}
				printf("%s\n\n", logStr);
				logAudit(logStr);
			}
			return;
		}
	}
	printf("Patient record not found!\n\n");
	return;
}

void aiPatFile(Patient *pati, Room *room) {
	int age, i, j, k, a;
	time_t curTime;
  	struct tm *timeInfo;
  	time(&curTime);
  	timeInfo = localtime(&curTime);
	int year = timeInfo->tm_year % 100 + 2000;
	
	bool isAllowed;
	
	printf("AI Assignment:\n------------------------------------\n\n");
	for (i = 0; i < totPatients; i++) {
		age = year - pati[i].yearBirth;
		isAllowed = false;
		for (j = 0; j < totRooms; j++) {
			for (k = 0; k < room[j].totPatIDs; k++) {
				if (strcmpi(pati[i].idNum, room[j].patientAssigned[k]) == 0) {
					break;
				}
			}
			if (k != room[j].totPatIDs) {
				break;
			}
		}
		if (j != totRooms) {
			if (strcmpi(pati[i].covStat, "none") == 0) {
				printf("%s was discharged (room %d)\n", pati[i].patName, room[j].roomNum);
				strcpy(pati[i].admitStat, "discharged");
				for (a = k; a < room[j].totPatIDs-1; a++) {
					strcpy(room[j].patientAssigned[a], room[j].patientAssigned[a+1]);
				}
				strcpy(room[j].patientAssigned[a], "");
			} else {
				printf("%s was not assigned (Already in room %d)\n", pati[i].patName, room[j].roomNum);
			}
		} else {
			if (strcmpi(pati[i].covStat, "none") == 0) {
				printf("%s was discharged (no room)\n", pati[i].patName);
				strcpy(pati[i].admitStat, "discharged");
			} else if (strcmpi(pati[i].covStat, "critical") == 0) {
				for (j = 0; j < totRooms; j++) {
					if (room[j].totPatIDs < room[j].numBed && strcmpi(room[j].respNeeded, "respirator") == 0 && strcmpi(room[j].persSign, "doctor") == 0) {
						isAllowed = true;
						break;
					}
				}
			} else if (age > 60) {
				if (strcmpi(pati[i].covStat, "severe") == 0) {
					for (j = 0; j < totRooms; j++) {
						if (room[j].totPatIDs < room[j].numBed && strcmpi(room[j].respNeeded, "respirator") == 0 && (strcmpi(room[j].persSign, "nurse") == 0 || strcmpi(room[j].persSign, "none"))) {
							isAllowed = true;
							break;
						}
					}
				} else if (strcmpi(pati[i].covStat, "mild") == 0 || strcmpi(pati[i].covStat, "average") == 0) {
					for (j = 0; j < totRooms; j++) {
						if (room[j].totPatIDs < room[j].numBed && strcmpi(room[j].respNeeded, "none") == 0 && strcmpi(room[j].persSign, "none") == 0) {
							isAllowed = true;
							break;
						}
					}
				}
			} else if (age >= 40) {
				if (strcmpi(pati[i].covStat, "average") == 0 || strcmpi(pati[i].covStat, "severe") == 0) {
					for (j = 0; j < totRooms; j++) {
						if (room[j].totPatIDs < room[j].numBed && strcmpi(room[j].respNeeded, "respirator") == 0 && (strcmpi(room[j].persSign, "nurse") == 0 || strcmpi(room[j].persSign, "none"))) {
							isAllowed = true;
							break;
						}
					}
				} else {
					for (j = 0; j < totRooms; j++) {
						if (room[j].totPatIDs < room[j].numBed && strcmpi(room[j].respNeeded, "none") == 0 && strcmpi(room[j].persSign, "none") == 0) {
							isAllowed = true;
							break;
						}
					}
				}
			} else if (age >= 20) {
				if (strcmpi(pati[i].covStat, "severe") == 0) {
					for (j = 0; j < totRooms; j++) {
						if (room[j].totPatIDs < room[j].numBed && strcmpi(room[j].respNeeded, "none") == 0 && strcmpi(room[j].persSign, "doctor") == 0) {
							isAllowed = true;
							break;
						}
					}
				} else if (strcmpi(pati[i].covStat, "mild") == 0 || strcmpi(pati[i].covStat, "average") == 0) {
					for (j = 0; j < totRooms; j++) {
						if (room[j].totPatIDs < room[j].numBed && strcmpi(room[j].respNeeded, "none") == 0 && strcmpi(room[j].persSign, "none") == 0) {
							isAllowed = true;
							break;
						}
					}
				}
			} else {
				if (strcmpi(pati[i].covStat, "average") == 0 || strcmpi(pati[i].covStat, "severe") == 0) {
					for (j = 0; j < totRooms; j++) {
						if (room[j].totPatIDs < room[j].numBed && strcmpi(room[j].respNeeded, "none") == 0 && strcmpi(room[j].persSign, "nurse") == 0) {
							isAllowed = true;
							break;
						}
					}
				} else {
					for (j = 0; j < totRooms; j++) {
						if (room[j].totPatIDs < room[j].numBed && strcmpi(room[j].respNeeded, "none") == 0 && strcmpi(room[j].persSign, "none") == 0) {
							isAllowed = true;
							break;
						}
					}
				}
			}
			if (isAllowed) {
				strcpy(room[j].patientAssigned[room[j].totPatIDs], pati[i].idNum);
				++room[j].totPatIDs;
				printf("%s was assigned (room %d)\n", pati[i].patName, room[j].roomNum);
			} else if (strcmpi(pati[i].covStat, "none") != 0) {
				printf("%s assignment unsucessful (no available room)\n", pati[i].patName);
			}
		}
	}
	printf("______________________________________\n\n");
	logAudit("AI Assignment was conducted");
}

void ageReport(Patient *pati, Room *room) {
	int ageFrequency[4] = {0, 0, 0, 0};
	char *category[] = {"Over 60", "40 to 60", "20 to 39", "Younger than 39"};
	int i, j, k, age, totInRoom = 0;
	time_t curTime;
  	struct tm *timeInfo;
  	time(&curTime);
  	timeInfo = localtime(&curTime);
	int year = timeInfo->tm_year % 100 + 2000;
	
	for (i = 0; i < totPatients; i++) {
		age = year - pati[i].yearBirth;
		for (j = 0; j < totRooms; j++) {
			for (k = 0; k < room[j].totPatIDs; k++) {
				if (strcmpi(pati[i].idNum, room[j].patientAssigned[k]) == 0) {
					break;
				}
			}
			if (k != room[j].totPatIDs) {
				break;
			}
		}
		if (j != totRooms) {
			++totInRoom;
			if (age > 60) {
				++ageFrequency[0];
			} else if (age >= 40) {
				++ageFrequency[1];
			} else {
				if (age == 39 || age >= 20) {
					++ageFrequency[2];
				} else {
					++ageFrequency[3];
				}
			}
		}
	}
	if (totInRoom > 0) {
		barGraphMaker("Room Admittance By Age Report", ageFrequency, category, 4);
	} else {
		printf("There are no patients in any room!\n\n");
	}
}

void barGraphMaker(char *title, int freq[], char *cat[], int tot) {
	int i, j;
	int unit, roundedFreq[tot], left, right;
	int max = 5, space, fileSize, choice;
	char defaultFile[100], fileBuffer[10000], fileName[100];
	char *choiceList[] = {"Yes", "No"};
	strcpy(defaultFile, title);
	FILE *ptr = fopen(strcat(defaultFile, ".txt"), "w");
	
	for (i = 0; i < tot; i++) {
		if (freq[i] > max) {
			max = freq[i];
		}
	}
	unit = ceil((double)max / 10);
	max = unit * 10;

	
	for (i = 0; i < tot; i++) {
		right = unit;
		while (right < freq[i]) {
			right += unit;
		}
		left = right - unit;
		//Round number to the nearest whole number
		if (freq[i]-left < right-freq[i]) {
			roundedFreq[i] = left;
		} else {
			roundedFreq[i] = right;
		}
	}
	
	fprintf(ptr, "%-19s%s\n", "", title);
	fprintf(ptr, "%-19s%d units per '*****'\n", "", unit);
	fprintf(ptr, "%-18s ________________________________________________________________\n", "");
	for (i = 0; i < tot; i++) {
		fprintf(ptr, "%-18s|%-64s|\n%-18s|", "", "", "");
		for (j = unit; j <= roundedFreq[i]; j += unit) fprintf(ptr, "*****");
		space = 64 - (5*roundedFreq[i]/unit);
		for (j = 1; j <= space; j++) fprintf(ptr, " ");
		fprintf(ptr, "|\n%15s --|", cat[i]);
		for (j = unit; j <= roundedFreq[i]; j += unit) fprintf(ptr, "*****");
		fprintf(ptr, " %-2d ", freq[i]);
		space = 60 - (5*roundedFreq[i]/unit);
		for (j = 1; j <= space; j++) fprintf(ptr, " ");
		fprintf(ptr, "|\n%-18s|", "");
		for (j = unit; j <= roundedFreq[i]; j += unit) fprintf(ptr, "*****");
		space = 64 - (5*roundedFreq[i]/unit);
		for (j = 1; j <= space; j++) fprintf(ptr, " ");
		fprintf(ptr, "|\n");
	}
	fprintf(ptr, "%-18s|________________________________________________________________|\n", "");
	fclose(ptr);
	ptr = fopen(defaultFile, "r");
	fscanf(ptr, "%[^\0]s", fileBuffer);
	fclose(ptr);
	printf("%s", fileBuffer);
	
	printf("\nDo you want to save the file?\n------------------------------------------\n");
	choice = menu(choiceList, 2);
	if (choice == 1) {
		do {
			printf("File Name: ");
			gets(fileName);
			fflush(stdin);
			if (strstr(fileName, ".txt") == NULL) {
				printf("File name must have .txt in it!\n");
			} else {
				break;
			}
		} while (true);
		printf("%s was saved!\n", title);
		logAudit("Room admittance by age report was viewed");
		rename(defaultFile, fileName);
	} else {
		remove(defaultFile);
	}
}

void covidReport(Patient *pati) {
	char *covidStats[] = {"none", "mild", "average", "severe", "critical"};
	char *catTemp;
	int statusFreq[] = {0, 0, 0, 0, 0};
	int i, j, freqTemp;
	
	for (i = 0; i < totPatients; i++) {
		for (j = 0; j < 5; j++) {
			if (strcmpi(pati[i].covStat, covidStats[j]) == 0) {
				++statusFreq[j];
			}
		}
	}
	
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4-i; j++) {
			if (statusFreq[j] < statusFreq[j+1]) {
				freqTemp = statusFreq[j];
				statusFreq[j] = statusFreq[j+1];
				statusFreq[j+1] = freqTemp;
				catTemp = covidStats[j];
				covidStats[j] = covidStats[j+1];
				covidStats[j+1] = catTemp;
			}
		}
	}
	barGraphMaker("Covid-19 Status Report", statusFreq, covidStats, 5);
	logAudit("Covid-19 Status Report was viewed");
}

void admitPatientList(Patient *pati, Room *room) {
	int i, j, k, totAdmit = 0, choice;
	Patient admittedPat[35], temp;
	char roomID[50], fileBuffer[10000], fileName[100];
	char *choiceList[] = {"Yes", "No"};
	FILE *ptr = fopen("Admitted Patients List.txt", "w");
	
	for (i = 0; i < totPatients; i++) {
		if (strcmpi(pati[i].admitStat, "admitted") == 0) {
			admittedPat[totAdmit] = pati[i];
			++totAdmit;
		}
	}
	
	if (totAdmit > 0) {
	
		for (i = 0; i < totAdmit-1; i++) {
			for (j = 0; j < totAdmit-(i+1); j++) {
				if (strcmpi(admittedPat[j].idNum, admittedPat[j+1].idNum) > 0) {
					temp = admittedPat[j];
					admittedPat[j] = admittedPat[j+1];
					admittedPat[j+1] = temp;
				}
			}
		}
		
		fprintf(ptr, "Admitted Patients List Report\n____________________________________________________\n\n");
		for (i = 0; i < totAdmit; i++) {
			for (j = 0; j < totRooms; j++) {
				for (k = 0; k < room[j].totPatIDs; k++) {
					if (strcmpi(admittedPat[i].idNum, room[j].patientAssigned[k]) == 0) {
						break;
					}
				}
				if (k != room[j].totPatIDs) {
					break;
				}
			}
			fprintf(ptr, "%s, %s (Room %s): %s\n", admittedPat[i].idNum, admittedPat[i].patName, j != totRooms ? itoa(room[j].roomNum, roomID, 10) : "none", admittedPat[i].covStat);
		}
		fprintf(ptr, "____________________________________________________\n\n");
		
		fclose(ptr);
		ptr = fopen("Admitted Patients List.txt", "r");
		fscanf(ptr, "%[^\0]s", fileBuffer);
		printf("%s", fileBuffer);
		fclose(ptr);
		
		printf("\nDo you want to save the file?\n------------------------------------------\n");
		choice = menu(choiceList, 2);
		if (choice == 1) {
			do {
				printf("File Name: ");
				gets(fileName);
				fflush(stdin);
				if (strstr(fileName, ".txt") == NULL) {
					printf("File name must have .txt in it!\n");
				} else if (rename("Admitted Patients List.txt", fileName)) {
					printf("A File with that name already exists, try again!\n\n");
				} else {
					break;
				}
			} while (true);
			printf("Admitted Patients List was saved!\n");
		} else {
			remove("Admitted Patients List.txt");
		}
	} else {
		printf("There are no admitted patients!\n");
	}
	logAudit("Admitted Patient Report was viewed");
}

void logAudit(char *logStr){
	int i;
	char record[100];
	int totItems = 0;
	time_t curTime;
  	struct tm *timeInfo;
  	time(&curTime);
  	timeInfo = localtime(&curTime);
	strftime(record, 100, "%c", timeInfo);
    FILE *ptr;
    ptr = fopen("log.txt", "a");
    fprintf(ptr, "%s: %s\n", record, logStr);
    fclose(ptr);
}
