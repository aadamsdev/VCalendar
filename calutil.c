/********
 * Author: Andrew Adams #0828800
 * Contact: aadams03@mail.uoguelph.ca
 * 
calutil.c -- Source code for iCalendar utility functions
Last updated:  Jan 29/16
********/

#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calutil.h"

static int lineCount = 0;

int countParams (CalParam * node);

/* Free all components in CalProp object and the Calprop object itself
 * 
 * Arguments: initialized Calprop structure
 * 
 * Preconditions: *head must be initialized 
 * Postconditions: contents of *head are free'd and so is *head itself
 * 
 * Return val: none
 * */
void freePropList (CalProp *head);

/* Free all components in CalParam object and the CalParam object itself
 * 
 * Arguments: initialized CalParam structure
 * 
 * Preconditions: *head must be initialized 
 * Postconditions: contents of *head are free'd and so is *head itself
 * 
 * Return val: none
 * */
void freeParamList (CalParam * head);

/*	Parse a string of parameters of unlimited length, then return a CalParam structure
 * 
 * Arguments: initialized string of VCALENDAR parameters
 * 
 * Preconditions: *optionalParams must be initialized
 * Postconditions: a CalParam object is created, its contents are assigned
 * 
 * Return val: a CalParam object with its contents filled in
 * */
CalParam * parseParams (char * optionalParams);

/*	Checks if a the contents of a CalParam linked list are valid according to the A1 spec
 * 
 * Arguments: an initalized CalParam structure
 * 
 * Preconditions: *node must be initialized
 * Postconditions: none
 * 
 * Return val: true if all parameter names and values are valid according to the A1 spec, false otherwise
 * */
bool validateParams (CalParam * node);

/*	Creates a CalParam structure and assigns its contents based on the parameter passed as an argument
 * 
 * Arguments: a parameter string
 * 
 * Preconditions: *parameter must be initialized
 * Postconditions: memory is allocated for a CalParam structure and its contents
 * 
 * Return val: a newly allocated CalParam structure with its contents filled in
 * */
CalParam * createNode (char *parameter);

/*	Adds a node to the end of a CalParam linked list
 * 
 * Arguments: a reference to the head of a CalParam linked list and CalParam node to add to the end of the linked list
 * 
 * Preconditions: **head and *toAdd must be initalized
 * Postconditions: *toAdd is added to the end of the linked list indicated by head. If **head is NULL, *toAdd becomes the head of the linked list
 * 
 * Return val: none
 * */
void addParamNode (CalParam **head, CalParam *toAdd);

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

/*	Checks for the NOCAL error in a CalComp structure
 * 
 * Arguments: a CalComp structure
 * 
 * Preconditions: *toCheck is initialized
 * Postconditions: none
 * 
 * Return val: false if atleast one of CalComp structure's name doesn't start with a 'V', true otherwise
 * */
bool checkNoCal (CalComp * toCheck);

/*	Checks for the BADVER error in a CalProp structure
 * 
 * Arguments: a CalProp structure
 * 
 * Preconditions: *head is initialized
 * Postconditions: none
 * 
 * Return val: false if VERSION is missing, occurs more than once, or if its value does not match the macro VCAL_VER. True otherwise
 * */
bool checkBadVer (CalProp * head);

/*	Checks for the NOPROD error in a CalProp structure
 * 
 * Arguments: a CalProp structure
 * 
 * Preconditions: *toCheck is initialized
 * Postconditions: none
 * 
 * Return val: false if a CalProp structure's name doesn't start with a 'V', true otherwise
 * */
bool checkProd (CalProp * head);

void freeCalComp (CalComp *const comp){
	
	int i;
	
	/* Check if comp->name is already NULL, if not free it then set to NULL */
	if (comp->name != NULL){
		free(comp->name);
		comp->name = NULL;
	}
	
	/* Check if comp->prop is already NULL, if not free it */
	if (comp->prop != NULL)
		freePropList(comp->prop);
	
	/* Free all flexible array elements by recursively calling this function */
	for (i = 0; i < comp->ncomps; ++i){
		
		freeCalComp(comp->comp[i]);
	}
	
	/* If comp isn't already NULL free it */
	if (comp != NULL){
		free(comp);
	}
}

void freePropList (CalProp *head){
	
	CalProp * temp;
	
	/* Iterate through CalProp linked list and free each structure until we reach the end of the list */
	while (head != NULL){
		
		temp = head; 
		head = head->next; // Set head to the next element 
       
       /* If temp->name isn't NULL free it and set to NULL */
		if (temp->name != NULL){
			
			free(temp->name);
			temp->name = NULL;
		}
		
		/* If temp->value isn't NULL free it and set to NULL */
		if (temp->value != NULL){
			
			free(temp->value);
			temp->value = NULL;
		}
		
		/* If temp->param isn't NULL free the CalParam list and set to NULL */
		if (temp->param != NULL){
			
			freeParamList(temp->param);
			temp->param = NULL;
		}
		
		/* If temp isn't NULL free it and set to NULL */
		if (temp != NULL){
			
			free(temp);
			temp = NULL;
		}
	}
}

void freeParamList (CalParam * head){
	
	CalParam * temp;
	int i;
	
	/* Iterate through CalParam linked list and free each structure until we reach the end of the list */
	while (head != NULL){
		
		temp = head;
		head = head->next; // Set head to the next element 
       
       /* If temp->name isn't NULL free it and set to NULL */
       if (temp->name != NULL){
		   
			free(temp->name);
			temp->name = NULL;
		}
		
		/* Iterate through param values, if value isn't NULL free it and set to NULL*/
		for (i = 0; i < temp->nvalues; ++i){
			
			if (temp->value[i] != NULL){
				free(temp->value[i]);	
				temp->value[i] = NULL;
			}
		}
		
		/* If temp isn't NULL free it and set and to NULL */
		if (temp != NULL){
			free(temp);
			temp = NULL;
		}
	}
	
}

bool checkNoCal (CalComp * toCheck){
			
	int i, y;
	
	/* Iterate through all components and see if the name of atleast one starts with 'V' */
	for (i = 0; i < toCheck->ncomps; ++i){
		
		if (toCheck->comp[i]->name[0] == 'V')
			return true;
			
		for (y = 0; y < toCheck->comp[i]->ncomps; ++y){
			
			if (toCheck->comp[i]->comp[y]->name[0] == 'V')
				return true;
		}
	}

	/* If CalComp has no components return false */
    if (toCheck->ncomps == 0){
        
        return false;
    }
		
	return false; // If no components start with a 'V' return false 
}

bool checkBadVer (CalProp * head){
	
	bool foundCorrectVer;
	
	foundCorrectVer = false; // Set boolean value to false 
	
	/* Iterate through all CalProp structures and check if we find VERSION twice or if its value doesn't match VCAL_VER */
	while (head != NULL){
		
		/* If we've found VERSION and it has the correct value */
		if (strcmp(head->name, "VERSION") == 0 && strcmp(head->value, VCAL_VER) == 0){
			
			/* If we've already ran into VERSION return false */
			if (foundCorrectVer == true){
			
				return false;
			}
			
			/* Otherwise return true */
			else{
			
				foundCorrectVer = true;
			}
		}
		
		head = head->next; // Iterate to the next structure in the linked list
	}
	
	/* If we only found one VERSION and it has the correct value return true */
	if (foundCorrectVer == true){
	
		return true;
	}
	
	/* If we find VERSION at all return false */
	else{
		
		return false;
	}
	
}

bool checkProd (CalProp * head){
	
	bool foundProd;
	
	foundProd = false;
	
	/* Iterate through all CalProp objects looking for PRODID */
	while (head != NULL){
		
		/* If we found PRODID */
		if (strcmp(head->name, "PRODID") == 0){
			
			/* If we've found PRODID already return false */
			if (foundProd == true){
			
				return false;
			}
			
			/* Otherwise set boolean value to true */
			else{
			
				foundProd = true;
			}
		}
		
		head = head->next; // Iterate to the next structure in the linked list 
	}
	
	/* If we only found one PRODID return true */
	if (foundProd == true){
	
		return true;
	}
	
	/* Otherwise return false */
	else{
		
		return false;
	}
	
}
 
CalStatus readCalFile( FILE *const ics, CalComp **const pcomp ){

	CalStatus status;
	char * buffer;
	
	buffer = NULL;
	
	readCalLine(NULL, NULL); // Initial call to readCalLine to reset everything
	
	/* Allocate memory for *pcomp and initialize all its contents */
	*pcomp = malloc(sizeof(CalComp) + (sizeof(CalComp *)));
	assert(*pcomp);
	
	(*pcomp)->name = NULL;
	(*pcomp)->nprops = 0;
	(*pcomp)->prop = NULL;
	(*pcomp)->ncomps = 0;
	
	status = readCalComp(ics, pcomp);
	
	/* Check if readCalComp returned an error, free *pcomp if so and return the suberror */
	if (status.code != OK){
		
		freeCalComp(*pcomp);
		return status;
	}
	
	/* Check for NOCAL error, free *pcomp if so and return the suberror */
	else if (checkNoCal(*pcomp) == false){
		
		status.code = NOCAL;
		status.linefrom = lineCount;
		status.lineto = lineCount;
		
		freeCalComp(*pcomp);
		return status;
	}
	
	/* Check for BADVER error, free *pcomp if so and return the suberror */
	else if (checkBadVer((*pcomp)->prop) == false){
		
		status.code = BADVER;
		status.linefrom = lineCount;
		status.lineto = lineCount;
		
		freeCalComp(*pcomp);
		return status;
	}
	
	/* Check for NOPROD error, free *pcomp if so and return the suberror */
	else if (checkProd((*pcomp)->prop) == false){
		
		status.code = NOPROD;
		status.linefrom = lineCount;
		status.lineto = lineCount;
		
		freeCalComp(*pcomp);
		return status;
	}
	
	readCalLine(ics, &buffer); // Call readCalLine to check for AFTEND error 
	
	/* If we receive something other then NULL from readCalLine the file hasn't ended so return AFTEND and free *pcomp */
	if (buffer != NULL){
		free(buffer);
		
		status.code = AFTEND;
		status.linefrom = lineCount;
		status.lineto = lineCount;
		
		freeCalComp(*pcomp);
		return status;
	}
	
	//printCalComp(*pcomp);
	//freeCalComp(*pcomp);
	
	/* Set error code to OK and return */
    //--lineCount;
	status.code = OK;
	status.linefrom = lineCount;
	status.lineto = lineCount;
	return status;
}

CalStatus readCalComp( FILE *const ics, CalComp **const pcomp ){
	
	static int depth = 0;
	bool foundLastEnd = false;
	CalProp *toAdd;
	CalError returnVal;
	CalStatus status, returnValStatus;
	char ** pbuff, * buffer;
	int i;
	
	buffer = NULL;
	pbuff = &buffer;
	
	toAdd = NULL;
	
	/* Read lines from file until EOF or we run into END:VCALENDAR */
	do{

		status = readCalLine(ics, pbuff); // Read a line from the input file 
		
		if (buffer != NULL){
		
			/* Allocate memory for a temporary CalProp structure and parse the string from readCalLine */
			toAdd = malloc(sizeof(CalProp));
			assert(toAdd);
			
			returnVal = parseCalProp(buffer, toAdd);
			
			/* If parseCalProp returns an error, free temp CalProp and return suberror */
			if (returnVal != OK){
				
				free(buffer);
				freePropList(toAdd);
				
				status.code = returnVal;
				status.linefrom = lineCount;
				status.lineto = lineCount;
				
				return status;
			}
			
			/* If we've found the first BEGIN */
			if ((*pcomp)->name == NULL && toAdd->name != NULL && strcmp(toAdd->name, "BEGIN") == 0){
				
				/* Check if value is "VCALENDAR" */
				if (strcmp(toAdd->value, "VCALENDAR") == 0){
				
					(*pcomp)->name = malloc(sizeof(char) * (strlen(toAdd->value) + 1)); // Allocate memory for the name
					assert((*pcomp)->name);
					
					/* Copy value from toAdd to name character by character */
					for (i = 0; i < strlen(toAdd->value); ++i)
						(*pcomp)->name[i] = toupper(toAdd->value[i]);
						
					(*pcomp)->name[i] = '\0'; // Add null terminator
					
					depth = 1; // Set depth to 1
					
					/* Free temp CalProp */
					free(toAdd->name);
					free(toAdd->value);
					free(toAdd);
				}
				
				/* If first BEGIN didn't have "VCALENDAR" as value */
				else{
					
					/* Free temp CalProp and buffer from readCalLine */
					free(toAdd->name);
					free(toAdd->value);
					free(toAdd);
					free(buffer);
					
					/* Return NOCAL error */
					status.code = NOCAL;
					status.linefrom = lineCount;
					status.lineto = lineCount;
					return status;
				}
			}
			
			/* If we've run into a nested BEGIN */
			else if (toAdd != NULL && toAdd->name != NULL && strcmp(toAdd->name, "BEGIN") == 0){
				
				/* Check for SUBCOM error */
				if (depth == 3){
					
					/* Free temp CalProp and other allocated memory */
					free(toAdd->name);
					free(toAdd->value);
					free(toAdd);
					free(buffer);
					
					/* Return SUBCOM error */
					status.code = SUBCOM;
					status.linefrom = lineCount;
					status.lineto = lineCount;
					return status;
				}
				
				/* Allocate memory for the nested BEGIN */
				(*pcomp) = realloc((*pcomp), sizeof(CalComp) + (sizeof(CalComp *) * ((*pcomp)->ncomps + 1)));
				assert((*pcomp));
				
				(*pcomp)->comp[(*pcomp)->ncomps] = malloc(sizeof(CalComp) + sizeof(CalComp *));
				assert((*pcomp)->comp[(*pcomp)->ncomps]);
				
				(*pcomp)->comp[(*pcomp)->ncomps]->nprops = 0;
				(*pcomp)->comp[(*pcomp)->ncomps]->prop = NULL;
				(*pcomp)->comp[(*pcomp)->ncomps]->ncomps = 0;
				
				(*pcomp)->comp[(*pcomp)->ncomps]->name = malloc(sizeof(char) * (strlen(toAdd->value) + 1));
				assert((*pcomp)->comp[(*pcomp)->ncomps]->name);
				
				/* Copy value from temp CalProp to name character by character */
				for (i = 0; i < strlen(toAdd->value); ++i)
					(*pcomp)->comp[(*pcomp)->ncomps]->name[i] = toAdd->value[i];
					
				(*pcomp)->comp[(*pcomp)->ncomps]->name[i] = '\0'; // Add null terminator
				
				/* Free temp CalProp */
				free(toAdd->name);
				free(toAdd->value);
				free(toAdd);
				
				++depth; // Increment depth
				
				/* Recursively call readCalComp and increment ncomps in current CalComp structure */
				returnValStatus = readCalComp(ics, &(*pcomp)->comp[(*pcomp)->ncomps]); 
				++(*pcomp)->ncomps;
				
				/* If readCalComp return an error, return the suberror and free buffer from readCalLine */
				if (returnValStatus.code != OK){

					free(buffer);
				
					return returnValStatus;
				}
			}
			
			/* If we've run into an END */
			else if (toAdd != NULL && toAdd->name != NULL && strcmp(toAdd->name, "END") == 0){
				
                for (i = 0; i < strlen(toAdd->value); ++i)
					toAdd->value[i] = toupper(toAdd->value[i]);
                    
				/* Check if value for END matches the current name */
				if (strcmp((*pcomp)->name, toAdd->value) == 0){
					
					/* Check for NODATA error */
					if ((*pcomp)->ncomps == 0 && (*pcomp)->nprops == 0){
						
						/* Free temp CalProp and buffer from readCalLine */
						free(toAdd->value);
						free(toAdd->name);
						free(toAdd);
						free(buffer);
						--depth;

						/* Return NODATA error */
						status.code = NODATA;
						status.linefrom = lineCount;
						status.lineto = lineCount;			
						return status;
					}
					
					else{
						
						/* Check if this is the last END */
						if (strcmp((*pcomp)->name, "VCALENDAR") == 0){
							
							foundLastEnd = true;
						}
						
						/* Free temp CalProp */
						free(toAdd->value);
						free(toAdd->name);
						free(toAdd);
						free(buffer);

						/* Reduce depth and return OK */
						--depth;
						status.code = OK;
						status.linefrom = lineCount;
						status.lineto = lineCount;
						return status;
					}
				}
				
				/* If END has wrong value */
				else if (strcmp((*pcomp)->name, toAdd->value) != 0){
					
					
					/* Free temp CalProp and buffer */
					free(toAdd->value);
					free(toAdd->name);
					free(toAdd);
					free(buffer);
					
					
					/* Reduce depth and return BEGEND error */
					--depth;
					status.code = BEGEND;
					status.linefrom = lineCount;
					status.lineto = lineCount;
					return status;
				}
			}
			
			/* Otherwise increment nprops and add toAdd to the list of CalProps */
			else if (toAdd  != NULL){
				
				(*pcomp)->nprops++;
				addPropNode(&(*pcomp)->prop, toAdd);
			}
			
			free(buffer); // Free readCalLine buffer
		}
		
        if ((*pcomp)->name == NULL){
        
            //printf("!!!!");
            status.code = NOCAL;
            status.linefrom = lineCount;
            status.lineto = lineCount;
            
            return status;
            
        }
        
		/* If we've reached EOF and didn't get the last END statement */
		else if (buffer == NULL && foundLastEnd == false){
			
            //printf("BEGEND!");
			/* Return BEGEND error */
            //lineCount;
			status.code = BEGEND;
			status.linefrom = lineCount;
			status.lineto = lineCount;
			return status;
		}
		
	}while (*pbuff != NULL);


	free(buffer); // Free readCalLine buffer 
	
	return status;
}

CalStatus readCalLine( FILE *const ics, char **const pbuff ){
	
	char currentChar, *buildBuffer;
	CalStatus status;
	int charCount, loopCount, foldedCount;
	bool carriageReturn, onlyEOF;
	
	carriageReturn = false;
	charCount = 0;
	foldedCount = 0;
    onlyEOF = true;
    
	/* Reset everything if no input file is given */
	if (ics == NULL){
		
        //printf("NULL\n");
		lineCount = 0;
		status.linefrom = 0;
		status.lineto = 0;
		return status; 
	}
	
	else{
		
        //lineStart = lineCount;
		buildBuffer = malloc(sizeof(char) * MAXSTRINGLENGTH); // Allocate memory for current line 
		assert(buildBuffer);
		
		/* Get characters from file until EOF */
		while((currentChar = fgetc(ics)) != EOF){ 				
            
            onlyEOF = false;
            
			/* If we've run into a carraige return */
			if (currentChar == '\r'){
				//status.linefrom = lineStart;
					
				carriageReturn = true;
				continue;
			}
			
			/* If we've run into an EOL and the last character was a carriage return */
			else if (currentChar == '\n' && carriageReturn == true){
				
                ++lineCount;
                
				/* Check for folding */
				if ((currentChar = fgetc(ics)) != EOF){
						
					/* Continue if this line is folded */
					if (currentChar == '\t' || currentChar == ' '){
                        ++foldedCount;
						carriageReturn = false;
						continue;
					}
					
					/* If EOF return OK and set *pbuff to NULL */
					else if (currentChar == EOF){
						
						*pbuff = NULL;
						free(buildBuffer);

						status.code = OK;
						status.linefrom = lineCount - foldedCount;
						status.lineto = lineCount;
						return status;
					}					
					
					/* If line isn't folded put the character we grabbed to check for folding back in its place */
					else{
						
						ungetc(currentChar, ics);
					}
				}
				
                
				buildBuffer[charCount] = '\0'; // Add null terminator 
				
				/* Check if line is only whitespace; return OK and set *pbuff to the current line if we run into atleast one non-whitespace char */
				for (loopCount = 0; loopCount < strlen(buildBuffer); ++loopCount){
					
					if (isspace(buildBuffer[loopCount]) == 0){

						*pbuff = buildBuffer;
						
                        //printf("%s %d\n", *pbuff, lineCount);
						status.code = OK;
						status.linefrom = lineCount - foldedCount;
						status.lineto = lineCount;
						
						return status;
					}
				}
				
                carriageReturn = false;
				continue;
			}
			
			/* If we run into an EOL and didn't previously receive a carriage return */
			else if (currentChar == '\n' && carriageReturn == false){
				
				/* Set *pbuff to null and free buffer */
				*pbuff = NULL;
				free(buildBuffer);

				/* Return NOCRNL error */
				status.code = NOCRNL;
				status.linefrom = lineCount - foldedCount;
				status.lineto = lineCount;
				return status;
			}
            
            else if (currentChar != '\n' && carriageReturn == true){
                
                //printf("HERE!\n");
				*pbuff = NULL;
				free(buildBuffer);

				
				status.code = NOCRNL;
				status.linefrom = lineCount - foldedCount;
				status.lineto = lineCount;
				return status;
                
            }

			/* Otherwise just add current character to buffer */
			else{
				
				buildBuffer[charCount] = currentChar;
				++charCount;	
			}
		}
        
        /*if (onlyEOF == true)
            buildBuffer[charCount] = '\0';*/
            
        if (currentChar == EOF && buildBuffer != NULL ){
                
            ++lineCount;
            buildBuffer[charCount] = '\0'; // Add null terminator 
            
            if (strlen(buildBuffer) != 0){
            //printf("%d \n", strlen(buildBuffer));

                    
                /* Check if line is only whitespace; return OK and set *pbuff to the current line if we run into atleast one non-whitespace char */
                for (loopCount = 0; loopCount < strlen(buildBuffer); ++loopCount){
                    
                    if (isspace(buildBuffer[loopCount]) == 0){
                
                        *pbuff = buildBuffer;
                        //printf("%s - %d\n", *pbuff, lineCount);
                        status.code = OK;
                        status.linefrom = lineCount - foldedCount;
                        status.lineto = lineCount;
                        
                        return status;
                    }
                }
            }
        }
		
		/* Free buffer and set *pbuff to NULL */
		free(buildBuffer);
		*pbuff = NULL;
        
        if (onlyEOF == true && lineCount == 1){
            //printf("true!\n");
            onlyEOF = false;
            --lineCount;
        }
        
        else if (onlyEOF == true && lineCount != 1){
            //printf("!!!");
            --lineCount;
            onlyEOF = false;
        }
        status.code = OK;
        status.linefrom = lineCount - foldedCount;
        status.lineto = lineCount;
        //printf("%d\n", lineCount - foldedCount);
	}

	return status;
}

CalError parseCalProp( char *const buff, CalProp *const prop ){
	
	bool onlyPropVal, parsedParams, quoteStart;
	CalError status;
    
	int i, j, count;
	char * currentString;
	
	/* Set all contents to NULL or zero */
	prop->name = NULL;
	prop->value = NULL;
	prop->nparams = 0;
	prop->param = NULL;
	prop->next = NULL;

	currentString = malloc(sizeof(char) * strlen(buff)); 
	assert(currentString);
	
	/* Set count to 0 and boolean to false */
	count = 0;
	onlyPropVal = false;
	parsedParams = false;
	quoteStart = false;
	
	/* Iterate through buffer */
	for (i = 0; i < strlen(buff); ++i){
		
		/* If we've run into a quote and its the opening one */
		if (buff[i] == '"' && quoteStart == false){
			
			quoteStart = true;
		}
		
		/* If we've run into a quote and its the closing one */
		else if (buff[i] == '"' && quoteStart == true){
			
			quoteStart = false;
		}
		
		/* Parse name */
		if ((buff[i] == ';' || buff[i] == ':') && prop->name == NULL){
			
			/* In the case of no params */
			if (buff[i] == ':')
				onlyPropVal = true;
				
			currentString[count] = '\0';
			
			prop->name = currentString;
			
			/* Convert to uppercase */
			for (j = 0; j < strlen(prop->name); ++j){
				
				prop->name[j] = toupper(prop->name[j]);
			}
			
			count = 0;
			currentString = malloc(sizeof(char) * strlen(buff));
			assert(currentString);
			
			//printf("Prop name: %s\n\n", prop->name);
			
			continue;
		}
		
		/* Parse optional parameters */
		else if (onlyPropVal == false && prop->name != NULL && (buff[i] == ':' || buff[i] == ';') && quoteStart == false && parsedParams == false){
			
            if (buff[i] == ':')
                parsedParams = true;

			currentString[count] = '\0';
            
            addParamNode(&prop->param, parseParams(currentString));
			prop->nparams = countParams(prop->param);
            
			free(currentString);
			
			count = 0;
			currentString = malloc(sizeof(char) * strlen(buff));
			assert(currentString);
			
			continue;
		}
		
		/* Parse value */
		else if ((onlyPropVal == true || parsedParams == true) && i == (strlen(buff) - 1)){
			
			currentString[count] = buff[i];
			++count;
			
			currentString[count] = '\0';
			prop->value = currentString;
			
			count = 0;
			continue;
		}
		
		currentString[count] = buff[i];
		++count;
	}
	
	/* If we didn't find a value, set it to zero length */
	if (prop->value == NULL){
		
		prop->value = malloc(sizeof(char));
		assert(prop->value);
		
		prop->value[0] = '\0';
		free(currentString);
	}
	
	/* Check if name was found and isn't zero length */
	if (prop->name != NULL && strlen(prop->name) != 0){
		
		/* Check if params are valid, return OK if so */
		if (prop->param != NULL && validateParams(prop->param) == true){
			
			status = OK;
			return status;
		}
		
		/* Check if params are invalid, free prop and its contents if so then return SYNTAX */
		else if (prop->param != NULL && validateParams(prop->param) == false){
				
			if (prop->name != NULL){
				free(prop->name);
				prop->name = NULL;
			}
			
			if (prop->value != NULL){
				free(prop->value);
				prop->value = NULL;
			}
			
			if (prop->param != NULL){
				freeParamList(prop->param);
				prop->param = NULL;
			}
				
			status = SYNTAX;
			return status;
		}
		
        /* Check if params are invalid, free prop and its contents if so then return SYNTAX */
		else if (parsedParams == true && prop->nparams == 0){
				
			if (prop->name != NULL){
				free(prop->name);
				prop->name = NULL;
			}
			
			if (prop->value != NULL){
				free(prop->value);
				prop->value = NULL;
			}
			
			if (prop->param != NULL){
				freeParamList(prop->param);
				prop->param = NULL;
			}
				
			status = SYNTAX;
			return status;
		}
        
		/* Otherwise we can return OK */
		status = OK;
		return status;
	}
	
	/* If anything else goes wrong, return SYNTAX and free prop and its contents */
	else{

		if (prop->name != NULL){
			free(prop->name);
			prop->name = NULL;
		}
		
		if (prop->value != NULL){
			free(prop->value);
			prop->value = NULL;
		}
		
		if (prop->param != NULL){
			freeParamList(prop->param);
			prop->param = NULL;
		}
		
		status = SYNTAX;
		return status;
	}
	
	return status;
}

CalParam * parseParams (char * optionalParams){ 
	
	bool quoteStart, multipleParams;
	CalParam * toReturn;
	char * paramName;
	int i, charCount;
		
	/* Set charCount to zero and all booleans to false */
	multipleParams = false;
	quoteStart = false;	
	charCount = 0;
	
	toReturn = NULL;
	
	paramName = malloc(sizeof(char) * strlen(optionalParams));
	assert(paramName);
	
	/* Iterate through each char */
	for (i = 0; i < strlen(optionalParams); ++i){
		
		//prop->param = malloc(sizeof(CalParam));
		
		/* If we run into a quote and its the opening one */
		if (optionalParams[i] == '"'){
			
			quoteStart = true;
		}
		
		/* If we run into a quote and its the closing one */
		else if (optionalParams[i] == '"' && quoteStart == true){
			
			quoteStart = false;
		}
		
		/* Found param */
		if (optionalParams[i] == ';' && quoteStart == false){
			
			multipleParams = true;
			paramName[charCount] = '\0';
			
			/* If its the first param, create it with createNode */
			if (toReturn == NULL){
				
				toReturn = createNode(paramName);
			}
			
			/* Otherwise just add it to the linked list of params */
			else if (toReturn != NULL){
				
				addParamNode(&toReturn, createNode(paramName));
			}
			
			/* Free temp string and allocate memory for next temp string */
			free(paramName);
			paramName = malloc(sizeof(char) * strlen(optionalParams));
			assert(paramName);
			
			charCount = 0;
			continue;
		}
		
		/* Last param with multiple */
		else if (multipleParams == true && i == (strlen(optionalParams) - 1)){
			
			paramName[charCount] = optionalParams[i];
			++charCount;
			
			paramName[charCount] = '\0';
			
			/* If its the first param, create it with createNode */
			if (toReturn == NULL){
				
				toReturn = createNode(paramName);
			}
			
			/* Otherwise just add it to the linked list of params */
			else if (toReturn != NULL){
				
				addParamNode(&toReturn, createNode(paramName));
			}
			
			free(paramName); // Free temp string 

			break;
		}
		
		/* Last and only param */
		else if (multipleParams == false && i == (strlen(optionalParams) - 1)){
			
            free(paramName);
			paramName = optionalParams;
			
			/* If its the first param, create it with createNode */
			if (toReturn == NULL){
				
				toReturn = createNode(paramName);
			}
			
			/* Otherwise just add it to the linked list of params */
			else if (toReturn != NULL){
				
				addParamNode(&toReturn, createNode(paramName));
			}
			
			
		}
		
		/* Otherwise add character to temp string */
		paramName[charCount] = optionalParams[i];
		++charCount;
	}
	
	return toReturn;
}

int countParams (CalParam * node){
    
    int count;
    
    count = 0;
    
    /* Iterate through linked list */
	while (node != NULL){
		
        ++count;
		
        //printf("%s\n", node->name);
		node = node->next;
	}
    
    return count;
}

bool validateParams (CalParam * node){
	
    int i;
    
	/* Iterate through linked list */
	while (node != NULL){
		
        
		/* If name is NULL or zero length return false */
		if (node->name == NULL || strlen(node->name) == 0){
			
			return false;
		}
        
        for (i = 0; i < strlen(node->name); ++i){
            
            if (node->name[i] == ' ')
                return false;
        }
		
		node = node->next;
	}
	
	return true; // Return true if we've made it to the end of the linked list 
}

CalParam * createNode (char * parameter){
	
	int i, j, charCount, nameSize, valueCount;
	bool quoteStart;
	CalParam * toReturn;
	
	/* Arrays to keep track of where param values start and the length of them */
	int valueSize[MAXARRAYSIZE] = {0};
	int valueStart[MAXARRAYSIZE] = {0};
	
	quoteStart = false;
	
	/* Set all counts to zero */
	charCount = 0;
	nameSize = 0;
	valueCount = 0;
	
	toReturn = NULL;
	
	/* Iterate through parameter string */
	for (i = 0; i < strlen(parameter); ++i){
		
        
		/* If we've run into a quote and it is the opening one */
		if (parameter[i] == '"' && quoteStart == false){
			
			quoteStart = true;
		}
		
		/* If we've run into a quote and it is the closing one */
		else if (parameter[i] == '"' && quoteStart == true){
			
			quoteStart = false;
		}
        

		
		/* Ignore equals sign if they are inside quotes */
		if (parameter[i] == '=' && quoteStart == false){
			
			nameSize = charCount;
			charCount = 0;
			
            /* If we've run into the paramter value */
            if (i == (strlen(parameter) - 1)){
                
                //printf("!!!!!");
                valueSize[valueCount] = charCount;
                valueStart[valueCount] = i - (valueSize[valueCount] - 1);
                ++valueCount;
                charCount = 0;
            }
            
			continue;
		}
        
		/* If we've run into the paramter value */
		if (i == (strlen(parameter) - 1)){
			
			++charCount;
			valueSize[valueCount] = charCount;
			valueStart[valueCount] = i - (valueSize[valueCount] - 1);
			++valueCount;
			charCount = 0;
		}
		
		/* If we've run into a comma and it isn't inside quotations */
		else if (parameter[i] == ',' && quoteStart == false){
			
			valueSize[valueCount] = charCount;
			valueStart[valueCount] = i - (valueSize[valueCount]);
			charCount = 0;
	
			++valueCount;
			continue;
		}
		
		++charCount;
	}
	
	/* Allocate memory for paramter to be returned */
	toReturn = malloc(sizeof(CalParam) + (valueCount * sizeof(char *)));
	assert(toReturn);
	toReturn->name = malloc(sizeof(char) * (nameSize + 1));
	assert(toReturn->name);
	toReturn->next = NULL;
	toReturn->nvalues  = valueCount;
	
	/* Set the name for the parameter converting it to uppercase as we go */
	for (i = 0; i < nameSize; ++i){
		
		toReturn->name[i] = toupper(parameter[i]);
	}
	
	toReturn->name[i] = '\0'; // Add null terminator to name 
    
	/* Set values for paramter using the arrays initialized at the beginning of this function */
	for (i = 0; i < valueCount; ++i){
	
		toReturn->value[i] = malloc(sizeof(char) * (valueSize[i] + 1));
		assert(toReturn->value[i]);
		charCount = 0;
		
        if (valueSize[i] == 0){
            
            toReturn->value[i][charCount] = '\0';
        }
		
        else{
            
            /* Copy characters for current value */
            for (j = valueStart[i]; j < (valueStart[i] + valueSize[i]); ++j){
                
                toReturn->value[i][charCount] = parameter[j];
                ++charCount;
            }
            
            toReturn->value[i][charCount] = '\0'; // Add null terminator to value
            
            /* Convert value to uppercase if value isn't enclosed in quotes */
            for (j = 0; j < strlen(toReturn->value[i]); ++j){
                
                if (toReturn->value[i][j] == '"' && j == 0){
                
                    break;
                }
                
                else{
                    
                    toReturn->value[i][j] = toReturn->value[i][j];
                }
            }
        }
	}
	
	return toReturn;
}

void addParamNode (CalParam **head, CalParam *toAdd){
 
    CalParam *last = *head; 
 
	/* If head is empty, set head to the param to be added */
    if (*head == NULL){
       
       *head = toAdd;
       return;
    }  

	/* Otherwise iterate to the end of the linked list */
    while (last->next != NULL)
        last = last->next;
  
    last->next = toAdd; // Add *toAdd to the end of the list 
    return;    
}

void addPropNode (CalProp **head, CalProp *toAdd){
	
    CalProp *last = *head; 
    
    /* If head is empty, set head to the property to be added */
    if (*head == NULL){
       
       *head = toAdd;
       return;
    }  
      
    /* Otherwise iterate to the end of the linked list */
    while (last->next != NULL)
        last = last->next;

    last->next = toAdd; // Add *toAdd to the end of the list 
    return;    
}
