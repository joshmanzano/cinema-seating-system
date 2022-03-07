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


int main(){
    char buffer[255];
    printf("Hello world!");

    fgets(buffer, 255, stdin);

    printf("%s", buffer);

    return 0;

}