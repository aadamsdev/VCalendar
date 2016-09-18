#include "calutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Python.h>

/* Call readCalFile on a given file name then return a list containing the CalComp and strings for the FVP
 * 
 * Arguments: A fileName (which is a file name) and result (a PyList)
 * 
 * Preconditions: the PyList must be initialized
 * Postconditions: None
 * 
 * Return val: the PyList contains a CalComp at index 0 and strings afterwards 
 * */
static PyObject *Cal_readFile( PyObject *self, PyObject *args );

/* Call writeCalComp then return "OK" if writeCalComp executes successfully
 * 
 * Arguments: pcal (CalComp structure), complist (an integer) and toDoIndexes (if applicable)
 * 
 * Preconditions: pcal and complist must be initalized 
 * Postconditions: none
 * 
 * Return val: "OK" if successful
 * */
static PyObject *Cal_writeFile( PyObject *self, PyObject *args );

/* Call freeCalComp on a CalCOmp then return NULL
 * 
 * Arguments: pcal (CalComp structure)
 * 
 * Preconditions: pcal must be initalized 
 * Postconditions: none
 * 
 * Return val: NULL
 * */
static PyObject *Cal_freeFile( PyObject *self, PyObject *args );


static PyMethodDef CalMethods[] = {

	{"readFile", Cal_readFile, METH_VARARGS},
	{"writeFile", Cal_writeFile, METH_VARARGS},
	{"freeFile", Cal_freeFile, METH_VARARGS},
	{NULL, NULL} 
};
	
static struct PyModuleDef calModuleDef = {
	
    PyModuleDef_HEAD_INIT,
	"Cal", //enable "import Cal"
	NULL, //omit module documentation
	-1, //don't reinitialize the module
	CalMethods //link module name "Cal" to methods table
}; 

PyMODINIT_FUNC PyInit_Cal(void) { 
    
    return PyModule_Create( &calModuleDef ); 
}

static PyObject *Cal_readFile( PyObject *self, PyObject *args ){
	
    PyObject * result;
    PyObject * toAdd;
	char * fileName, compInfo[MAXSTRINGLENGTH], buffer[10];
	FILE * file;
	CalComp * comp = NULL;
    CalProp * currentProp;
    int i;
    
	PyArg_ParseTuple(args, "sO", &fileName, &result); // Parse arguments
	
    /* Open file for reading and then call readCalFile on the newly opened file*/
	file = fopen(fileName, "r");
    readCalFile(file, &comp);

    fclose(file); // Close the file
	
    result = PyList_New(comp->ncomps + 2); // Create a new PyList
    
    /* Add the CalComp to the list */
    toAdd = Py_BuildValue("k", (unsigned long*)comp);
    PyList_SetItem(result, 0, toAdd);
    
    /* Iterate through all top level components */
    for (i = 0; i < comp->ncomps; ++i){
        
        /* Build a string using the name, prop count and sub comp count of the current component */
        strcpy(compInfo, comp->comp[i]->name);
        strcat(compInfo, ",");
        snprintf(buffer, 10, "%d", comp->comp[i]->nprops);
        strcat(compInfo, buffer);
        strcat(compInfo, ",");
        snprintf(buffer, 10, "%d", comp->comp[i]->ncomps);
        strcat(compInfo, buffer);
        
        /* Iterate through all properties */
        currentProp = comp->comp[i]->prop;
        while (currentProp != NULL){
            
            /* Add the summary value to the string if it exists */
            if (strcmp(currentProp->name, "SUMMARY") == 0){
                
                strcat(compInfo, ",");
                strcat(compInfo, currentProp->value);
                break;
            }
            
            currentProp = currentProp->next;
        }
        
        /* Add the string to the list */        
        toAdd = Py_BuildValue("s", compInfo);
        PyList_SetItem(result, i + 1, toAdd);

        memset(&compInfo[0], 0, sizeof(compInfo)); // Clear the buffer 
    }
    
	return result;
}

static PyObject *Cal_writeFile( PyObject *self, PyObject *args ){
    
    
    char *filename;
    CalComp *pcal;
    FILE * fp;
    int complist, i;
    long int y;
    PyObject *toReturn;
    PyObject *toDoIndexes;
    
    PyArg_ParseTuple( args, "skiO", &filename, (unsigned long*)&pcal, &complist, &toDoIndexes); // Parse arguments
    
    fp = fopen(filename, "w"); // Open file for writing
    
    /* If we want to write the whole CalComp to the file */
    if (complist == -1){
        
        writeCalComp(fp, pcal);
    }
    
    /* If we want to remove certain VTODO components and write the rest of the CalComp to the file */
    else if (complist == -2){
        
        /* Iterate through all subcomponents */
        for (i = 0; i < pcal->ncomps; ++i){
            
            /* Iterate through all the indexes to be removed */
            for (y = 0; y < PyList_Size(toDoIndexes); ++y){
            
                /* If we find a component to be removed set it to NULL */
                if (i == (int)PyLong_AsLong(PyList_GetItem(toDoIndexes, y))){
                    
                    pcal->comp[i] = NULL;
                }
            }
        }
        
        writeCalComp(fp, pcal); // Write whats left of the CalComp to the file 
    }
    
    /* Otherwise we are just writing one component to the file (show selected */
    else{
        
        writeCalComp(fp, pcal->comp[complist]);
    }
    
    fclose(fp); // Close the file
    
    /* Return 'OK' */
    toReturn = Py_BuildValue("s", "OK"); 
    return toReturn;
}

static PyObject *Cal_freeFile( PyObject *self, PyObject *args ){
    
    PyObject * toReturn;
    CalComp * pcal;
    
    PyArg_ParseTuple(args, "k", (unsigned long*)&pcal); // Parse the argument
    
    freeCalComp(pcal); // Free the CalComp
    
    /* Return NULL */
    toReturn = Py_BuildValue("z", NULL);
    return toReturn;
}
