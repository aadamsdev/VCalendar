 # Author: Andrew Adams #0828800
 # Contact: aadams03@mail.uoguelph.ca
 # 
 # xcal.py -- Source code for xcal GUI
 # Last updated:  Feb 22/2016

#!/usr/bin/python

from Cal import *
import tkinter as tk
from tkinter import *
import os
from tkinter import filedialog
import subprocess 

class xCalGUI:
    
    # global variables
    dateMask = ""
    currentFile = ""
    result = []
    printCount = 1
    
    # constructor
    def __init__(self,root):
        
        frame = Frame(root)
        frame.pack(fill = 'both', expand = 1)
        self.initialize(frame, root) #initialize main window
        self.initialDateMask(frame) # Ask user if they want to set date mask 
        
        return
        
    # date mask window
    def initialDateMask(self, frame):
        
        # file selection for date mask
        def createDateMaskWin (self, initialWindow):
        
            types = [('All files', '*')]
            dialog = filedialog.askopenfilename(filetypes = types)
            
            if len(dialog) != 0:
                dateMask = dialog
                os.environ['DATEMSK'] = dialog
                initialWindow.destroy()
                
            return
        
        # create new window and disable resize by setting min and max window size
        intialWin = Toplevel(frame)
        intialWin.minsize(height = 50, width = 400)
        intialWin.maxsize(height = 50, width = 400)
        intialWin.title("Date Mask")
    
        # Create label and pack
        labelFrame = Frame(intialWin)
        warningLabel = Label (labelFrame, text = "Would you like to set the date mask system variable now?")
        warningLabel.pack()
        labelFrame.pack(side = TOP, fill = 'both', expand = 'yes')
        
        # Create buttons and pack
        buttonFrame = Frame(intialWin)
        continueButton = Button(buttonFrame, text = "Yes", command = lambda: createDateMaskWin(self, intialWin))
        cancelButton = Button(buttonFrame, text = "Cancel", command = lambda: intialWin.destroy())
        continueButton.pack(side = LEFT, fill = 'both', expand = 'yes')
        cancelButton.pack(side = RIGHT, fill = 'both', expand = 'yes')
        buttonFrame.pack(side = BOTTOM, fill = 'both', expand = 'yes')
        
        # Make window appear 'on top' of the main window
        intialWin.transient(root)
        intialWin.grab_set()
        
        intialWin.bind("<Escape>", lambda x: intialWin.destroy())
        
        return
 
    # Choose a date mask file
    def createDateMaskWin (self):
        
        # Create list of types and open dialog window to get file path
        types = [('All files', '*')]
        dialog = filedialog.askopenfilename(filetypes = types)
        
        # If user picked a file, set date mask system variable
        if len(dialog) != 0:
            dateMask = dialog
            os.environ['DATEMSK'] = dialog
            
        return
        
    # Yview for scrollbar
    def yview (self, *args):

        # Set all yview methods with the same argument
        self.numberPanel.yview(*args)
        self.namePanel.yview(*args)
        self.propsPanel.yview(*args)
        self.subsPanel.yview(*args)
        self.summaryPanel.yview(*args)
        
    # Initialize GUI
    def initialize(self, frame, root):
        
        # Extract Events
        def extractEvents(self):
            
            # String that will be executed through the shell
            systemStr = "./caltool -extract e < " + currentFile 
                
            # Try executing system string
            try:
                extractStr = subprocess.check_output(systemStr, shell = True, stderr = subprocess.STDOUT)
                    
            # If error is received print error to log panel and scroll to the bottom of the panel
            except subprocess.CalledProcessError as exc:                                                                                                   
                self.logPanel.insert(END, "\n")                                                        
                self.logPanel.insert(END, exc.output)
                self.logPanel.see(END)
                
            # Otherwise print the output of the system call to the log panel and scroll to the bottom
            else:   
                self.logPanel.insert(END, "\n")                                                                                                
                self.logPanel.insert(END, extractStr)
                self.logPanel.see(END)
                
        # Extract Props
        def extractProps(self):
            
            # String that will be executed through the shell
            systemStr = "./caltool -extract x < " + currentFile
                
            # Try executing system string
            try:
                extractStr = subprocess.check_output(systemStr, shell = True, stderr = subprocess.STDOUT)
                    
             # If error is received print error to log panel and scroll to the bottom of the panel 
            except subprocess.CalledProcessError as exc:                                                                                                   
                self.logPanel.insert(END, "\n")                                                        
                self.logPanel.insert(END, exc.output)
                self.logPanel.see(END)
                
            # Otherwise print the output of the system call to the log panel and scroll to the bottom
            else:   
                self.logPanel.insert(END, "\n")                                                                                                
                self.logPanel.insert(END, extractStr)
                self.logPanel.see(END)
                
        # Show selected comp
        def showSelected(self):
            
            selectionList = self.numberPanel.curselection() # Get list of currently selected components
            value = selectionList[0] # Retrieve the first element from the list
            
            # If current selection isn't part of the header at the top of the FVP
            if (value != 0 and value != 1):
                writeFile("tempFile.ics", result[0], value-2, None) # Write current component to a temporary file
        
                # Open the temporary file, write its contents to the log panel and scroll to the bottom of the panel
                tempFile = open("tempFile.ics", 'r') 
                self.logPanel.insert(END, "\n")
                self.logPanel.insert(END, tempFile.read())
                self.logPanel.see(END)
                
        # Open file
        def openFile(self):
            
            # Confirm opneing a new file if unsaved changes are present
            def openConfirmed(self, closeConfirm):

                # Check if we can close confirmation window
                if (closeConfirm == True):
                    openConfirm.destroy()
                    
                # Create list of types and open dialog window to get file path
                types = [('All files', '*')]
                dialog = filedialog.askopenfilename(filetypes = types)
                
                # If user picked a file
                if len(dialog) != 0:
                    
                    global currentFile
                    global printCount
                    
                    currentFile = dialog
                    printCount = 1
            
                    systemStr = "./caltool -info < " + currentFile # String that will be executed through the shell
                    
                    # Try executing system string
                    try:
                        calInfoStr = subprocess.check_output(systemStr, shell = True, stderr = subprocess.STDOUT)
                        
                    # If error is received print error to log panel and scroll to the bottom of the panel 
                    except subprocess.CalledProcessError as exc:                                                                                                   
                        self.logPanel.insert(END, "\n")                                                        
                        self.logPanel.insert(END, exc.output)
                        self.logPanel.see(END)
                    
                    # Otherwise print the output of the system call to the log panel and scroll to the bottom
                    else:   
                        global result
                        
                        self.logPanel.insert(END, "\n")                                                                                                
                        self.logPanel.insert(END, calInfoStr)
                        self.logPanel.see(END)
                        
                        # Initialize result list and call readFile on file provided by user
                        result = []
                        result = readFile(currentFile, result)
                        
                        root.title(os.path.basename(currentFile)) # Set title of window to the name of file provided the user
                       
                        # Clear FVP
                        self.numberPanel.delete(2, END)
                        self.namePanel.delete(2, END)
                        self.propsPanel.delete(2, END)
                        self.subsPanel.delete(2, END)
                        self.summaryPanel.delete(2, END)
                        
                        # Iterate through result list ignoring the CalComp struct
                        for i in range (1, len(result) - 1):
                    
                            currentString = result[i].split(",", 3) # Tokenize the current string in the list
                                
                            self.numberPanel.insert(END, printCount) # Insert the number to the FVP
                            
                            printCount += 1 # Increment printCount
                            
                            # Insert name, prop count, and sub component count into FVP
                            self.namePanel.insert(END, currentString[0])
                            self.propsPanel.insert(END, currentString[1])
                            self.subsPanel.insert(END, currentString[2])
                            
                            # If there is a summary print it to the FVP
                            if len(currentString) == 4:
                                self.summaryPanel.insert(END, currentString[3])
                                
                            # If there isn't a summary print an empty string to the FVP
                            elif len(currentString) == 3:
                                self.summaryPanel.insert(END, "")
                        
                        # Enable all file menu buttons
                        fileMenu.entryconfig(1, state = NORMAL)
                        fileMenu.entryconfig(2, state = NORMAL)
                        fileMenu.entryconfig(3, state = NORMAL)
                        fileMenu.entryconfig(4, state = NORMAL)
                        
                        # Enable all database menu buttons
                        databaseMenu.entryconfig(0, state = NORMAL)
                        databaseMenu.entryconfig(1, state = NORMAL)
                        databaseMenu.entryconfig(3, state = NORMAL)

                        # Enable To-do filtering
                        toDoMenu.entryconfig(0, state = NORMAL)
                        
                        # Enable FVP Buttons
                        showSelectedButton.config(state = NORMAL)
                        extractEventsButton.config(state = NORMAL)
                        extractPropsButton.config(state = NORMAL)
                        
                        # Enable keyboard shortcuts for file menu buttons that were previously disabled
                        root.bind("<Control-s>", lambda x: save(self))
                        root.bind("<Control-t>", lambda x: toDoList(self))
                        root.bind("<Control-z>", lambda x: undoToDo(self))
                        
                        # Select the first entry in the FVP and set the openedFile boolean to True
                        self.numberPanel.selection_set(first = 2, last = None)
                        self.openedFile = True
                        
                    return
                
            # If there are unsavedChanges in the FVP
            if (self.unsavedChanges == True):
                
                # Create confirmation window
                openConfirm = Toplevel()
                openConfirm.minsize(height = 50, width = 600)
                openConfirm.maxsize(height = 50, width = 600)
                openConfirm.title("Confirmation")
                
                # Create a label that warns the user about the unsaved changes and pack it
                labelFrame = Frame(openConfirm)
                warningLabel = Label (labelFrame, text = "WARNING: You have unsaved changes in the FVP. All unsaved changes will be discarded.")
                warningLabel.pack()
                labelFrame.pack(side = TOP, fill = 'both', expand = 'yes')
                
                # Create buttons and pack them
                buttonFrame = Frame(openConfirm)
                continueButton = Button(buttonFrame, text = "Continue", command = lambda: openConfirmed(self, True))
                cancelButton = Button(buttonFrame, text = "Cancel", command = lambda: openConfirm.destroy())
                continueButton.pack(side = LEFT, fill = 'both', expand = 'yes')
                cancelButton.pack(side = RIGHT, fill = 'both', expand = 'yes')
                buttonFrame.pack(side = BOTTOM, fill = 'both', expand = 'yes')
                
                # Make the window modal
                openConfirm.transient(root)
                openConfirm.grab_set()
                root.wait_window(openConfirm)
                
            # Otherwise if there unsaved changes just open the dialog box to choose a file as usual
            elif (self.unsavedChanges == False):
                openConfirmed(self, False)
            
        # Save a file
        def save(self):
            
            # Write current comp to current file, remove unsaved changes signified in title and set unsavedChanges boolean to false
            writeFile(currentFile, result[0], -1, None)
            root.title(os.path.basename(currentFile))
            self.unsavedChanges = False
            
            return
            
        # Save as
        def saveAs(self):
            
            global currentFile
            
            # Create list of types and open dialog window to get file path
            types = [('All files', '*')]
            dialog = filedialog.asksaveasfilename(filetypes = types)
            
            # If user picked a file
            if len(dialog) != 0:
                
                # Write current comp to file selected by user, set title of window to the name of file given by user
                writeFile(dialog, result[0], -1, None)
                root.title(os.path.basename(dialog))
                
                # Set currentFile to the file path specified by the user and set unsavedChanges boolean to false 
                currentFile = dialog
                self.unsavedChanges = False
                
            return
            
        # Combine two files
        def combine(self):
            global result
            
            # Create list of types and open dialog window to get file path
            types = [('All files', '*')]
            dialog = filedialog.askopenfilename(filetypes = types)
            
            # If user picked a file
            if len(dialog) != 0:
                
                # Write current comp to a temporary file
                writeFile("tempFile.ics", result[0], -1, None)
                
                # String that will be executed through the shell using redirection
                systemStr = "./caltool -combine " + dialog + " < " + "tempFile.ics " + " > tempFile2.ics"
                
                 # Try executing system string
                try:
                    calInfoStr = subprocess.check_output(systemStr, shell = True, stderr = subprocess.STDOUT)
                    
                # If error is received print error to log panel and scroll to the bottom of the panel 
                except subprocess.CalledProcessError as exc:                                                                                                   
                    self.logPanel.insert(END, "\n")                                                        
                    self.logPanel.insert(END, exc.output)
                    self.logPanel.see(END)
                
                # Otherwise print the output of the system call to the log panel and scroll to the bottom
                else:  
                    
                    global printCount
                    
                    printCount = 1
                    
                    # Initialize result list and call readFile on file provided by user
                    result = []
                    result = readFile("tempFile2.ics", result)
                    
                    root.title(os.path.basename(currentFile) + "*") # Set title of window to the name of file provided the user
                    
                    # Clear FVP
                    self.numberPanel.delete(2, END)
                    self.namePanel.delete(2, END)
                    self.propsPanel.delete(2, END)
                    self.subsPanel.delete(2, END)
                    self.summaryPanel.delete(2, END)
                    
                    # Iterate through result list ignoring the CalComp struct
                    for i in range (1, len(result) - 1):
                
                        currentString = result[i].split(",", 3) # Tokenize the current string in the list
                            
                        self.numberPanel.insert(END, printCount) # Insert the number to the FVP
                        printCount += 1 # Increment printCount
                        
                        # Insert name, prop count, and sub component count into FVP
                        self.namePanel.insert(END, currentString[0])
                        self.propsPanel.insert(END, currentString[1])
                        self.subsPanel.insert(END, currentString[2])
                        
                        # If there is a summary print it to the FVP
                        if len(currentString) == 4:
                            self.summaryPanel.insert(END, currentString[3])
                            
                        # If there isn't a summary print an empty string to the FVP
                        elif len(currentString) == 3:
                            self.summaryPanel.insert(END, "")
                            
                    # Remove both temporary files that were created
                    os.remove("tempFile.ics")
                    os.remove("tempFile2.ics")
                    
                    self.unsavedChanges = True # Set unsavedChanges boolean to True
                
            return
              
        # Filter components
        def filterComp(self):
            
            # Filter upon button press
            def filterCommand(self, mode, fromDate, toDate):                                                    
                    
                tempFile = open("tempFile.ics", "w") # Create a temporary file for writing
                
                systemStr = "" # Create an empty string for system call
                
                # Add quotes to fromDate if it isn't empty or "today"
                if (fromDate != "" and fromDate != "today"):
                    
                    tempString = "\"" + fromDate + "\""
                    fromDate = tempString
                
                # Add quotes to toDate if it isn't empty or "today"    
                if (toDate != "" and toDate != "today"):
                    
                    tempString = "\"" + toDate + "\""
                    toDate = tempString
                    
                # If user selects Todo radio button
                if (mode.get() == 1):
                    
                    # If user enters both from and to date, set system string appropriately
                    if (fromDate != "" and toDate != ""):
                        systemStr = "./caltool -filter " + "t from " + fromDate + " to " + toDate + " < " + currentFile + " > tempFile.ics"
                    
                    # If user enters just to date, set system string appropriately
                    elif (fromDate == "" and toDate != ""):
                        systemStr = "./caltool -filter " + "t to " + toDate + " < " + currentFile + " > tempFile.ics"
                        
                    # If user enters just from date, set system string appropriately
                    elif (fromDate != "" and toDate == ""):
                        systemStr = "./caltool -filter " + "t from " + fromDate + " < " + currentFile + " > tempFile.ics"
                        
                    # If user enters neither from or to date, set system string appropriately
                    elif (fromDate == "" and toDate == ""):
                        systemStr = "./caltool -filter " + "t" + " < " + currentFile + " > tempFile.ics"
                    
                elif (mode.get() == 2):
                    
                    # If user enters both from and to date, set system string appropriately
                    if (fromDate != "" and toDate != ""):
                        systemStr = "./caltool -filter " + "e from " + fromDate + " to " + toDate + " < " + currentFile + " > tempFile.ics"
                    
                    # If user enters just to date, set system string appropriately
                    elif (fromDate == "" and toDate != ""):
                        systemStr = "./caltool -filter " + "e to " + toDate + " < " + currentFile + " > tempFile.ics"
                        
                    # If user enters just from date, set system string appropriately
                    elif (fromDate != "" and toDate == ""):
                        systemStr = "./caltool -filter " + "e from " + fromDate + " < " + currentFile + " > tempFile.ics"
                        
                    # If user enters neither from or to date, set system string appropriately
                    elif (fromDate == "" and toDate == ""):
                        systemStr = "./caltool -filter " + "e" + " < " + currentFile + " > tempFile.ics"
                                            
                # Try executing system string
                try:
                    calInfoStr = subprocess.check_output(systemStr, shell = True, stderr = subprocess.STDOUT)
                    
                # If error is received print error to log panel, scroll to the bottom of the panel and destroy filterWin
                except subprocess.CalledProcessError as exc:     
                    self.logPanel.insert(END, "\n")                                                        
                    self.logPanel.insert(END, exc.output)
                    self.logPanel.see(END)
                    filterWin.destroy()
                    
                # Otherwise print the output of the system call to the log panel and scroll to the bottom
                else:  
                    global printCount
                    global result
                    
                    printCount = 1 
                    
                    # Initialize result list and call readFile on file provided by user
                    result = []
                    result = readFile("tempFile.ics", result)
                    
                    # Removetemporary file and add an asterisk to the title
                    os.remove("tempFile.ics")
                    root.title(os.path.basename(currentFile) + "*")
                    
                    # Clear FVP
                    self.numberPanel.delete(2, END)
                    self.namePanel.delete(2, END)
                    self.propsPanel.delete(2, END)
                    self.subsPanel.delete(2, END)
                    self.summaryPanel.delete(2, END)
                    
                    # Iterate through result list ignoring the CalComp struct
                    for i in range (1, len(result) - 1):
                
                        currentString = result[i].split(",", 3) # Tokenize the current string in the list
                            
                        self.numberPanel.insert(END, printCount)  # Insert the number to the FVP
                        
                        printCount += 1 # Increment printCount
                        
                        # Insert name, prop count, and sub component count into FVP
                        self.namePanel.insert(END, currentString[0])
                        self.propsPanel.insert(END, currentString[1])
                        self.subsPanel.insert(END, currentString[2])
                        
                        # If there is a summary print it to the FVP
                        if len(currentString) == 4:
                            self.summaryPanel.insert(END, currentString[3])
                            
                        # If there isn't a summary print an empty string to the FVP
                        elif len(currentString) == 3:
                            self.summaryPanel.insert(END, "")
                
                    filterWin.destroy() # Close filter window
                    
                    self.unsavedChanges = True
                    
                return
                
            
            # Create filter window, set minimum size and title
            filterWin = Toplevel()
            filterWin.minsize(height = 150, width = 300)
            filterWin.title("Filter")
            
            # Create frames
            toDoFrame = Frame(filterWin)
            eventsFrame = Frame(filterWin)
            buttonsFrame = Frame(filterWin)
            
            val = IntVar() # IntVar() for radio buttons
            
            # Create Todo radio button and select it as default. Then pack it
            toDoRadio = Radiobutton(toDoFrame, text = "Todo Items", variable = val, value = 1)
            toDoRadio.pack()
            toDoRadio.select()
            
            # Create Events radio button then pack it
            eventsRadio = Radiobutton(eventsFrame, text = "Events", variable = val, value = 2)
            eventsRadio.pack()
            
            # Create label for from date then pack it
            fromLabel = Label (eventsFrame, text = "From:")
            fromLabel.pack()
            
            # Create entry for from date then pack it
            fromEntry = Entry(eventsFrame, width = 30)
            fromEntry.pack()
            
            # Create label for from date then pack it
            toLabel = Label (eventsFrame, text = "To:")
            toLabel.pack()
            
            # Create entry for from date then pack it
            toEntry = Entry(eventsFrame, width = 30)
            toEntry.pack()
            
            # Create filter and cancel buttons then pack them
            filterButton = Button(buttonsFrame, text = "Filter", command = lambda: filterCommand(self,val, fromEntry.get(), toEntry.get()))
            cancelButton = Button(buttonsFrame, text = "Cancel", command = lambda: filterWin.destroy())
            filterButton.pack(side = LEFT)
            cancelButton.pack(side = RIGHT)
        
            # Pack all frames
            toDoFrame.pack(side = TOP)
            eventsFrame.pack(side = TOP)
            buttonsFrame.pack(side = BOTTOM)
            
            # Make filterWin modal
            filterWin.transient(root)
            filterWin.grab_set()
            root.wait_window(filterWin)
            
            return
            
        # Exit the program
        def exitXCal(self):
            
            # Exit upon button press
            def exitConfirmed(self):
                
                # If a file was opened, free the current comp
                if (self.openedFile == True):
                    freeFile(result[0])
                    
                # Destroy confirmation window and close root window
                exitConfirm.destroy()
                root.quit()
                
            # Create exit confirmation window, set size, disable resizing and set title
            exitConfirm = Toplevel()
            exitConfirm.minsize(height = 50, width = 600)
            exitConfirm.maxsize(height = 50, width = 600)
            exitConfirm.title("Exit confirmation")
            
            # Create warning label, then pack it
            labelFrame = Frame(exitConfirm)
            warningLabel = Label (labelFrame, text = "WARNING: You are about to exit the program. All unsaved changes will be discarded.")
            warningLabel.pack()
            labelFrame.pack(side = TOP, fill = 'both', expand = 'yes')
            
            # Create Exit and Cancel buttons, then pack it
            buttonFrame = Frame(exitConfirm)
            exitButton = Button(buttonFrame, text = "Exit", command = lambda: exitConfirmed(self))
            cancelButton = Button(buttonFrame, text = "Cancel", command = lambda: exitConfirm.destroy())
            
            # Pack all frames
            exitButton.pack(side = LEFT, fill = 'both', expand = 'yes')
            cancelButton.pack(side = RIGHT, fill = 'both', expand = 'yes')
            buttonFrame.pack(side = BOTTOM, fill = 'both', expand = 'yes')
            
            # Make exit confirmation window modal
            exitConfirm.transient(root)
            exitConfirm.grab_set()
            root.wait_window(exitConfirm)
        
            return
            
        # Program information
        def aboutXCal(self):
            
            # Destroy window
            def destroyDateMaskWin():
                
                aboutWindow.destroy() # Destroy about window
                return
        
            # Create about window, disable resizing and set title
            aboutWindow = Toplevel()
            aboutWindow.minsize(height = 100, width = 250)
            aboutWindow.maxsize(height = 100, width = 250)
            aboutWindow.title("About xcal")
            
            # Add information label and pack
            aboutWindowLabel = Label (aboutWindow, text = "Program: Xcal\nName: Andrew Adams\nCompatible with iCalendar V2.0\n")
            aboutWindowLabel.pack()
            
            # Add exit button and pack
            aboutWindowExit = Button (aboutWindow, text = "Exit", command = destroyDateMaskWin)
            aboutWindowExit.pack()
            
            return
            
        # Remove todo items
        def toDoList(self):
            
            # Remove upon button proes
            def doneToDo(self):
                
                global printCount
                global result
                
                # If undo hasn't already been called and there are unsaved toDo changes
                if (self.resultUndo == False):
                    self.resultUndo = True
                    writeFile("undoFile.ics", result[0], -1, None) # Write current comp to 'undoFile.ics'
                    
                
                checkedList = []
                toBeRemoved = []
                
                # Iterate checkbox IntVar() variables and find which checkboxes are selected
                for i in range(0, len(buttonVarList)):
                    if (buttonVarList[i].get() == 1):
                        checkedList.append(i)
                        
                # Add indexes of components to be removed into toBeRemoved
                for i in range (0, len(checkedList)):
                    toBeRemoved.append(compIndexList[checkedList[i]])
                
                writeFile("tempFile.ics", result[0], -2, toBeRemoved) # Write to temporary file with todo items removed
    
                # Read from the newly created temporary file and then remove the temporary file
                result = []
                result = readFile("tempFile.ics", result)
                os.remove("tempFile.ics")
                
                # Clear the FVP
                self.numberPanel.delete(2, END)
                self.namePanel.delete(2, END)
                self.propsPanel.delete(2, END)
                self.subsPanel.delete(2, END)
                self.summaryPanel.delete(2, END)
                
                printCount = 1 
                
                # Iterate through result list ignoring the CalComp struct
                for i in range (1, len(result) - 1):
            
                    currentString = result[i].split(",", 3) # Tokenize the current string in the list
                        
                    self.numberPanel.insert(END, printCount) # Insert the number to the FVP
                    printCount += 1  # Increment printCount
                    
                    # Insert name, prop count, and sub component count into FVP
                    self.namePanel.insert(END, currentString[0])
                    self.propsPanel.insert(END, currentString[1])
                    self.subsPanel.insert(END, currentString[2])
                    
                    # If there is a summary print it to the FVP
                    if len(currentString) == 4:
                        self.summaryPanel.insert(END, currentString[3])
                        
                    # If there isn't a summary print an empty string to the FVP
                    elif len(currentString) == 3:
                        self.summaryPanel.insert(END, "")
                
                root.title(os.path.basename(currentFile) + "*") # Add asterisk to title to indicate unsaved changes
                toDoWin.destroy() # Destroy toDo window
                toDoMenu.entryconfig(1, state = NORMAL) # Activate undo button
                
                self.unsavedChanges = True # Set unsavedChanges boolean to True
                
                return
                
            # Create toDo window, set minimum size and set title
            toDoWin = Toplevel()
            toDoWin.minsize(height = 400, width = 300)
            toDoWin.title("To-do List")
            
            checkBoxFrame = Frame(toDoWin) # Create frames for checkboxes
            
            toDoCompList = [] # List for checkbox text labels
            compIndexList = [] # List for indexs of VTODO components in comp->comp
            
            # Iterate through all current comps in the result list
            for i in range (1, len(result) - 1):
        
                currentString = result[i].split(",", 3) # Tokenize result strings
                    
                # If we've run into a VTODO component
                if (currentString[0] == "VTODO"):
                    
                    # Add to list with summary information and index
                    if len(currentString) == 4:
                        toDoCompList.append("#" + str(i) + " - Summary: " + currentString[3])
                        compIndexList.append(i - 1)
                         
                    # Add to lsit with summary as 'None' and index
                    else:
                        toDoCompList.append("#" + str(i) + " - Summary: None")
                        compIndexList.append(i - 1)
                
            # Create scrollbar and text area
            scroll = Scrollbar(checkBoxFrame, orient = "vertical")
            text = Text(checkBoxFrame, width = 40, height = 20, yscrollcommand = scroll.set)
            scroll.config(command = text.yview)
            
            # Pack text area and scrollbar
            scroll.pack(side = RIGHT, fill = "y")
            text.pack(side = LEFT, fill = "both", expand = True)

            buttonVarList = [] # list of checkboxes
            
            # Iterate through all checkbox text labels and add checkboxes as we go
            for i in range(0, len(toDoCompList)):
                
                buttonVarList.append(IntVar())
                cb = Checkbutton(checkBoxFrame, text = toDoCompList[i], variable = buttonVarList[i])
                text.window_create(END, window = cb)
                text.insert(END, "\n") 
                
            checkBoxFrame.pack(side = TOP, fill = 'both', expand = 'yes') # Pack frame containing checkboxes
            
            # Creat button frame, buttons and then pack both
            buttonFrame = Frame(toDoWin) 
            filterButton = Button(buttonFrame, text = "Done", command = lambda: doneToDo(self))
            cancelButton = Button(buttonFrame, text = "Cancel", command = lambda: toDoWin.destroy())            
            filterButton.pack(side = LEFT, fill = 'both', expand = 'yes')
            cancelButton.pack(side = LEFT, fill = 'both', expand = 'yes')
            buttonFrame.pack(side = BOTTOM, fill = 'both', expand = 'no')
            
            return
            
        # Undo todo removal
        def undoToDo(self):
            
            # Undo upon confirmation
            def confirmUndo(self):
                
                global result
                
                # Clear FVP
                self.numberPanel.delete(2, END)
                self.namePanel.delete(2, END)
                self.propsPanel.delete(2, END)
                self.subsPanel.delete(2, END)
                self.summaryPanel.delete(2, END)
                
                printCount = 1
                
                result = readFile("undoFile.ics", result) # Read from 'undoFile.ics' to revert FVP back 
                
                # Iterate through result list ignoring the CalComp struct
                for i in range (1, len(result) - 1):
            
                    currentString = result[i].split(",", 3) # Tokenize the current string in the list
                        
                    self.numberPanel.insert(END, printCount) # Insert the number to the FVP
                    
                    printCount += 1 # Increment printCount
                    
                    # Insert name, prop count, and sub component count into FVP
                    self.namePanel.insert(END, currentString[0])
                    self.propsPanel.insert(END, currentString[1])
                    self.subsPanel.insert(END, currentString[2])
                    
                    # If there is a summary print it to the FVP
                    if len(currentString) == 4:
                        self.summaryPanel.insert(END, currentString[3])
                        
                    # If there isn't a summary print an empty string to the FVP
                    elif len(currentString) == 3:
                        self.summaryPanel.insert(END, "")
                        
                self.resultUndo = False
                
                root.title(os.path.basename(currentFile)) # Remove asterisk from file name
                
                os.remove("undoFile.ics") # Remove the temporary file created by the to Do function
                
                toDoMenu.entryconfig(1, state = DISABLED) # Disable the undo button
                
                undoWin.destroy() # Destroy the undo window
                
                return
            
            # Create undo window, disable resizing and set title
            undoWin = Toplevel()
            undoWin.minsize(height = 50, width = 500)
            undoWin.maxsize(height = 50, width = 500)
            undoWin.title("Undo")
            
            # Create warning label and pack
            labelFrame = Frame(undoWin)
            warningLabel = Label (labelFrame, text = "WARNING: All Todo components removed since the last save will be restored")
            warningLabel.pack()
            labelFrame.pack(side = TOP, fill = 'both', expand = 'yes')
            
            # Create undo and cancel button then pack both
            buttonFrame = Frame(undoWin)
            undoButton = Button(buttonFrame, text = "Undo", command = lambda: confirmUndo(self))
            cancelButton = Button(buttonFrame, text = "Cancel", command = lambda: undoWin.destroy())
            undoButton.pack(side = LEFT, fill = 'both', expand = 'yes')
            cancelButton.pack(side = RIGHT, fill = 'both', expand = 'yes')
            buttonFrame.pack(side = BOTTOM, fill = 'both', expand = 'yes')
            
            # Make window modal
            undoWin.transient(root)
            undoWin.grab_set()
            root.wait_window(undoWin)
            
            return
                    
        # Self variables
        self.resultUndo = False
        self.unsavedChanges = False
        self.openedFile = False
        
        menuBar = Menu(root) # Create menu bar
        
        # Create file menu and add neccessary buttons
        fileMenu = Menu (menuBar, tearoff = 0)
        fileMenu.add_command(label = "Open...", accelerator = "CTRL+O", command = lambda: openFile(self)) # , command = donothing, accelerator = "CRTL+O")
        fileMenu.add_command(label = "Save", accelerator = "CTRL+S", state = DISABLED, command = lambda: save(self)) # , command = donothing, accelerator = "CRTL+O")
        fileMenu.add_command(label = "Save as...", state = DISABLED, command = lambda: saveAs(self)) # , command = donothing, accelerator = "CRTL+O")
        fileMenu.add_command(label = "Combine...", state = DISABLED, command = lambda: combine(self))
        fileMenu.add_command(label = "Filter...", state = DISABLED, command = lambda: filterComp(self))
        fileMenu.add_separator()
        fileMenu.add_command(label = "Exit", accelerator = "CTRL+X", command = lambda: exitXCal(self))  #, command = donothing, accelerator = "CRTL+X")
        menuBar.add_cascade(label = "File", menu = fileMenu)
        
        # Create to do menu and add neccessary buttons
        toDoMenu = Menu (menuBar, tearoff = 0)
        toDoMenu.add_command(label = "To-do List...", accelerator = "CTRL+T", state = DISABLED, command = lambda: toDoList(self))
        toDoMenu.add_command(label = "Undo...", accelerator = "CTRL+Z", state = DISABLED, command = lambda: undoToDo(self))
        menuBar.add_cascade(label = "Todo", menu = toDoMenu)
        
        # Create help menu and add neccessary buttons
        helpMenu = Menu (menuBar, tearoff = 0)
        helpMenu.add_command(label = "Date Mask...", command = lambda: self.createDateMaskWin())
        helpMenu.add_command(label = "About xcal...", command = lambda: aboutXCal(self))
        menuBar.add_cascade(label = "Help", menu = helpMenu)
        
        # Create database menu and add neccessary buttons
        databaseMenu = Menu (menuBar, tearoff = 0)
        databaseMenu.add_command(label = "Store All", state = DISABLED)
        databaseMenu.add_command(label = "Store Selected", state = DISABLED)
        databaseMenu.add_command(label = "Clear")
        databaseMenu.add_command(label = "Status")
        databaseMenu.add_command(label = "Query")
        menuBar.add_cascade(label = "Database", menu = databaseMenu)
        
        # Create file view frame and log panel frame
        fileViewFrame = Frame(frame)
        logPanelFrame = Frame(frame)
        
        # Add keyboard binds for open and exit
        root.bind("<Control-o>", lambda x: openFile(self))
        root.bind("<Control-x>", lambda x: exitXCal(self))
        
        # Create scrollbar for FVP and link it with all 5 panels
        scroll = Scrollbar(fileViewFrame)
        scroll.configure(command = self.yview)
        
        # Create individual columns for each 
        self.numberPanel = Listbox(fileViewFrame, height = 20, width = 10, yscrollcommand=scroll.set)
        self.namePanel = Listbox(fileViewFrame, height = 20, width = 25, yscrollcommand=scroll.set)
        self.propsPanel = Listbox(fileViewFrame, height = 20, width = 10, yscrollcommand=scroll.set)
        self.subsPanel = Listbox(fileViewFrame, height = 20, width = 10, yscrollcommand=scroll.set)
        self.summaryPanel = Listbox(fileViewFrame, height = 20, width = 35, yscrollcommand=scroll.set)
        
        # Remove ability to select components in all columns except number
        self.namePanel.bindtags((self.namePanel, fileViewFrame, "all"))
        self.propsPanel.bindtags((self.propsPanel, fileViewFrame, "all"))
        self.subsPanel.bindtags((self.subsPanel, fileViewFrame, "all"))
        self.summaryPanel.bindtags((self.summaryPanel, fileViewFrame, "all"))
        
        # Create log panel text box and clear button for log panel
        self.logPanel = Text (logPanelFrame, height = 20, width = 90)
        clearButton = Button(frame, text = "Clear", command = lambda: self.logPanel.delete(1.0, END))
        
        # Create scrollbar for log panel
        scroll2 = Scrollbar(logPanelFrame)
        scroll2.configure(command = self.logPanel.yview)
        self.logPanel.configure(yscrollcommand=scroll2.set)
        
        # Add the menu bar
        root.config(menu = menuBar)
        
        # Pack the scrollbars
        scroll.pack(side = RIGHT, fill = Y)
        scroll2.pack(side = RIGHT, fill = Y)
        
        # Pack all FVP components
        self.numberPanel.pack(side = LEFT, fill = 'both', expand ='no')
        self.namePanel.pack(side = LEFT, fill = 'both', expand = 'yes')
        self.propsPanel.pack(side = LEFT, fill = 'both', expand = 'no')
        self.subsPanel.pack(side = LEFT, fill = 'both', expand = 'no')
        self.summaryPanel.pack(side = RIGHT, fill = 'both', expand = 'yes')
        
        # Pack the log panel and clear button
        self.logPanel.pack(side = TOP, fill = 'both', expand = 'yes')
        clearButton.pack(side = BOTTOM, fill = 'both', expand = 'yes')
        
        # Pack the FVP frame itself
        fileViewFrame.pack(side = TOP, fill = 'both', expand = 'yes')
        logPanelFrame.pack(side = BOTTOM, fill = 'both', expand = 'yes')
        
        # Create FVP buttons
        fvpButtonFrame = Frame(frame) 
        showSelectedButton = Button (fvpButtonFrame, text = "Show Selected", state = DISABLED, command = lambda: showSelected(self))
        extractEventsButton = Button (fvpButtonFrame, text = "Extract Events", state = DISABLED, command = lambda: extractEvents(self))
        extractPropsButton = Button (fvpButtonFrame, text = "Extract Props", state = DISABLED, command = lambda: extractProps(self))
        
        # Pack all FVP buttons and the FVP button frame
        showSelectedButton.pack(side = LEFT, fill = 'both', expand = 'yes')
        extractEventsButton.pack(side = LEFT, fill = 'both', expand = 'yes')
        extractPropsButton.pack(side = LEFT, fill = 'both', expand = 'yes')
        fvpButtonFrame.pack(side = BOTTOM, fill = 'both', expand = 'yes')

        # Add No. heading
        self.numberPanel.insert(END, "No.")
        self.numberPanel.insert(END, "-----------------------------------------")
        
        # Add Name heading
        self.namePanel.insert(END, "Name")
        self.namePanel.insert(END, "----------------------------------------------------------------"
                                   "----------------------------------------------------------------"
                                   "----------------------------------------------------------------")
                                
        # Add prop count heading
        self.propsPanel.insert(END, "Props")
        self.propsPanel.insert(END, "----------------------------------------------------------------")
        
        # Add sub component count heading
        self.subsPanel.insert(END, "Subs")
        self.subsPanel.insert(END, "------------------------------------------------------------------")
        
        # Add summary heading
        self.summaryPanel.insert(END, "Summary")
        self.summaryPanel.insert(END, "----------------------------------------------------------------"
                                      "----------------------------------------------------------------"
                                      "----------------------------------------------------------------")
                                      
        
        return
        

if __name__ == "__main__":

    # Initialize main window and set minimum size
    root = Tk()
    root.minsize(width = 750, height = 750)
    
    # Initialize the GUI and set the title
    xcal = xCalGUI(root)
    root.title('xcal')
    root.mainloop()

    


