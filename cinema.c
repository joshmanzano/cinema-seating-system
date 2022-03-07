#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define maxSeats 50
#define maxPatrons 1 
#define maxReservedSeats 4
#define stringLength 255
#define firstSeatLetter 'A'
#define lastSeatLetter 'E'
#define firstSeatNumber 1
#define lastSeatNumber 10
#define MovieSchedFilename "MovieSched.txt"
#define TicketFilename "Ticket.txt"
#define false 0
#define true 1

// Auxiliary Structures
typedef struct SeatNumbers {
    char letter;
    int number;
    int taken;
} SeatNumber;

typedef struct Times {
    int hour;
    int minute;
} Time;


// Main Structures
typedef struct Movies {
    char title[stringLength];
    char desc[stringLength];
    Time showTime;
} Movie;

typedef struct Reservations {
    Movie* movie;
    SeatNumber* takenSeats;
} Reservation;

typedef struct Patrons {
    Movie* movie;
    SeatNumber* takenSeats;
    int numOfTakenSeats;
} Patron;

// Loads movie schedules from text file.
Movie* readMovieSched(Movie* movies, int* numOfMovies){
    FILE *fp;
    char titlebuff[stringLength];
    int hour = 0;
    int minute = 0;
    int count = 0;

    fp = fopen(MovieSchedFilename,"r");
    if(fp){
        while(fscanf(fp,"%[^\n]\n", titlebuff) != EOF){
            fscanf(fp,"%d:%d\n",&hour,&minute);
            *numOfMovies += 1;
        }
        fclose(fp);
        movies = malloc((*numOfMovies) * sizeof(Movie));
        fp = fopen(MovieSchedFilename,"r");
        while(fscanf(fp,"%[^\n]\n", titlebuff) != EOF){
            fscanf(fp,"%d:%d\n",&hour,&minute);
            strcpy(movies[count].title, titlebuff);
            movies[count].showTime.hour = hour;
            movies[count].showTime.minute = minute;
            count += 1;
        }
        fclose(fp);
    }else{
        printf("No file found. Upload a schedule file.\n");
    }

    return movies;
}

// Initializes SeatNumber structs with memory (malloc).
SeatNumber* initSeatNumber(){
    SeatNumber* takenSeats = malloc(maxSeats * sizeof(SeatNumber));
    int i,j, count = 0;
    for(i = firstSeatLetter; i <= lastSeatLetter; i++){
        for(j = firstSeatNumber; j <= lastSeatNumber; j++){
            takenSeats[count].letter = i;
            takenSeats[count].number = j;
            takenSeats[count].taken = false;
            count += 1;
        }
    }
    return takenSeats;
}

// Initializes Reservation structs with memory (malloc).
Reservation* initReservations(Reservation* reservations, Movie* movies, int numOfMovies){
    reservations = malloc(numOfMovies * sizeof(Reservation));
    int i;
    for(i = 0; i < numOfMovies; i++){
        reservations[i].movie = &movies[i]; 
        reservations[i].takenSeats = initSeatNumber();
    }
    return reservations;
}

// Display movie titles and showtimes.
void displayMovies(Movie* movies, int numOfMovies){
    int i;
    for(i = 0; i < numOfMovies; i++){
        printf("%s\n", movies[i].title);
        printf("%d:%d\n", movies[i].showTime.hour, movies[i].showTime.minute);
    }
}

// Display seats
void displayTakenSeats(SeatNumber* takenSeats){
    int i,j;
    for(i = 0; i <= lastSeatLetter-firstSeatLetter; i++){
        for(j = 0; j < lastSeatNumber; j++){
            if(takenSeats[j+(lastSeatNumber*i)].taken){
                printf("X ");
            }else{
                printf("%c%d ", takenSeats[j+(lastSeatNumber*i)].letter, takenSeats[j+(lastSeatNumber*i)].number);
            }
        }
        printf("\n");
    }
}

// Display all reservations.
void displayReservations(Reservation* reservations, int numOfMovies){
    int i;
    for(i = 0; i < numOfMovies; i++){
        printf("%s\n", reservations[i].movie->title);
        displayTakenSeats(reservations[i].takenSeats);
    }
}

// Display the actions a user can do.
int displayChoices(){
    int choice = 0;
    while(choice < 1 || choice > 5){
        printf("## CINEMA SEATING SYSTEM ##\n");
        printf("[1] Load Schedule\n");
        printf("[2] View Schedule\n");
        printf("[3] Reserve Seat\n");
        printf("[4] Print Ticket\n");
        printf("[5] Save and Exit\n");
        printf("Enter choice: ");
        fflush(stdin);
        scanf("%d",&choice);
    }
    return choice;
}

// Get the seat which corresponds to letter and number.
SeatNumber* getTakenSeat(char letter, int number, Reservation* reservation){
    int i;
    for(i = 0; i < maxSeats; i++){
        if(reservation->takenSeats[i].letter == letter
        && reservation->takenSeats[i].number == number){
            return &reservation->takenSeats[i];
        }
    }
    return NULL;
}

// Get the reservation of a movie.
Reservation* checkReservation(Reservation* reservations, char* movie, int numOfMovies){
    Reservation* reservation = NULL;
    int i;
    for(i = 0; i < numOfMovies; i++){
        if(strcmp(movie, reservations[i].movie->title) == 0){
            reservation = &reservations[i];
        }
        // displayTakenSeats(reservations[i].takenSeats);
    }
    return reservation;
}

// Check if all of the seats in the reservation are full.
int checkFullSeats(Reservation* reservation){
    int i;
    for(i = 0; i < maxSeats; i++){
        if(!reservation->takenSeats[i].taken){
            return false;
        }
    }
    return true;
}

// Allows users to pick preferred movies and reserve seat/s.
void reserveSeat(Reservation* reservations, Patron* patron, int numOfMovies){
    Reservation* reservation = NULL; 
    SeatNumber* seat = NULL;
    char movie[stringLength];
    char buffer[stringLength];
    char letter;
    int number;
    int i;
    int numOfSeats = 0;

    printf("Preferred movie: ");
    fflush(stdin);
    fgets(movie,stringLength,stdin);
    strtok(movie,"\n");
    print("%s",movie);
    reservation = checkReservation(reservations, movie, numOfMovies);
    if(!checkFullSeats(reservation)){
        displayTakenSeats(reservation->takenSeats);
        printf("======\n");
        patron->movie = reservation->movie;
        while(numOfSeats <= 0 || numOfSeats > maxReservedSeats){
            printf("Number of Seats: ");
            fflush(stdin);
            scanf("%d",&numOfSeats);
            if(numOfSeats > maxReservedSeats){
                printf("Patrons can only reserve up to %d seats.\n",maxReservedSeats);
            }
        }
        patron->takenSeats = malloc(numOfSeats * sizeof(SeatNumber));
        patron->numOfTakenSeats = numOfSeats;
        for(i = 0; i < numOfSeats; i++){
            printf("\nSeat # %d\n",i+1);
            printf("Seat Letter: ");
            fflush(stdin);
            scanf("%s",buffer);
            printf("Seat Number: ");
            fflush(stdin);
            scanf("%d",&number);
            letter = buffer[0];
            seat = getTakenSeat(letter, number, reservation);
            if(seat != NULL){
                if(seat->taken == false){
                    printf("Seat successfully reserved!");
                    seat->taken = true;
                    patron->takenSeats[i] = *seat;
                }else{
                    if(seat->taken == true){
                        printf("Seat is already taken!\n");
                        displayTakenSeats(reservation->takenSeats);
                    }
                }
            }else{
                printf("Seat does not exist!");
            }
        }
    }else{
        printf("Seats for this movie are already full!\n");
    }
}

// Print patron data to a text file.
void printTicket(Patron* patron){
    FILE *fp;
    int i;

    fp = fopen(TicketFilename,"w");
    fprintf(fp,"%s\n",patron->movie->title);
    printf("%s\n",patron->movie->title);
    fprintf(fp,"%d:%d\n",patron->movie->showTime.hour,patron->movie->showTime.minute);
    printf("%d:%d\n",patron->movie->showTime.hour,patron->movie->showTime.minute);
    for(i = 0; i < patron->numOfTakenSeats; i++){
        if(i < patron->numOfTakenSeats - 1){
            fprintf(fp,"%c%d, ",patron->takenSeats[i].letter,patron->takenSeats[i].number);
            printf("%c%d, ",patron->takenSeats[i].letter,patron->takenSeats[i].number);
        }else{
            fprintf(fp,"%c%d\n",patron->takenSeats[i].letter,patron->takenSeats[i].number);
            printf("%c%d\n",patron->takenSeats[i].letter,patron->takenSeats[i].number);
        }
    }
    fclose(fp);
}

// Print reservation data to a text file.
void saveAndExit(Reservation* reservations, int numOfMovies, char* DateFilename){
    FILE *fp;
    int i, j;
    fp = fopen(DateFilename,"w");

    for(i = 0; i < numOfMovies; i++){
        fprintf(fp, "Title: %s\n", reservations[i].movie->title);
        fprintf(fp, "Time: %d:%d\n", reservations[i].movie->showTime.hour,reservations[i].movie->showTime.minute);
        fprintf(fp, "Taken Seats:\n"); 
        for(j = 0; j < maxSeats ; j++){
            if(reservations[i].takenSeats[j].taken){
                fprintf(fp, "%c%d, ", reservations[i].takenSeats[j].letter, reservations[i].takenSeats[j].number); 
            }
            if(j == maxSeats - 1)
                fprintf(fp, "\n"); 
        }
    }

    fclose(fp);
}


int main() {
    Movie* movies = NULL;
    Reservation* reservations = NULL;
    Patron* patron = malloc(maxPatrons * sizeof(Patron));
    char DateFilename[stringLength];
    char* day = malloc(2 * sizeof(char));
    char* month = malloc(2 * sizeof(char));
    char* year = malloc(4 * sizeof(char));
    int numOfMovies = 0;
    int choice = 0;
    movies = readMovieSched(movies, &numOfMovies);
    reservations  = initReservations(reservations, movies, numOfMovies);

    if(movies != NULL){
        while(choice != 5){
            choice = displayChoices();
            switch (choice){
                case 1:
                    free(movies);
                    free(reservations);
                    free(patron);
                    movies = NULL;
                    reservations = NULL;
                    patron = malloc(maxPatrons * sizeof(Patron));
                    numOfMovies = 0;
                    movies = readMovieSched(movies,&numOfMovies);
                    reservations  = initReservations(reservations, movies, numOfMovies);
                    printf("\n=== LOAD SCHEDULE ===\n");
                    printf("Loaded schedules from \"%s\"!",MovieSchedFilename);
                    printf("\n======\n\n");
                break;
                case 2:
                    printf("\n=== VIEW SCHEDULE ===\n");
                    displayMovies(movies,numOfMovies);
                    printf("======\n");
                    displayReservations(reservations,numOfMovies);
                    printf("======\n\n");
                break;
                case 3:
                    printf("\n=== RESERVE SEAT ===\n");
                        displayMovies(movies, numOfMovies);
                        printf("======\n");
                        reserveSeat(reservations, patron, numOfMovies);
                    printf("\n======\n\n");
                break;
                case 4:
                    printf("\n=== PRINT TICKET ===\n");
                    if(patron->movie != NULL){
                        printTicket(patron);
                        printf("======\n");
                        printf("Ticket printed to \"%s\"!",TicketFilename);
                    }else{
                        printf("No data entered.");
                    }
                    printf("\n======\n\n");
                break;
                case 5:
                    printf("\n=== SAVE AND EXIT ===\n");
                        printf("Current day (dd):");
                        scanf("%s", day);
                        printf("Current month (mm):");
                        scanf("%s", month);
                        printf("Current year (yyyy):");
                        scanf("%s", year);
                        strcpy(DateFilename, "show-");
                        strcat(DateFilename, day);
                        strcat(DateFilename, "-");
                        strcat(DateFilename, month);
                        strcat(DateFilename, "-");
                        strcat(DateFilename, year);
                        strcat(DateFilename, ".txt");
                        printf("Saved to \"%s\"",DateFilename);
                        saveAndExit(reservations,numOfMovies,DateFilename);
                    printf("\n======\n\n");
                break;
            }
        }
    }

    return 0;
}