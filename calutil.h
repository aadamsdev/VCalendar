/********
 * Author: Andrew Adams #0828800
 * Contact: aadams03@mail.uoguelph.ca
 * 
calutil.h -- Public interface for iCalendar utility functions in calutil.c
Last updated:  8:04 PM January-16-16

RevA: Changed CalComp* argument of readCalComp().
********/

#ifndef CALUTIL_H
#define CALUTIL_H A1_RevA

#include <stdio.h>

#define FOLD_LEN 75     // fold lines longer than this length (RFC 5545 3.1)
#define VCAL_VER "2.0"  // version of standard accepted
#define MAXSTRINGLENGTH 1024
#define MAXARRAYSIZE 1000

/* data structures for ICS file in memory */

typedef struct CalParam CalParam;
typedef struct CalParam {    // property's parameter
    char *name;         // uppercase
    CalParam *next;     // linked list of parameters (ends with NULL)
    int nvalues;        // no. of values
    char *value[];      // uppercase or "..." (flexible array member)
} CalParam;

typedef struct CalProp CalProp;
typedef struct CalProp {    // (sub)component's property (=contentline)
    char *name;         // uppercase
    char *value;
    int nparams;        // no. of parameters
    CalParam *param;    // -> first parameter (or NULL)
    CalProp *next;      // linked list of properties (ends with NULL)
} CalProp;

typedef struct CalComp CalComp;
typedef struct CalComp {    // calendar's (sub)component
    char *name;         // uppercase
    int nprops;         // no. of properties
    CalProp *prop;      // -> first property (or NULL)
    int ncomps;         // no. of subcomponents
    CalComp *comp[];    // component pointers (flexible array member)
} CalComp;


/* General status return from functions */

typedef enum { OK=0,
    AFTEND,     // more text found after end of calendar 
    BADVER,     // version missing or wrong
    BEGEND,     // BEGIN...END not found as expected
    IOERR,      // I/O error
    NOCAL,      // outer block not VCALENDAR, or no V components found
    NOCRNL,     // CRNL missing at end of line
    NODATA,     // nothing between BEGIN...END
    NOPROD,     // PRODID missing
    SUBCOM,     // subcomponent not allowed
    SYNTAX,     // property not in valid form
} CalError;
    
typedef struct {
    CalError code;          // error code
    int linefrom, lineto;   // line numbers where error occurred
} CalStatus;    

/* File I/O functions */

CalStatus readCalFile( FILE *const ics, CalComp **const pcomp );
CalStatus readCalComp( FILE *const ics, CalComp **const pcomp );
CalStatus readCalLine( FILE *const ics, char **const pbuff );
CalError parseCalProp( char *const buff, CalProp *const prop );
void freeCalComp( CalComp *const comp );
CalStatus writeCalComp(FILE *const ics, const CalComp *comp);

/*	Adds a node to the end of a CalProp linked list
 * 
 * Arguments: a reference to the head of a CalProp linked list and CalProp node to add to the end of the linked list
 * 
 * Preconditions: **head and *toAdd must be initalized
 * Postconditions: *toAdd is added to the end of the linked list indicated by head. If **head is NULL, *toAdd becomes the head of the linked list
 * 
 * Return val: none
 * */
void addPropNode (CalProp **head, CalProp *toAdd);

#endif
