/********
 * Author: Andrew Adams #0828800
 * Contact: aadams03@mail.uoguelph.ca
 * 
caltool.c -- Source code for calTool functions
Last updated:  Feb 22/2016
********/

#include "caltool.h"
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calutil.h"

static int lineCount = 0;

/* Count VEVENT components in comp
 * 
 * Arguments: initialized CalComp structure
 * 
 * Preconditions: *comp must be initialized 
 * Postconditions: none
 * 
 * Return val: number of VEVENT components in comp
 * */
int countVEvent (const CalComp *comp);

/* Count VTODO components in comp
 * 
 * Arguments: initialized CalComp structure
 * 
 * Preconditions: *comp must be initialized 
 * Postconditions: none
 * 
 * Return val: number of VTODO components in comp
 * */
int countToDo (const CalComp *comp);

/* Count other components in comp
 * 
 * Arguments: initialized CalComp structure
 * 
 * Preconditions: *comp must be initialized 
 * Postconditions: none
 * 
 * Return val: number of other components in comp
 * */
int countOther (const CalComp *comp);

/* Count subcomponents in comp
 * 
 * Arguments: initialized CalComp structure
 * 
 * Preconditions: *comp must be initialized 
 * Postconditions: none
 * 
 * Return val: number of subcomponents in comp
 * */
int countSubComponents (const CalComp *comp);

/* Count total number of properties in comp
 * 
 * Arguments: initialized CalComp structure
 * 
 * Preconditions: *comp must be initialized 
 * Postconditions: none
 * 
 * Return val: total number of properties in comp
 * */
int countProperties (const CalComp *comp);

/* Print sorted organizer names and date range in comp
 * 
 * Arguments: initialized CalComp structure
 * 
 * Preconditions: *comp must be initialized 
 * Postconditions: none
 * 
 * Return val: IOERR if fprintf fails, OK otherwise
 * */
CalStatus sortedCommonNames (const CalComp *comp, FILE *const txtfile );

/* Compares two strings and checks which comes first alphabetically
 * 
 * Arguments: both a and b are initialized char * strings
 * 
 * Preconditions:  both a and b are initialized char * strings
 * Postconditions: none
 * 
 * Return val: positive int if a comes first alphabetically, negative int if b comes first alphabetically
 * */
int compareStrings(const void *a, const void *b);

/* Compares two dates and check which comes first
 * 
 * Arguments: both a and b are initialized struct tm * variables
 * 
 * Preconditions:   both a and b are initialized struct tm * variables
 * Postconditions: none
 * 
 * Return val: 1 if a comes first, -1 if b comes first
 * */
int compareDates (const void *a, const void *b);

int main(int argc, char *argv[]){
    
    FILE * combineFile;
    struct tm * start, * end;
    time_t today;
    CalComp * pcomp, * pcomp2;
    CalStatus status;
    
    status.code = OK;
    status.linefrom = lineCount;
    status.lineto = lineCount;
    
    /* If user wants to run calInfo */
    if (argc == 2 && strcmp(argv[1], "-info") == 0){
        
        pcomp = NULL;
        status = readCalFile(stdin, &pcomp);
        
        /* Check if readCalFile returned successfully, prints an error on stderr if readCalFile returns an error */
        if (status.code != OK){

			if (status.code == AFTEND)
				fprintf(stderr, "Error: AFTEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
			if (status.code == BADVER)
				fprintf(stderr, "Error: BADVER reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == BEGEND)
				fprintf(stderr, "Error: BEGEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == IOERR)
				fprintf(stderr, "Error: IOERR reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCAL)
				fprintf(stderr, "Error: NOCAL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCRNL)
				fprintf(stderr, "Error: NOCRNL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NODATA)
				fprintf(stderr, "Error: NODATA reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOPROD)
				fprintf(stderr, "Error: NOPROD reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SUBCOM)
				fprintf(stderr, "Error: SUBCOM reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SYNTAX)
				fprintf(stderr, "Error: SYNTAX reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
            return EXIT_FAILURE;
        }
        
        status = calInfo(pcomp, status.lineto, stdout);
        
        freeCalComp(pcomp);
    }
    
    /* If user wants to run calExtract with kind set to events */
    else if (argc == 3 && strcmp(argv[1], "-extract") == 0 && strcmp(argv[2], "e") == 0){
        
        pcomp = NULL;
        status = readCalFile(stdin, &pcomp);
        
		/* Check if readCalFile returned successfully, prints an error on stderr if readCalFile returns an error */
        if (status.code != OK){

			if (status.code == AFTEND)
				fprintf(stderr, "Error: AFTEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
			if (status.code == BADVER)
				fprintf(stderr, "Error: BADVER reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == BEGEND)
				fprintf(stderr, "Error: BEGEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == IOERR)
				fprintf(stderr, "Error: IOERR reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCAL)
				fprintf(stderr, "Error: NOCAL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCRNL)
				fprintf(stderr, "Error: NOCRNL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NODATA)
				fprintf(stderr, "Error: NODATA reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOPROD)
				fprintf(stderr, "Error: NOPROD reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SUBCOM)
				fprintf(stderr, "Error: SUBCOM reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SYNTAX)
				fprintf(stderr, "Error: SYNTAX reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				        
            return EXIT_FAILURE;
        }
        
        status = calExtract(pcomp, OEVENT, stdout);
        
        freeCalComp(pcomp);
    }
    
    /* If user wants to run calExtract with kind set to x-properties */
    else if (argc == 3 && strcmp(argv[1], "-extract") == 0 && strcmp(argv[2], "x") == 0){
        
        pcomp = NULL;
        status = readCalFile(stdin, &pcomp);
        
		/* Check if readCalFile returned successfully, prints an error on stderr if readCalFile returns an error */
        if (status.code != OK){
        
			if (status.code == AFTEND)
				fprintf(stderr, "Error: AFTEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
			if (status.code == BADVER)
				fprintf(stderr, "Error: BADVER reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == BEGEND)
				fprintf(stderr, "Error: BEGEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == IOERR)
				fprintf(stderr, "Error: IOERR reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCAL)
				fprintf(stderr, "Error: NOCAL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCRNL)
				fprintf(stderr, "Error: NOCRNL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NODATA)
				fprintf(stderr, "Error: NODATA reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOPROD)
				fprintf(stderr, "Error: NOPROD reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SUBCOM)
				fprintf(stderr, "Error: SUBCOM reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SYNTAX)
				fprintf(stderr, "Error: SYNTAX reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				        
            return EXIT_FAILURE;
        }
        
        status = calExtract(pcomp, OPROP, stdout);
        
        freeCalComp(pcomp);
    }
    
    /* If user wants to run calFilter without dates */
    else if (argc == 3 && strcmp(argv[1], "-filter") == 0 && (strcmp(argv[2], "t") == 0 || strcmp(argv[2], "e") == 0)){
        
        pcomp = NULL;
        status = readCalFile(stdin, &pcomp);
        
        if (status.code != OK){

			if (status.code == AFTEND)
				fprintf(stderr, "Error: AFTEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
			if (status.code == BADVER)
				fprintf(stderr, "Error: BADVER reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == BEGEND)
				fprintf(stderr, "Error: BEGEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == IOERR)
				fprintf(stderr, "Error: IOERR reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCAL)
				fprintf(stderr, "Error: NOCAL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCRNL)
				fprintf(stderr, "Error: NOCRNL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NODATA)
				fprintf(stderr, "Error: NODATA reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOPROD)
				fprintf(stderr, "Error: NOPROD reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SUBCOM)
				fprintf(stderr, "Error: SUBCOM reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SYNTAX)
				fprintf(stderr, "Error: SYNTAX reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);

            return EXIT_FAILURE;
        }
        
        /* Run with content set to OTODO or OEVENT depending on what the user specified */
		if (strcmp(argv[2], "t") == 0)
			status = calFilter(pcomp, OTODO, 0, 0, stdout);
		
		else if (strcmp(argv[2], "e") == 0)
			status = calFilter(pcomp, OEVENT, 0, 0, stdout);
			
        freeCalComp(pcomp);
    }
    
    /* If user wants to run calFilter but supplied an invalid argument for content */
    else if (argc >= 3 && strcmp(argv[1], "-filter") == 0 && strcmp(argv[2], "t") != 0 && strcmp(argv[2], "e") != 0){

		fprintf(stderr, "Error: content option can only be t (todo) or e (event)\n");
		
		return EXIT_FAILURE; 
    }
    
    /* If user wants to run calFilter with dates */
    else if (argc > 3 && strcmp(argv[1], "-filter") == 0 && (strcmp(argv[2], "t") == 0 || strcmp(argv[2], "e") == 0)){
		
		/* If user provided to many cmd line arguments, print an error on stderr and return EXIT_FAILURE */
		if (argc > 7){
			
			fprintf(stderr, "Error: too many command line arguments. Correct syntax is: caltool -filter content [from date ] [to date ]\n");
			
			return EXIT_FAILURE;
		}
		
		pcomp = NULL;
        status = readCalFile(stdin, &pcomp);
        
		/* Check if readCalFile returned successfully, prints an error on stderr if readCalFile returns an error */
        if (status.code != OK){
        
			if (status.code == AFTEND)
				fprintf(stderr, "Error: AFTEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
			if (status.code == BADVER)
				fprintf(stderr, "Error: BADVER reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == BEGEND)
				fprintf(stderr, "Error: BEGEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == IOERR)
				fprintf(stderr, "Error: IOERR reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCAL)
				fprintf(stderr, "Error: NOCAL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCRNL)
				fprintf(stderr, "Error: NOCRNL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NODATA)
				fprintf(stderr, "Error: NODATA reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOPROD)
				fprintf(stderr, "Error: NOPROD reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SUBCOM)
				fprintf(stderr, "Error: SUBCOM reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SYNTAX)
				fprintf(stderr, "Error: SYNTAX reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
            return EXIT_FAILURE;
        }
			
		start = malloc(sizeof(struct tm));
		assert(start);
		
		start->tm_sec = 0;
		start->tm_min = 0;
		start->tm_hour = 0;
		start->tm_mday = 0;
		start->tm_mon = 0;
		start->tm_year = 0;
		start->tm_wday = 0;
		start->tm_yday = 0;
		start->tm_isdst = -1;
		
		end = malloc(sizeof(struct tm));
		assert(end);
		
		end->tm_sec = 0;
		end->tm_min = 0;
		end->tm_hour = 0;
		end->tm_mday = 0;
		end->tm_mon = 0;
		end->tm_year = 0;
		end->tm_wday = 0;
		end->tm_yday = 0;
		end->tm_isdst = -1;
		
		/* If user set today as the start date and specified an end date */
		if (argc == 7 && strcmp(argv[3], "from") == 0 && strcmp(argv[4], "today") == 0 && strcmp(argv[5], "to") == 0 && getdate_r(argv[6], end) == 0){
			
			today = time(NULL);
			free(start);
			start = localtime(&today);	
			
            start->tm_sec = 0;
			start->tm_min = 0;
			start->tm_hour = 0;
            
			end->tm_sec = 0;
			end->tm_min = 59;
			end->tm_hour = 23;
			
			/* Check if start date is before end date, print an error and return EXIT_FAILURE if so */
			if (mktime(start) > mktime(end)){
				
				fprintf(stderr, "Error: filter start date is not before end date.\n");
				freeCalComp(pcomp);
				free(end);
				return EXIT_FAILURE;
			}	
			
			/* Run with content set to OTODO or OEVENT depending on what the user specified */
			if (strcmp(argv[2], "t") == 0)
				status = calFilter(pcomp, OTODO, mktime(start), mktime(end), stdout);
			
			else if (strcmp(argv[2], "e") == 0)
				status = calFilter(pcomp, OEVENT, mktime(start), mktime(end), stdout);
				
			free(end);
		}
		
		/* If user specified start date and set today as the end date */
		else if (argc == 7 && strcmp(argv[3], "from") == 0 && getdate_r(argv[4], start) == 0 && strcmp(argv[5], "to") == 0 && strcmp(argv[6], "today") == 0){
			
			today = time(NULL);
			free(end);
			end = localtime(&today);
			
			start->tm_sec = 0;
			start->tm_min = 0;
			start->tm_hour = 0;
            
            end->tm_sec = 0;
			end->tm_min = 59;
			end->tm_hour = 23;
			
			/* Run with content set to OTODO or OEVENT depending on what the user specified */
			if (mktime(start) > mktime(end)){
				
				fprintf(stderr, "Error: filter start date is not before end date.\n");
				freeCalComp(pcomp);
				free(start);
				return EXIT_FAILURE;
			}	
			
			/* Run with content set to OTODO or OEVENT depending on what the user specified */
			if (strcmp(argv[2], "t") == 0)
				status = calFilter(pcomp, OTODO, mktime(start), mktime(end), stdout);
			
			else if (strcmp(argv[2], "e") == 0)
				status = calFilter(pcomp, OEVENT, mktime(start), mktime(end), stdout);
				
			free(start);
			
		}
		
		/* If user specified start date and end date */
		else if (argc == 7 && strcmp(argv[3], "from") == 0 && getdate_r(argv[4], start) == 0 && strcmp(argv[5], "to") == 0 && getdate_r(argv[6], end) == 0){
			
			start->tm_sec = 0;
			start->tm_min = 0;
			start->tm_hour = 0;
			
			end ->tm_sec = 0;
			end->tm_min = 59;
			end->tm_hour = 23;
			
			/* Run with content set to OTODO or OEVENT depending on what the user specified */
			if (mktime(start) > mktime(end)){
				
				fprintf(stderr, "Error: filter start date is not before end date.\n");
				freeCalComp(pcomp);
				free(start);
				free(end);
				return EXIT_FAILURE;
			}				
			
			/* Run with content set to OTODO or OEVENT depending on what the user specified */
			if (strcmp(argv[2], "t") == 0)
				status = calFilter(pcomp, OTODO, mktime(start), mktime(end), stdout);
			
			else if (strcmp(argv[2], "e") == 0)
				status = calFilter(pcomp, OEVENT, mktime(start), mktime(end), stdout);
				
			free(start);
			free(end);
		}	 
		
		/* If user specified start date */
		else if (argc == 5 && strcmp(argv[3], "from") == 0 && getdate_r(argv[4], start) == 0){
			
			start->tm_sec = 0;
			start->tm_min = 0;
			start->tm_hour = 0;			
		
			/* Run with content set to OTODO or OEVENT depending on what the user specified */
			if (strcmp(argv[2], "t") == 0)
				status = calFilter(pcomp, OTODO, mktime(start), 0, stdout);
			
			else if (strcmp(argv[2], "e") == 0)
				status = calFilter(pcomp, OEVENT, mktime(start), 0, stdout);
				
			free(start);
			free(end);
		}
		
		/* If user specified end date */
		else if (argc == 5 && strcmp(argv[3], "to") == 0 && getdate_r(argv[4], start) == 0){
		
			start->tm_sec = 0;
			start->tm_min = 0;
			start->tm_hour = 0;
		
			/* Run with content set to OTODO or OEVENT depending on what the user specified */
			if (strcmp(argv[2], "t") == 0)
				status = calFilter(pcomp, OTODO, 0, mktime(start), stdout);
			
			else if (strcmp(argv[2], "e") == 0)
				status = calFilter(pcomp, OEVENT, 0, mktime(start), stdout);
							
			free(start);
			free(end);
		}
		
		/* If user set start date as today */
		else if (argc == 5 && strcmp(argv[3], "from") == 0 && strcmp(argv[4], "Today") == 0){
			
			today = time(NULL);
			free(start);
			start = localtime(&today);
            
            start->tm_sec = 0;
            start->tm_min = 0;
            start->tm_hour = 0;
            
			end->tm_sec = 0;
			end->tm_min = 59;
			end->tm_hour = 23;
			
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            
			/* Run with content set to OTODO or OEVENT depending on what the user specified */
			if (strcmp(argv[2], "t") == 0)
				status = calFilter(pcomp, OTODO, mktime(start), 0, stdout);
			
			else if (strcmp(argv[2], "e") == 0)
				status = calFilter(pcomp, OEVENT, mktime(start), 0, stdout);
				
			free(end);
		}
		
		/* If user set end date as today */
		else if (argc == 5 && strcmp(argv[3], "to") == 0 && strcmp(argv[4], "today") == 0){
		
			today = time(NULL);
			free(end);
			end = localtime(&today);
			
			start->tm_sec = 0;
			start->tm_min = 0;
			start->tm_hour = 0;
			
			/* Run with content set to OTODO or OEVENT depending on what the user specified */
			if (strcmp(argv[2], "t") == 0)
				status = calFilter(pcomp, OTODO, mktime(start), mktime(end), stdout);
			
			else if (strcmp(argv[2], "e") == 0)
				status = calFilter(pcomp, OEVENT, mktime(start), mktime(end), stdout);
			
			free(start);
		}
		
		/* If user entered a bad start or end date */
		else if (argc == 7 && (getdate_r(argv[4], start) != 0 || getdate_r(argv[6], end) != 0 || strcmp(argv[3], "from") != 0 || strcmp(argv[5], "to") != 0)){
			
			/* Print an error to stderror depending on the return value of getdate_r */
			if (getdate_r(argv[4], start) <= 5 || getdate_r(argv[6], end) <= 5)
				fprintf(stderr, "Error: Problem with DATEMSK environment variable or template file (error codes 1-5)\n");
				
			if (getdate_r(argv[4], start) >= 7)
				fprintf(stderr, "Error: Date \"%s\" could not be interpreted (7-8).\n", argv[4]);
				
			if (getdate_r(argv[6], end) >= 7)
				fprintf(stderr, "Error: Date \"%s\" could not be interpreted (7-8).\n", argv[6]);

			free(start);
			free(end);
            
            return EXIT_FAILURE;
		}
		
		/* If user entered a bad start date */
		else if (argc == 5 && getdate_r(argv[4], start) != 0){
			
			if (getdate_r(argv[4], start) <= 5)
				fprintf(stderr, "Error: Problem with DATEMSK environment variable or template file (error codes 1-5)\n");
		
			if (getdate_r(argv[4], start) >= 7)
				fprintf(stderr, "Error: Date \"%s\" could not be interpreted (7-8).\n", argv[4]);

			free(start);
			free(end);
            
            return EXIT_FAILURE;
		}
        
        else{
            
            fprintf(stderr, "Error: invalid arguments. Please try again\n");
            
            free(start);
            free(end);
            
            return EXIT_FAILURE;
        }
		
		freeCalComp(pcomp);
	}
	
	/* If user wants to run calCombine */
	else if (argc == 3 && strcmp(argv[1], "-combine") == 0){
		
        pcomp = NULL;
        status = readCalFile(stdin, &pcomp);
        
        /* Check if readCalFile returned successfully, prints an error on stderr if readCalFile returns an error */
        if (status.code != OK){
        
			if (status.code == AFTEND)
				fprintf(stderr, "Error: AFTEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
			if (status.code == BADVER)
				fprintf(stderr, "Error: BADVER reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == BEGEND)
				fprintf(stderr, "Error: BEGEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == IOERR)
				fprintf(stderr, "Error: IOERR reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCAL)
				fprintf(stderr, "Error: NOCAL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOCRNL)
				fprintf(stderr, "Error: NOCRNL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NODATA)
				fprintf(stderr, "Error: NODATA reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == NOPROD)
				fprintf(stderr, "Error: NOPROD reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SUBCOM)
				fprintf(stderr, "Error: SUBCOM reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
			
			if (status.code == SYNTAX)
				fprintf(stderr, "Error: SYNTAX reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
            return EXIT_FAILURE;
        }
        
        /* Open the second file */
        pcomp2 = NULL;
        combineFile = fopen(argv[2], "r");
        
        /* Check if file opened successfully */
        if (combineFile != NULL){
			
			status = readCalFile(combineFile, &pcomp2);
			
			/* Check if readCalFile returned successfully, prints an error on stderr if readCalFile returns an error */
			if (status.code != OK){

				if (status.code == AFTEND)
					fprintf(stderr, "Error: AFTEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
					
				if (status.code == BADVER)
					fprintf(stderr, "Error: BADVER reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
				if (status.code == BEGEND)
					fprintf(stderr, "Error: BEGEND reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
				if (status.code == IOERR)
					fprintf(stderr, "Error: IOERR reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
				if (status.code == NOCAL)
					fprintf(stderr, "Error: NOCAL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
				if (status.code == NOCRNL)
					fprintf(stderr, "Error: NOCRNL reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
				if (status.code == NODATA)
					fprintf(stderr, "Error: NODATA reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
				if (status.code == NOPROD)
					fprintf(stderr, "Error: NOPROD reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
				if (status.code == SUBCOM)
					fprintf(stderr, "Error: SUBCOM reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
				if (status.code == SYNTAX)
					fprintf(stderr, "Error: SYNTAX reported by readCalFile, linefrom = %d, lineto = %d\n", status.linefrom, status.lineto);
				
				free(pcomp);
				return EXIT_FAILURE;
			}
			
			status = calCombine(pcomp, pcomp2, stdout);
			
			/* Close the file and free both CalComp's */
			fclose(combineFile); 
			freeCalComp(pcomp);
			freeCalComp(pcomp2);
		}
		
		/* If file failed to open print an error, free pcomp and return EXIT_FAILURE */
		else{
			
            if (argc >= 3)
                fprintf(stderr, "Error: Unable to open file %s\n", argv[2]);
                
			freeCalComp(pcomp);
			
			return EXIT_FAILURE;
		}
	}
	
	/* Otherwise, print error and let the user know what the proper syntax is */
	else{
		
		fprintf(stderr, "Error: invalid syntax, please use one of the following commands.\n");
		fprintf(stderr, "caltool -info\n");
		fprintf(stderr, "caltool -extract kind\n");
		fprintf(stderr, "caltool -filter content [from date ] [to date ]\n");
		fprintf(stderr, "caltool -combine file2\n");
        
        return EXIT_FAILURE;
	}
    
    /* If any subcomponent calls returned an error print an error to stderr and return EXIT_FAILURE */
    if (status.code != OK){
		
		if (status.code == IOERR){
			
			fprintf(stderr, "Error: IOERR received\n");
		}
		
		else if (status.code == NOCAL){
			
			fprintf(stderr, "Error: NOCAL received from calFilter\n");
		}
		
		return EXIT_FAILURE;
	}
    
    /* Otherwise return EXIT_SUCCESS */
	else{
		
		return EXIT_SUCCESS;
	}
}

CalStatus calInfo( const CalComp *comp, int lines, FILE *const txtfile ){
    
    CalStatus status;
    
    /* Print poper grammer for lines and check fprintf return val. */
    if (lines != 1){
        
        /* Check if we wrote to txtfile succesfully */
        if (fprintf(txtfile, "%d lines\n", lines) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;
		}
			
		++lineCount;
    }
    
    else{
        
        /* Check if we wrote to txtfile succesfully */
        if (fprintf(txtfile, "%d line\n", lines) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;
		}
		
		++lineCount;
    }
    
    /* Print proper grammar for component count and check fprint return val */
    if (comp->ncomps != 1){
        
        /* Check if we wrote to txtfile succesfully */
        if (fprintf(txtfile, "%d components: ", comp->ncomps) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;	
		}
		
		++lineCount;
    }
    
    else{
        
        /* Check if we wrote to txtfile succesfully */
        if (fprintf(txtfile, "%d component: ", comp->ncomps) < 0){
		
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;	
		}
		
		++lineCount;
    }
    
    /* Print proper grammar for VEVENT count and check fprint return val */
    if (countVEvent(comp) != 1){
        
        /* Check if we wrote to txtfile succesfully */
        if (fprintf(txtfile, "%d events, ", countVEvent(comp)) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;
		}
		
		++lineCount;
    }

	 
    else{
			
		/* Check if we wrote to txtfile succesfully */   
        if (fprintf(txtfile, "%d event, ", countVEvent(comp)) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;
		}
		
		++lineCount;
    }
    
    if (countToDo(comp) != 1){
    
        /* Print proper grammar for VTODO count and check fprint return val */
        if (fprintf(txtfile, "%d todos, ", countToDo(comp)) < 0){
        
            status.code = IOERR;
            status.linefrom = lineCount;
            status.lineto = lineCount;
                
            return status;
        } // CHECK GRAMMAR FROM PROF
    }
    
    else{
        
        /* Print proper grammar for VTODO count and check fprint return val */
        if (fprintf(txtfile, "%d todo, ", countToDo(comp)) < 0){
        
            status.code = IOERR;
            status.linefrom = lineCount;
            status.lineto = lineCount;
                
            return status;
        } // CHECK GRAMMAR FROM PROF
    }
    
    ++lineCount;
    
    /* Print proper grammar for other count and check fprint return val */
    if (countOther(comp) != 1){
        
		/* Check if we wrote to txtfile succesfully */           
        if (fprintf(txtfile, "%d others\n", countOther(comp)) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;
		}
		
		++lineCount;
    }
    
    else{
        
        /* Check if we wrote to txtfile succesfully */   
        if (fprintf(txtfile, "%d other\n", countOther(comp)) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;
		}
		
		++lineCount;
    }
    
    /* Print proper grammar for subcomponent count and check fprint return val */
    if (countSubComponents(comp) != 1){
    
		/* Check if we wrote to txtfile succesfully */   
		if (fprintf(txtfile, "%d subcomponents\n", countSubComponents(comp)) < 0){
		
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;
		}
		
		++lineCount;
    }
    
    else{
    
		/* Check if we wrote to txtfile succesfully */   
		if (fprintf(txtfile, "%d subcomponent\n", countSubComponents(comp)) < 0){
		   
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;
		}
		
		++lineCount;
    }
    
    /* Print proper grammar for property count and check fprint return val */
    if (countProperties(comp) + comp->nprops != 1){
        
		/* Check if we wrote to txtfile succesfully */   
        if (fprintf(txtfile, "%d properties\n", countProperties(comp) + comp->nprops) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;
		}
		
		++lineCount;
    }
    
    else{
        
        /* Check if we wrote to txtfile succesfully */   
        if (fprintf(txtfile, "%d property\n", countProperties(comp) + comp->nprops) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			
			return status;
		}
		
		++lineCount;
    }
    
    status = sortedCommonNames(comp, txtfile); // Print date range and organizers
    
    return status;
}

CalStatus sortedCommonNames (const CalComp *comp, FILE *const txtfile ){
    
    CalStatus status;
    bool foundOldest, foundNewest;
    char ** toReturn;
    int i, y, l, nameCount;
    CalProp * currentProp;
    CalParam * currentParam;
    struct tm * oldest, * newest, * temp;
    nameCount = 0;
    char printDate[MAXSTRINGLENGTH];
    
    foundOldest = false;
    foundNewest = false;
    
    oldest = NULL;
    newest = NULL;
    
    temp = malloc(sizeof(struct tm));
    assert(temp);

    toReturn = malloc(sizeof(char*));
    assert(toReturn != NULL);
    
    currentProp = comp->prop;
    
    /* Iterate through all properties */
    while (currentProp != NULL){
        
        currentParam = currentProp->param;
        
        /* Iterate through all paramters in current property */
        while (currentParam != NULL){
            
            /* Check if currentProp is an organizer property and name of currentParam is CN */
            if (strcmp(currentProp->name, "ORGANIZER") == 0 && strcmp(currentParam->name, "CN") == 0){
             
				/* Iterate through all param values and add them to the array */
                for (i = 0; i < currentParam->nvalues; ++i){

                    if (nameCount != 0){
                        
                        toReturn = realloc(toReturn, sizeof(char *) * (nameCount + 1));
                        assert(toReturn);
                    }
                    
                    toReturn[nameCount] = malloc(sizeof(char) * (strlen(currentParam->value[i]) + 1));
                    assert(toReturn[nameCount]);
                    
                    strcpy(toReturn[nameCount],currentParam->value[i]);
                    ++nameCount;
                }
            }
            
            currentParam = currentParam->next; // Iterate to next param
        }
        
        currentProp = currentProp->next; // Iterate to next prop
    }
    
    /* Iterate through all components in top level */
    for (i = 0; i < comp->ncomps; ++i){
        
        currentProp = comp->comp[i]->prop;
    
		/* Iterate through all properties */
        while (currentProp != NULL){
            
            currentParam = currentProp->param;
            
            /* If current prop is a recognized date prop */
            if (strcmp(currentProp->name, "COMPLETED") == 0 || strcmp(currentProp->name, "DTEND") == 0 || strcmp(currentProp->name, "DUE") == 0 || strcmp(currentProp->name, "DTSTART") == 0 ||
            strcmp(currentProp->name, "CREATED") == 0 || strcmp(currentProp->name, "DTSTAMP") == 0 || strcmp(currentProp->name, "LAST-MODIFIED") == 0){
            
				/* If we haven't found any date properties yet, parse value and store it */
                if (foundOldest == false){
                    
                    temp->tm_sec = 0;
                    temp->tm_min = 0;
                    temp->tm_hour = 0;
                    temp->tm_mday = 0;
                    temp->tm_mon = 0;
                    temp->tm_year = 0;
                    temp->tm_wday = 0;
                    temp->tm_yday = 0;
                    temp->tm_isdst = -1;
                    
                    strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
                   
                    oldest = temp;
                    temp = NULL;
                    
                    foundOldest = true;
                }
                
                /* If we found the oldest date but not the newest*/
                else if (foundOldest == true && foundNewest == false){
                    
                    temp = malloc(sizeof(struct tm));
                    assert(temp);
                
                    temp->tm_sec = 0;
                    temp->tm_min = 0;
                    temp->tm_hour = 0;
                    temp->tm_mday = 0;
                    temp->tm_mon = 0;
                    temp->tm_year = 0;
                    temp->tm_wday = 0;
                    temp->tm_yday = 0;
                    temp->tm_isdst = -1;
                    
                    strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
                    
                    /* Check if we've found the newest date */
                    if (mktime(temp) > mktime(oldest)){
                        
                        newest = temp;
                        temp = NULL;
                        
                        foundNewest = true;
                    }
                    
                    /* Check if we've found the oldest date */
                    else if (mktime(temp) < mktime(oldest)){
                        
                        free(oldest);
                        
                        oldest = temp;
                        temp = NULL;
                    }
                    
                    /* Otherwise free the temp struct tm */
                    else{
                        
                        free(temp);
                        temp = NULL;
                    }
                }
                
                /* Otherwise we already have both the oldest and newest dates found so far */
                else{
                    
                    temp = malloc(sizeof(struct tm));
                    assert(temp);
                    
                    temp->tm_sec = 0;
                    temp->tm_min = 0;
                    temp->tm_hour = 0;
                    temp->tm_mday = 0;
                    temp->tm_mon = 0;
                    temp->tm_year = 0;
                    temp->tm_wday = 0;
                    temp->tm_yday = 0;
                    temp->tm_isdst = -1;
                    
                    strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
                    
                    /* If we've found the newest date */
                    if (mktime(temp) > mktime(newest)){
                        
                        free(newest);
                        
                        newest = temp;
                        temp = NULL;
                    }
                    
                    /* If we've found the oldest date */
                    else if (mktime(temp) < mktime(oldest)){
                        
                        free(oldest);
                        
                        oldest = temp;
                        temp = NULL;
                    }
                    
					/* Otherwise free the temp struct tm */
                    else{
                        
                        free(temp);
                        temp = NULL;
                    }
                }
            }
            
			/* Iterate through all paramters in current property */
            while (currentParam != NULL){
                
				/* Check if currentProp is an organizer property and name of currentParam is CN */                
                if (strcmp(currentProp->name, "ORGANIZER") == 0 && strcmp(currentParam->name, "CN") == 0){
                 
					/* Iterate through all param values and add them to the array */
                    for (y = 0; y < currentParam->nvalues; ++y){
                        
                
                        if (nameCount != 0){
                            toReturn = realloc(toReturn, sizeof(char *) * (nameCount + 1));
                            assert(toReturn);
                        }
                        
                        toReturn[nameCount] = malloc(sizeof(char) * (strlen(currentParam->value[y]) + 1));
                        assert(toReturn[nameCount]);
                        
                        strcpy(toReturn[nameCount],currentParam->value[y]);
                        ++nameCount;
            
                    }
                }
                
                currentParam = currentParam->next;
            }
            
            currentProp = currentProp->next;
        }
        
		/* Iterate through all components at lowest level */
        for (y = 0; y < comp->comp[i]->ncomps; ++y){
            
            currentProp = comp->comp[i]->comp[y]->prop;
    
			/* Iterate through all properties */
            while (currentProp != NULL){
                
                currentParam = currentProp->param;
                
				/* If current prop is a recognized date prop */
                if (strcmp(currentProp->name, "COMPLETED") == 0 || strcmp(currentProp->name, "DTEND") == 0 || strcmp(currentProp->name, "DUE") == 0 || strcmp(currentProp->name, "DTSTART") == 0 ||
                strcmp(currentProp->name, "CREATED") == 0 || strcmp(currentProp->name, "DTSTAMP") == 0 || strcmp(currentProp->name, "LAST-MODIFIED") == 0){
            
					/* If we've found the oldest date */
                    if (foundOldest == false){
                        
                        temp->tm_sec = 0;
                        temp->tm_min = 0;
                        temp->tm_hour = 0;
                        temp->tm_mday = 0;
                        temp->tm_mon = 0;
                        temp->tm_year = 0;
                        temp->tm_wday = 0;
                        temp->tm_yday = 0;
                        temp->tm_isdst = -1;
                        
                        strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
                        
                        oldest = temp;
                        temp = NULL;
                        
                        foundOldest = true;
                    }
                    
					 /* If we found the oldest date but not the newest*/
                    else if (foundOldest == true && foundNewest == false){
                        
                        temp = malloc(sizeof(struct tm));
                        assert(temp);
                    
                        temp->tm_sec = 0;
                        temp->tm_min = 0;
                        temp->tm_hour = 0;
                        temp->tm_mday = 0;
                        temp->tm_mon = 0;
                        temp->tm_year = 0;
                        temp->tm_wday = 0;
                        temp->tm_yday = 0;
                        temp->tm_isdst = -1;
                        
                        strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
                        
                        /* If we've found the newest date */
                        if (mktime(temp) > mktime(oldest)){
                            
                            newest = temp;
                            temp = NULL;
                            
                            foundNewest = true;
                        }
                        
                        /* If we've found the oldest date */
                        else if (mktime(temp) < mktime(oldest)){
                            
                            free(oldest);
                            
                            oldest = temp;
                            temp = NULL;
                        }
                        
                        /* Otherwise free the temp struct tm */
                        else{
                            
                            free(temp);
                            temp = NULL;
                        }
                    }
                    
                    /* Otherwise we already have both the oldest and newest dates found so far*/
                    else{
                        
                        temp = malloc(sizeof(struct tm));
                        assert(temp);
                        
                        temp->tm_sec = 0;
                        temp->tm_min = 0;
                        temp->tm_hour = 0;
                        temp->tm_mday = 0;
                        temp->tm_mon = 0;
                        temp->tm_year = 0;
                        temp->tm_wday = 0;
                        temp->tm_yday = 0;
                        temp->tm_isdst = -1;
                        
                        strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
                        
                        /* If we've found the newest date */
                        if (mktime(temp) > mktime(newest)){
                            
                            free(newest);
                            
                            newest = temp;
                            temp = NULL;
                        }
                        
                        /* If we've found the oldest date */
                        else if (mktime(temp) < mktime(oldest)){
                            
                            free(oldest);
                            
                            oldest = temp;
                            temp = NULL;
                        }
                        
                        /* Otherwise free the temp struct tm */
                        else{
                            
                            free(temp);
                            temp = NULL;
                        }
                    }
                }
                
                /* Iterate through all parameters in current property */
                while (currentParam != NULL){
                    
                    /* Check if currentProp is an organizer property and name of currentParam is CN */
                    if (strcmp(currentProp->name, "ORGANIZER") == 0 && strcmp(currentParam->name, "CN") == 0){
                    
						/* Iterate through all param values and add them to the array */
                        for (l = 0; l < currentParam->nvalues; ++l){
                            
                            if (nameCount != 0){
                                toReturn = realloc(toReturn, sizeof(char *) * (nameCount + 1));
                                assert(toReturn);
                            }
                            
                            toReturn[nameCount] = malloc(sizeof(char) * (strlen(currentParam->value[l]) + 1));
                            assert(toReturn[nameCount]);
                            
                            strcpy(toReturn[nameCount],currentParam->value[l]);
                            ++nameCount;
                        }
                    }
                    
                    currentParam = currentParam->next;
                }
                
                currentProp = currentProp->next;
            }
        }
    }
    
    /* If we were able to find both the oldest and newest date for our date range */
    if (foundOldest == true && foundNewest == true){
        
		/* Check if we wrote to txtfile succesfully */
		if (fprintf(txtfile, "From ") < 0){
		
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;	
			
			return status;
		}
        
        ++lineCount;
        strftime(printDate, MAXSTRINGLENGTH, "%Y-%b-%d", oldest);
       
		/* Check if we wrote to txtfile succesfully */
		if (fprintf(txtfile, "%s ", printDate) < 0){
		   
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;	
			
			return status;
		}
        
        ++lineCount;
        strftime(printDate, MAXSTRINGLENGTH, "%Y-%b-%d", newest);
        
        /* Check if we wrote to txtfile succesfully */
        if (fprintf(txtfile, "to %s\n", printDate) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;	
			
			return status;			
		}
        
        ++lineCount;
        
        free(oldest);
        free(newest);
    }
    
    /* If we only found one date (the oldest) */
    else if (foundOldest == true && foundNewest == false){
        
        /* Check if we wrote to txtfile succesfully */
        if (fprintf(txtfile, "From ") < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;	
			
			return status;			
		}
        
        ++lineCount;
        strftime(printDate, MAXSTRINGLENGTH, "%Y-%b-%d", oldest);
        
        /* Check if we wrote to txtfile succesfully */
        if (fprintf(txtfile, "%s ", printDate) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;	
			
			return status;
		}
		
		++lineCount;
		
		/* Check if we wrote to txtfile succesfully */
        if (fprintf(txtfile, "to %s\n", printDate) < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;	
			
			return status;	
		}
        
        ++lineCount;
        
        free(oldest);
    }
    
    /* If no dates were found */
    else{
        
        /* Check if we wrote to txtfile succesfully */
        if (fprintf(txtfile, "No dates\n") < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;	
			
			return status;
		}
		
		++lineCount;
    }
    
    /* If no organizers were found */
    if (nameCount == 0){
		
		/* Check if we wrote to txtfile succesfully */
		if (fprintf(txtfile, "No organizers\n") < 0){
		
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;	
			
			return status;
		}
		
		++lineCount;
	}
	
	/* If atleast one organizer was found, sort organizer names alphabetically */
    else{
        
        qsort(toReturn, nameCount, sizeof(char *), compareStrings);
        
        /* Check if we wrote to txtfile succesfully */
        if (fprintf(txtfile, "Organizers:\n") < 0){
			
			status.code = IOERR;
			status.linefrom = lineCount;
			status.lineto = lineCount;	
			
			return status;	
		}
        
        ++lineCount;
        
        /* Iterate through all organizer names in array */
        for (i = 0; i < nameCount; ++i){
        
			/* Check if we wrote to txtfile succesfully */
            if (fprintf(txtfile, "%s\n", toReturn[i]) < 0){
				
				status.code = IOERR;
				status.linefrom = lineCount;
				status.lineto = lineCount;	
				
				return status;
			}
			
			++lineCount;
            
            /* Check if there are duplicates of the name just printed, skip to next unique name if so */
            for (y = i + 1; y < nameCount; ++y){
                
                if (strcmp(toReturn[i], toReturn[y]) == 0 && y == nameCount - 1){
					
					i = nameCount;
					break;
				}
				
                else if (strcmp(toReturn[i], toReturn[y]) == 0){
                    
                    continue;
                }
                
                else{
                    
                    i = y - 1;
                    break;
                }
            }
        }
    }
    
    /* Free contents of array */
    for (i = 0; i < nameCount; ++i){
        
        free(toReturn[i]);
    }
    
    free(toReturn); // Free the array itself
    
    /* Set status code and linefrom/lineto */
    status.code = OK;
    status.linefrom = lineCount;
    status.lineto = lineCount;
    
    return status;
}

int compareStrings (const void *a, const void *b){ 
    
    /* Cast parameters */
    const char ** castA = (const char **) a;
    const char ** castB = (const char **) b;
    
    return strcmp(*castA, *castB); // Compare and b as strings and call strmcp to check which comes first alphabetically 
} 

int compareDates (const void *a, const void *b){
    
    /* Cast parameters */
    struct tm * castA = (struct tm *) a;
    struct tm * castB = (struct tm *) b;
    
    /* If date a is newer than date b */
    if (mktime(castA) > mktime(castB))
        return 1;
        
	/* If date b is newer than date a */
    else if (mktime(castA) < mktime(castB))
        return -1;
       
    /* If the date a and b are the same */
    else 
        return 0;
}

int countProperties (const CalComp *comp){
    
    int count, i;
    
    count = 0;
    
    /* Iterate through all components recursively and sum all properties */
    for (i = 0; i < comp->ncomps; ++i){
		
        count += comp->comp[i]->nprops;
        count += countProperties(comp->comp[i]);
	}
    
    return count;
}

int countSubComponents (const CalComp *comp){
    
    int count, i;
    
    count = 0;
    
    /* Iterate through all subcomponents and count how many are found */
    for (i = 0; i < comp->ncomps; ++i){
		
		if (comp->comp[i]->ncomps != 0){
            
            count += comp->comp[i]->ncomps;
        }
	}
    
    return count;
}

int countVEvent (const CalComp *comp){
    
    int count, i;
    
    count = 0;
    
    /* Iterate through all components and count all VEVENTs */
    for (i = 0; i < comp->ncomps; ++i){
		
		if (strcmp(comp->comp[i]->name, "VEVENT") == 0){
            
            ++count;
        }
	}
    
    return count;
}

int countToDo (const CalComp *comp){
    
    int count, i;
    
    count = 0;
    
	/* Iterate through all components and count all VTODOs */
    for (i = 0; i < comp->ncomps; ++i){
		
		if (strcmp(comp->comp[i]->name, "VTODO") == 0){
            
            ++count;
        }
	}
    
    return count;
}

int countOther (const CalComp *comp){
    
    int count, i;
    
    count = 0;
    
    /* Iterate through all components and count all that are not VTODO/VEVENT */
    for (i = 0; i < comp->ncomps; ++i){
		
		if (strcmp(comp->comp[i]->name, "VTODO") != 0 && strcmp(comp->comp[i]->name, "VEVENT") != 0){
            
            ++count;
        }
	}
    
    return count;
}

CalStatus calExtract( const CalComp *comp, CalOpt kind, FILE *const txtfile){

    CalStatus status;
    struct  tm times[1000];
    char ** toReturn;
    char ** sortedNames;
    char printDate[MAXSTRINGLENGTH];
    int i, y, nameCount, timeCount;
    CalProp * currentProp;
    
    nameCount = 0;
    timeCount = 0;
        
    toReturn = malloc(sizeof(char*));
    assert(toReturn);
    
    currentProp = comp->prop;    
    
    /* Iterate through all properties*/
    while (currentProp != NULL){

        /* If we've run into an X-property or a summary property store it in our string array */
        if ((kind == OPROP && currentProp->name[0] == 'X' && currentProp->name[1] == '-') || (kind == OEVENT && strcmp(currentProp->name, "SUMMARY") == 0)){
            
            if (nameCount != 0){
                
                toReturn = realloc(toReturn, sizeof(char *) * (nameCount + 1));
                assert(toReturn);
            }
            
            if (kind == OPROP){
                
                toReturn[nameCount] = malloc(sizeof(char) * (strlen(currentProp->name) + 1));
                assert(toReturn[nameCount]);
            
                strcpy(toReturn[nameCount],currentProp->name);
                ++nameCount;
            }
        }
            
        currentProp = currentProp->next;
    }

    /* Iterate through all components at the top level */
    for (i = 0; i < comp->ncomps; ++i){
            
        currentProp = comp->comp[i]->prop;
    
		/* Iterate through all properties*/
        while (currentProp != NULL){
            
            /* If current comp is a VEVENT and current property is DTSTART, convert the time value and store it */
            if (strcmp(comp->comp[i]->name, "VEVENT") == 0 && kind == OEVENT && strcmp(currentProp->name, "DTSTART") == 0){
                
                times[timeCount].tm_sec = 0;
                times[timeCount].tm_min = 0;
                times[timeCount].tm_hour = 0;
                times[timeCount].tm_mday = 0;
                times[timeCount].tm_mon = 0;
                times[timeCount].tm_year = 0;
                times[timeCount].tm_wday = 0;
                times[timeCount].tm_yday = 0;
                times[timeCount].tm_isdst = -1;
                
                strptime(currentProp->value, "%Y%m%eT%H%M%S", &times[timeCount]);
                strftime(printDate, MAXSTRINGLENGTH, "%Y-%b-%d %l:%M %p:", &times[timeCount]);
                ++timeCount;
            }

            /* If we've run into an X-property or a summary property store it in our string array */
            else if ((kind == OPROP && currentProp->name[0] == 'X' && currentProp->name[1] == '-') || (kind == OEVENT && strcmp(currentProp->name, "SUMMARY") == 0)){
                
                if (nameCount != 0){
                    
                    toReturn = realloc(toReturn, sizeof(char *) * (nameCount + 1));
                    assert(toReturn);
                }
                
                if (kind == OPROP){
                    
                    toReturn[nameCount] = malloc(sizeof(char) * (strlen(currentProp->name) + 1));
                    assert(toReturn[nameCount]);
                
                    strcpy(toReturn[nameCount],currentProp->name);
                    ++nameCount;
                }
                
                else if (kind == OEVENT){
                    
                    toReturn[nameCount] = malloc(sizeof(char) * (strlen(currentProp->value) + 1));
                    assert(toReturn[nameCount]);
                
                    strcpy(toReturn[nameCount],currentProp->value);
                    ++nameCount;
                }
            }
                
            currentProp = currentProp->next;
        }
        
        /* If we didn't find a summary property to match one of our DTSTART properties */
        if (timeCount != nameCount){
        
			/* Enlarge our string array */
            if (nameCount != 0){
                
                toReturn = realloc(toReturn, sizeof(char *) * (nameCount + 1));
                assert(toReturn);
            }
            
            /* Allocate memory for 1 character */
            toReturn[nameCount] = malloc(sizeof(char));
            assert(toReturn[nameCount]);
            
            /* Add null terminator and increment name count */
            toReturn[nameCount][0] = '\0';
            ++nameCount;
        }
        
        /* Iterate through all subcomponents */
        for (y = 0; y < comp->comp[i]->ncomps; ++y){
            
            currentProp = comp->comp[i]->comp[y]->prop;
			
			/* Iterate through properties */
            while (currentProp != NULL){
                 
                /* If current comp is a VEVENT and current property is DTSTART, convert the time value and store it */
                if (strcmp(comp->comp[i]->comp[y]->name, "VEVENT") == 0 && kind == OEVENT && strcmp(currentProp->name, "DTSTART") == 0){
                    
                    times[timeCount].tm_sec = 0;
                    times[timeCount].tm_min = 0;
                    times[timeCount].tm_hour = 0;
                    times[timeCount].tm_mday = 0;
                    times[timeCount].tm_mon = 0;
                    times[timeCount].tm_year = 0;
                    times[timeCount].tm_wday = 0;
                    times[timeCount].tm_yday = 0;
                    times[timeCount].tm_isdst = -1;
                    
                    strptime(currentProp->value, "%Y%m%eT%H%M%S", &times[timeCount]);
                    
                    strftime(printDate, MAXSTRINGLENGTH, "%Y-%b-%d %l:%M %p:", &times[timeCount]);
                    ++timeCount;
                }
            
				/* If we've run into an X-property or a summary property store it in our string array */
                else if ((kind == OPROP && currentProp->name[0] == 'X' && currentProp->name[1] == '-') || (kind == OEVENT && strcmp(currentProp->name, "SUMMARY") == 0)){
                    
                    if (nameCount != 0){
                        
                        toReturn = realloc(toReturn, sizeof(char *) * (nameCount + 1));
                        assert(toReturn);
                    }
                    
                    if (kind == OPROP){
                        
                        toReturn[nameCount] = malloc(sizeof(char) * (strlen(currentProp->name) + 1));
                        assert(toReturn[nameCount]);
                    
                        strcpy(toReturn[nameCount],currentProp->name);
                        ++nameCount;
                    }
                    
                    else if (kind == OEVENT){
                        
                        toReturn[nameCount] = malloc(sizeof(char) * (strlen(currentProp->value) + 1));
                        assert(toReturn[nameCount]);
                    
                        strcpy(toReturn[nameCount],currentProp->value);
                        ++nameCount;
                    }
                }
    
                currentProp = currentProp->next;
            }
            
            /* If we didn't find a summary property to match one of our DTSTART properties */
            if (timeCount != nameCount){
        
				/* Enlarge our string array */
				if (nameCount != 0){
					
					toReturn = realloc(toReturn, sizeof(char *) * (nameCount + 1));
					assert(toReturn);
				}
				
				
				/* Allocate memory for 1 character */
				toReturn[nameCount] = malloc(sizeof(char));
				assert(toReturn[nameCount]);
				
				toReturn[nameCount][0] = '\0';
				++nameCount;
			}
        }
    }       
    
    
    /* If we are dealing with only VEVENTs */
    if (kind == OEVENT){
        
        sortedNames = malloc(sizeof(char *) * timeCount); // Allocate memory for array of sorted names
        
        /* Iterate through array and create string for each line that will be printed while the dates and values are still in the same array positions */
        for (i = 0; i < timeCount; ++i){
			
			strftime(printDate, MAXSTRINGLENGTH, "%Y-%b-%d %l:%M %p: ", &times[i]);
			
			sortedNames[i] = malloc(sizeof(char) * (strlen(toReturn[i]) + strlen(printDate) + 5));
			
			strcpy(sortedNames[i], printDate);
			
			if (strlen(toReturn[i]) == 0)
				strcat(sortedNames[i], "(na)");
			
			else
				strcat(sortedNames[i], toReturn[i]);
		}
		
        qsort(times, timeCount, sizeof(struct tm), compareDates); // Sort the array of dates
         
        /* Print dates from oldest to newest */
        for (i = 0; i < timeCount; ++i){
            
            strftime(printDate, MAXSTRINGLENGTH, "%Y-%b-%d %l:%M %p: ", &times[i]);
            
            for (y = 0; y < timeCount; ++y){
				
				if (strncmp(printDate, sortedNames[y], strlen(printDate)) == 0){
					
					if (fprintf(txtfile, "%s\n", sortedNames[y]) < 0){
						
						status.code = IOERR;
						status.linefrom = lineCount;
						status.lineto = lineCount;
						
						return status;
					}
					
					++lineCount;
				}
			}
        }
        
        /* Free elements of array */
        for (i = 0; i < timeCount; ++i){
		
			free(sortedNames[i]);
		}
		
		free(sortedNames); // Free the array itself
    }
    
    /* If we are dealing with X-properties */
    if (kind == OPROP){
        
        qsort(toReturn, nameCount, sizeof(char *), compareStrings); // Sort x-properties alphabetically 
        
        /* Iterate through all x-properties */
        for (i = 0; i < nameCount; ++i){
        
			/* Print current x-property */
			if (strlen(toReturn[i]) != 0){
			
				if (fprintf(txtfile, "%s\n", toReturn[i]) < 0){
					
					status.code = IOERR;
					status.linefrom = lineCount;
					status.lineto = lineCount;
					
					return status;
				}
            
				++lineCount;
			}
			
			/* Iterate to the next unique x-property */
            for (y = i + 1; y < nameCount; ++y){
                    
				if (strcmp(toReturn[i], toReturn[y]) == 0 && y == nameCount - 1){
					
					i = nameCount;
					break;
				}
				
                else if (strcmp(toReturn[i], toReturn[y]) == 0){
                    
                    continue;
                }
                
                else{
                    
                    i = y - 1;
                    break;
                }
            }
        }
    }
    
    /* Free array contents */
    for (i = 0; i < nameCount; ++i){
        
        free(toReturn[i]);
    }
         
    free(toReturn); // Free string array 
    
    status.code = OK;
    status.linefrom = lineCount;
    status.lineto = lineCount;
    
    return status;
}

CalStatus calFilter(const CalComp *comp, CalOpt content, time_t datefrom, time_t dateto, FILE *const icsfile){
    
    bool removeComp;
    CalComp * compCopy;
    CalStatus status;
    int i, y;
    CalProp * currentProp;
    struct tm * temp;
    
    compCopy = malloc(sizeof(CalComp) + (sizeof(CalComp *) * comp->ncomps));
    assert(compCopy);
    
    memcpy(compCopy, comp, sizeof(CalComp) + (sizeof(CalComp *) * comp->ncomps));

	/* Iterate through all components */
    for (i = 0; i < compCopy->ncomps; ++i){
    
		/* If filtering only VTODO, remove all other types of components */
        if (content == OTODO && strcmp(compCopy->comp[i]->name, "VTODO") != 0){
            
            for (y = i; y < compCopy->ncomps; ++y){
                
                if (y != compCopy->ncomps - 1)
					compCopy->comp[y] = compCopy->comp[y+1];
            }
            
            --i;
            --compCopy->ncomps;
            continue;
        }
        
       /* If filtering only VEVENT, remove all other types of components */
       if (content == OEVENT && strcmp(compCopy->comp[i]->name, "VEVENT") != 0){
            
            for (y = i; y < compCopy->ncomps; ++y){
                
                //printf("%d %d\n", y, y + 1);
                
                if (y != compCopy->ncomps - 1)
					compCopy->comp[y] = compCopy->comp[y+1];
            }
            
            --i;
            --compCopy->ncomps;
            continue;
        }
    }  
    
	/* If given both a start and end date */
    if (datefrom != 0 && dateto != 0){
		
		/* Iterate through all components */
		for (i = 0; i < compCopy->ncomps; ++i){
	   
			currentProp = compCopy->comp[i]->prop;
			removeComp = true;
			
			/* Iterate through all properties */
			while (currentProp != NULL){
				
				/* Check if property is one of the recognized date props */
				if (strcmp(currentProp->name, "COMPLETED") == 0 || strcmp(currentProp->name, "DTEND") == 0 || strcmp(currentProp->name, "DUE") == 0 || strcmp(currentProp->name, "DTSTART") == 0){
						
					temp = malloc(sizeof(struct tm));
					assert(temp);
					
					temp->tm_sec = 0;
					temp->tm_min = 0;
					temp->tm_hour = 0;
					temp->tm_mday = 0;
					temp->tm_mon = 0;
					temp->tm_year = 0;
					temp->tm_wday = 0;
					temp->tm_yday = 0;
					temp->tm_isdst = -1;
					
					strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
					
					/* If date property's value falls within the date range don't remove it */
					if (datefrom <= mktime(temp) && mktime(temp) <= dateto){
						
						free(temp);
						removeComp = false;
						break;
					}
					
					/* Otherwise free the temp struct tm */
					else{
						
						free(temp);
					}					
				}
				
				currentProp = currentProp->next;
			}
			
			/* Iterate through all subcomponents */
			for (y = 0; y < compCopy->comp[i]->ncomps; ++y){
				
				currentProp = compCopy->comp[i]->comp[y]->prop;
		
				/* Iterate through all properties */
				while (currentProp != NULL){
					
					/* Check if property is one of the recognized date props */
					if (strcmp(currentProp->name, "COMPLETED") == 0 || strcmp(currentProp->name, "DTEND") == 0 || strcmp(currentProp->name, "DUE") == 0 || strcmp(currentProp->name, "DTSTART") == 0){
							
						temp = malloc(sizeof(struct tm));
						assert(temp);
						
						temp->tm_sec = 0;
						temp->tm_min = 0;
						temp->tm_hour = 0;
						temp->tm_mday = 0;
						temp->tm_mon = 0;
						temp->tm_year = 0;
						temp->tm_wday = 0;
						temp->tm_yday = 0;
						temp->tm_isdst = -1;
						
						strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
						
						/* If date property's value falls within the date range don't remove it */
						if (datefrom <= mktime(temp) && mktime(temp) <= dateto){
							
							free(temp);
							removeComp = false;
							break;
						}
						
						/* Otherwise free the temp struct tm */
						else{
							
							free(temp);
						}						
					}
		
					currentProp = currentProp->next;
				}
			}
			
			/* If we didn't find a date prop with a value within the specified range remove the component */
			if (removeComp == true){
				
				for (y = i; y < compCopy->ncomps; ++y){
					
					if (y != compCopy->ncomps - 1)
						compCopy->comp[y] = compCopy->comp[y+1];
				}
				
				--i;
				--compCopy->ncomps;
				continue;
			}
		} 
	}	
	
	/* If there is only a start date */
	else if (datefrom != 0 && dateto == 0){
		
		for (i = 0; i < compCopy->ncomps; ++i){
	   
			currentProp = compCopy->comp[i]->prop;
			removeComp = true;
			
			/* Iterate through all properties */
			while (currentProp != NULL){
				
				/* Check if property is one of the recognized date props */
				if (strcmp(currentProp->name, "COMPLETED") == 0 || strcmp(currentProp->name, "DTEND") == 0 || strcmp(currentProp->name, "DUE") == 0 || strcmp(currentProp->name, "DTSTART") == 0){
						
					temp = malloc(sizeof(struct tm));
					assert(temp);
					
					temp->tm_sec = 0;
					temp->tm_min = 0;
					temp->tm_hour = 0;
					temp->tm_mday = 0;
					temp->tm_mon = 0;
					temp->tm_year = 0;
					temp->tm_wday = 0;
					temp->tm_yday = 0;
					temp->tm_isdst = -1;
					
					strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
					
					/* If date property's value falls within the date range don't remove it */
					if (datefrom <= mktime(temp)){
						
						free(temp);
						removeComp = false;
						break;
					}
					
					/* Otherwise free the temp struct tm */
					else{
						
						free(temp);
					}
				}
				
				currentProp = currentProp->next;
			}
			
			/* Iterate through all subcomponents */
			for (y = 0; y < compCopy->comp[i]->ncomps; ++y){
				
				currentProp = compCopy->comp[i]->comp[y]->prop;
		
				/* Iterate through all properties */
				while (currentProp != NULL){
					
					/* Check if property is one of the recognized date props */
					if (strcmp(currentProp->name, "COMPLETED") == 0 || strcmp(currentProp->name, "DTEND") == 0 || strcmp(currentProp->name, "DUE") == 0 || strcmp(currentProp->name, "DTSTART") == 0){
							
						temp = malloc(sizeof(struct tm));
						assert(temp);
						
						temp->tm_sec = 0;
						temp->tm_min = 0;
						temp->tm_hour = 0;
						temp->tm_mday = 0;
						temp->tm_mon = 0;
						temp->tm_year = 0;
						temp->tm_wday = 0;
						temp->tm_yday = 0;
						temp->tm_isdst = -1;
						
						strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
						
						/* If date property's value falls within the date range don't remove it */
						if (datefrom <= mktime(temp)){
						
							free(temp);
							removeComp = false;
							break;
						}
						
						/* Otherwise free the temp struct tm */
						else{
							
							free(temp);
						}						
					}
		
					currentProp = currentProp->next;
				}
			}
			
			/* If we didn't find a date prop with a value within the specified range remove the component */
			if (removeComp == true){
				
				for (y = i; y < compCopy->ncomps; ++y){
					
					if (y != compCopy->ncomps - 1)
						compCopy->comp[y] = compCopy->comp[y+1];
				}
				
				--i;
				--compCopy->ncomps;
				continue;
			}
		}
		
	}
	
	/* If there is only an end date */
	else if (datefrom == 0 && dateto != 0){
		
		/* Iterate through all components */
		for (i = 0; i < compCopy->ncomps; ++i){
	   
			currentProp = compCopy->comp[i]->prop;
			removeComp = true;
			
			/* Iterate through all properties */
			while (currentProp != NULL){
				
				/* Check if property is one of the recognized date props */
				if (strcmp(currentProp->name, "COMPLETED") == 0 || strcmp(currentProp->name, "DTEND") == 0 || strcmp(currentProp->name, "DUE") == 0 || strcmp(currentProp->name, "DTSTART") == 0){
						
					temp = malloc(sizeof(struct tm));
					assert(temp);
					
					temp->tm_sec = 0;
					temp->tm_min = 0;
					temp->tm_hour = 0;
					temp->tm_mday = 0;
					temp->tm_mon = 0;
					temp->tm_year = 0;
					temp->tm_wday = 0;
					temp->tm_yday = 0;
					temp->tm_isdst = -1;
					
					strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
					
					/* If date property's value falls within the date range don't remove it */
					if (mktime(temp) <= dateto){
						
						free(temp);
						removeComp = false;
						break;
					}
					
					/* Otherwise free the temp struct tm */
					else{
						
						free(temp);
					}
				}
				
				currentProp = currentProp->next;
			}
			
			/* Iterate through all subcomponents */
			for (y = 0; y < compCopy->comp[i]->ncomps; ++y){
				
				currentProp = compCopy->comp[i]->comp[y]->prop;
		
				/* Iterate through all properties */
				while (currentProp != NULL){
					
					/* Check if property is one of the recognized date props */
					if (strcmp(currentProp->name, "COMPLETED") == 0 || strcmp(currentProp->name, "DTEND") == 0 || strcmp(currentProp->name, "DUE") == 0 || strcmp(currentProp->name, "DTSTART") == 0){
							
						temp = malloc(sizeof(struct tm));
						assert(temp);
						
						temp->tm_sec = 0;
						temp->tm_min = 0;
						temp->tm_hour = 0;
						temp->tm_mday = 0;
						temp->tm_mon = 0;
						temp->tm_year = 0;
						temp->tm_wday = 0;
						temp->tm_yday = 0;
						temp->tm_isdst = -1;
						
						strptime(currentProp->value, "%Y%m%eT%H%M%S", temp);
						
						/* If date property's value falls within the date range don't remove it */
						if (mktime(temp) <= dateto){
							
							free(temp);
							removeComp = false;
							break;
						}
						
						/* Otherwise free the temp struct tm */
						else{
							
							free(temp);
						}						
					}
		
					currentProp = currentProp->next;
				}
			}
			
			/* If we didn't find a date prop with a value within the specified range remove the component */
			if (removeComp == true){
				
				for (y = i; y < compCopy->ncomps; ++y){
					
					if (y != compCopy->ncomps - 1)
						compCopy->comp[y] = compCopy->comp[y+1];
				}
				
				--i;
				--compCopy->ncomps;
				continue;
			}
		}
	}
	
    status = writeCalComp (icsfile, compCopy); // write to the icsfile
	
	/* Check for NOCAL caused by filtering */
	if (compCopy->ncomps == 0){
	   
		status.code = NOCAL;
		status.linefrom = 0;
		status.lineto = 0;
	}
	
	free(compCopy);

	return status;
}

CalStatus calCombine( const CalComp *comp1, const CalComp *comp2, FILE *const icsfile ){
    
    bool foundVer, foundProdid;
    CalComp * compCopy;
    CalStatus status;
    int i, y;
    CalProp * currentProp, * prodid, * version;
   
	i = 1;
	
    compCopy = malloc(sizeof(CalComp) + (sizeof(CalComp *) * comp1->ncomps)); // Allocate memory for the copy
    assert(compCopy);
    
    memcpy(compCopy, comp1, sizeof(CalComp) + (sizeof(CalComp *) * comp1->ncomps)); // Create the shallow copy
    
	addPropNode(&compCopy->prop, comp2->prop); // Add prop list from comp2 to the end of prop list in compCopy
    
    currentProp = compCopy->prop;    
	
	/* Iterate through all properties and remove second set of prodid and version. Also keep note of their addresses */
	while (currentProp != NULL){
		
		if (i >= compCopy->nprops && currentProp->next != NULL && strcmp(currentProp->next->name, "PRODID") == 0){
			
			prodid = currentProp->next;
			currentProp->next = prodid->next;
		
			/* Nested if incase VERSION is directly after PRODID */
			if (i >= compCopy->nprops && currentProp->next != NULL && strcmp(currentProp->next->name, "VERSION") == 0){
			
				version = currentProp->next;
				currentProp->next = version->next;
			}
		}
		
		 if (i >= compCopy->nprops && currentProp->next != NULL && strcmp(currentProp->next->name, "VERSION") == 0){
		
			version = currentProp->next;
			currentProp->next = version->next;
			
			/* Nested if incase PRODID is directly after VERSION */
			if (i >= compCopy->nprops && currentProp->next != NULL && strcmp(currentProp->next->name, "PRODID") == 0){
				
				prodid = currentProp->next;
				currentProp->next = prodid->next;
			}
		}
		
		currentProp = currentProp->next;
		++i;
	}
	
	compCopy->nprops += comp2->nprops - 2; // Set nprops to the correct value after removing second set of version and prodid
	
	compCopy = realloc(compCopy, sizeof(CalComp) + (sizeof(CalComp *) * (compCopy->ncomps + comp2->ncomps))); // Allocate enough memory for comp2 flexible array member
	
	y = 0;
	
	/* Add comp2 flexible array member elements to the flexible array in compCopy */
	for (i = compCopy->ncomps; i < compCopy->ncomps + comp2->ncomps; ++i){
		
		compCopy->comp[i] = comp2->comp[y];
		++y;
	}
	
	compCopy->ncomps += comp2->ncomps; // Set ncomps to the correct amount including the number of comps in comp2
	
	status = writeCalComp(icsfile, compCopy); // Write combined struct to icsfile
	
	currentProp = comp1->prop;

	/* Iterate to the end of comp1's prop list */
	for (i = 0; i < compCopy->nprops - (comp2->nprops - 1); ++i){
		
		currentProp = currentProp->next;
	}
	
	currentProp->next = NULL; // Unlink comp1 and comp2 prop lists
	
	foundProdid = false;
	foundVer = false;
	
	currentProp = comp2->prop;
	
	/* Iterate through comp2's prop list and check if VERSION or PRODID is in the prop list after unlinking */
	while (currentProp != NULL){
		
		if (strcmp(currentProp->name, "VERSION") == 0)
			foundVer = true;
			
		else if (strcmp(currentProp->name, "PRODID") == 0)
			foundProdid = true;
		
		currentProp = currentProp->next;
	}

	currentProp = comp2->prop;
	
	/* Iterate through comp2's prop list and put back VERSION and PRODID in spot they were in before removal */
	while (currentProp != NULL){
		
		/* If prodid is before version and both are at the end of the list */
		if (foundProdid == false && currentProp->next == NULL && prodid->next == version){

			currentProp->next = prodid;			
			foundProdid = true;
			break;
		}
		
		/* If version is before prodid and both are at the end of the list */
		else if (foundVer == false && currentProp->next == NULL && version->next == prodid){
			
			currentProp->next = version;
			foundVer = true;
			break;
		}
		
		/* If version is before prodid and prodid is the last in the list */
		else if (foundVer == false && currentProp->next == prodid->next && version->next == prodid){
			
			currentProp->next = version;
			foundVer = true;
			break;
		}
		
		/* If prodid is before version and version is at the end of the list */
		else if (foundProdid == false && currentProp->next == version->next && prodid->next == version){
			
			currentProp->next = prodid;
			foundVer = true;
			break;
		}
		
		/* If version and prodid are not before/after each other and we've found the spot where version was previously */
		else if (foundVer == false && currentProp->next == version->next){
			
			currentProp->next = version;
			foundVer = true;
		}
		
		/* If version and prodid are not before/after each other and we've found the spot where prodid was previously */
		else if (foundProdid == false && currentProp->next == prodid->next){
			
			currentProp->next = prodid;
			foundProdid = true;
		}
		
		currentProp = currentProp->next;
	}
	
	free(compCopy); // Free just the structure 
		
	return status;	
}

CalStatus writeCalComp (FILE *const ics, const CalComp *comp){
	
    char buffer[MAXSTRINGLENGTH], foldedBuffer[MAXSTRINGLENGTH];
	CalParam * currentParam;
    CalProp * currentProp;
    CalStatus status;
    int i, y;
    	
	/* BEGIN statement */
	strcpy(buffer, "BEGIN:");
	strcat(buffer, comp->name);
	strcat(buffer, "\r");
	strcat(buffer, "\n");
	
	/* Check if we wrote to ics succesfully */           
	if (fprintf(ics, "%s", buffer) < 0){
		
		status.code = IOERR;
		status.linefrom = lineCount;
		status.lineto = lineCount;
		
		return status;
	}
	
	currentProp = comp->prop;
	
	/* Iterate through all properties */
	while (currentProp != NULL){
			
		strcpy(buffer, currentProp->name); // Add prop name to buffer
		
		/* Add paramters to buffer if there are any */
		if (currentProp->nparams != 0){
			
			strcat(buffer, ";");
			
			currentParam = currentProp->param;
			
			/* Iterate through params */
			while (currentParam != NULL){
				
				/* Add param name and equal sign */
				strcat(buffer, currentParam->name);
				strcat(buffer, "=");
				
				/* Add all values for current param */
				for (y = 0; y < currentParam->nvalues; ++y){
					
					strcat(buffer, currentParam->value[y]);
					
					/* If not the last value add a comma */
					if (y != currentParam->nvalues - 1){
						
						strcat(buffer, ",");
					}
				}
				
				/* If not the last param add semi colon */
				if (currentParam->next != NULL)
					strcat(buffer, ";");
				
				currentParam = currentParam->next;
			}
		}
		
		/* Add colon and prop value */
		strcat(buffer, ":");
		strcat(buffer, currentProp->value);
		
		/* Check if line is too long and must be folded */
		if (strlen(buffer) > FOLD_LEN){
		
			y = 0;
			
			/* Iterate through buffer */
			for (i = 0; i < strlen(buffer); ++i){
				
				/* Add characters for first line */
				if (y <= 75){
					
					foldedBuffer[y] = buffer[i];
					++y;
				}

				/* If at max length for a line and it is the first one add EOL and NULL terminator*/
				if (i <= 74 && y == 75){
					
					foldedBuffer[y] = '\r';
					++y;
					foldedBuffer[y] = '\n';
					++y;
					foldedBuffer[y] = '\0';
					
					/* Check if we wrote to ics succesfully */ 
					if (fprintf(ics, "%s", foldedBuffer) < 0){
						
						status.code = IOERR;
						status.linefrom = lineCount;
						status.lineto = lineCount;
						
						return status;	
					}
					
					++lineCount;
					y = 0;
				}
				
				/* If at max length for a line and it is the first one or we've reached the end of the buffer, add EOL and NULL terminator */
				else if ((i > 73 && y == 74) || i == strlen(buffer) - 1){
					
					/* Check if we wrote to ics succesfully */ 
					if (fprintf(ics, " ") < 0){
						
						status.code = IOERR;
						status.linefrom = lineCount;
						status.lineto = lineCount;
						
						return status;						
					}
						
					
					foldedBuffer[y] = '\r';
					++y;
					foldedBuffer[y] = '\n';
					++y;
					foldedBuffer[y] = '\0';
					
					/* Check if we wrote to ics succesfully */ 
					if (fprintf(ics, "%s", foldedBuffer) < 0){
						
						status.code = IOERR;
						status.linefrom = lineCount;
						status.lineto = lineCount;
						
						return status;						
					}
					
					++lineCount;
					y = 0;
					
					strcpy(foldedBuffer, ""); // Clear the buffer 
				}
				
			}
			
		}
		
		/* Otherwise add EOL and print the line to ics */
		else{
			strcat(buffer, "\r");
			strcat(buffer, "\n");
			
			/* Check if we wrote to ics succesfully */ 
			if (fprintf(ics, "%s", buffer) < 0){
				
				status.code = IOERR;
				status.linefrom = lineCount;
				status.lineto = lineCount;
				
				return status;	
			}
			
			++lineCount;
		}
		
		currentProp = currentProp->next;
	}
	
	/* Recursively call writeCalComp on all components */
	for (i = 0; i < comp->ncomps; ++i){
		
        if (comp->comp[i] != NULL)
            writeCalComp(ics, comp->comp[i]);
	}
	
	/* Print END statement */
	strcpy(buffer, "END:");
	strcat(buffer, comp->name);
	strcat(buffer, "\r");
	strcat(buffer, "\n");
	
	/* Check if we wrote to ics succesfully */ 
	if (fprintf(ics, "%s", buffer) < 0){
		
		status.code = IOERR;
		status.linefrom = lineCount;
		status.lineto = lineCount;
		
		return status;		
	}
	
	++lineCount;
	
	status.linefrom = lineCount;
	status.lineto = lineCount;
	status.code = OK;
	
	return status;
}
