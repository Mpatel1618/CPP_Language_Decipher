/* ---------------------------------------------------------
countInputCharsFromFile.cpp

Author: Mayur Patel

Description of Program:
We want to figure out how frequency counts data might be useful in contexts,
such as knowing which letters to guess next in a hangman word-guess game, or
in decoding encrypted text.Also, to encrypted
explore whether or not the letter frequency counts would work as a signature for
a particular language, with the letter frequency counts functioning as a sort of
bar-code for that language
 -----------------------------------------------------------
*/

#include <iostream>     // For input and output
#include <cctype>       // For the letter checking functions
#include <fstream>      // For file input
#include <iomanip>      // For setw
#include <cstdlib>      // For exit()
#include <cstring>      // For strings

using namespace std;

const int COLUMNS = 8,         // Column size that should not change
          EXTRA_COLUMN = 9;    // Column size for the last part that should not change
#define LIMIT 26               // Max array size, which is number of alphabet
int combinedArrayCount[LIMIT][COLUMNS],                  // Array of letters counted with 26 rows and 8 columns
    combinedInputCount[LIMIT];                           // Array of letter counted from input
char frequencyArrayOrder[LIMIT][COLUMNS],                // Array of the frequencies of each letter in each language 
     frequencyLanguageInputOrder[LIMIT][EXTRA_COLUMN],   // Array of frequencies with the user input and the Macbeth files
     frequencyInputOrder[LIMIT];                         // Array of frequencies with just the user input

// Function the initilaize the arrays to a '0' or '#'
void initializeArrays() {
    for (int i = 0; i < LIMIT; ++i) {
        combinedInputCount[i] = 0;
        frequencyInputOrder[i] = '#';
        for (int j = 0; j < COLUMNS; ++j) {
            combinedArrayCount[i][j] = 0;
            frequencyArrayOrder[i][j] = '#';
        }
        for (int j = 0; j < EXTRA_COLUMN; ++j) {
            frequencyLanguageInputOrder[i][j] = '#';
        }
    }
}

// Function to print the header and options of the program
void printHeader() {
    cout << "\nSelect from the following stages of output to display:\n"
         << "1. Letter frequency counts\n"
         << "2. Letter frequency order\n"
         << "3. Get user input and display frequency counts\n"
         << "4. Get user input, display frequency counts, and display language\n"
         << "0. Exit the program\n"
         << "Your choice --> ";
}

// Function to take the Macbeth files and count the letter frequencies
void letterCountDisplay(string fileName, int& columnNumber) {
   ifstream inputFileStream;  // declare the input file stream
   char c = ' ';              // input character
   int i;                     // loop counter
    
   // open input file and verify
   inputFileStream.open(fileName.c_str());   // Convert C++ string to expected C-style string
   if (!inputFileStream.is_open()) {
        cout << "Could not find input file. Exiting..." << endl;
        exit(-1);
   }

   // Process input one character at at time, echoing input
   // Note that the input skips leading spaces, which is why we don't see
   // them as part of the output.
   while (inputFileStream >> c) {
      // convert alphabetic input characters to upper case
      if (isalpha(c)) {
          c = toupper(c);
          combinedArrayCount[c - 'A'][columnNumber]++; // update letter count, using the character as the index
          // cout << c << " ";    // Takes too long to display when enabled
      }
   }
   ++columnNumber;  // Each time it is called the column number changes to input a new language
   
   inputFileStream.close(); 
}

// Function to initialize the first array since all the other arrays run off of it
void initializeCountAssignments(int& columnNumber) {
   letterCountDisplay("MacbethEnglish.txt", columnNumber);
   letterCountDisplay("MacbethFinnish.txt", columnNumber);
   letterCountDisplay("MacbethFrench.txt", columnNumber);
   letterCountDisplay("MacbethGerman.txt", columnNumber);
   letterCountDisplay("MacbethHungarian.txt", columnNumber);
   letterCountDisplay("MacbethItalian.txt", columnNumber);
   letterCountDisplay("MacbethPortuguese.txt", columnNumber);
   letterCountDisplay("MacbethSpanish.txt", columnNumber);
}

// Function to swap to variables using pointers
void swap(int *xp, int *yp) 
{ 
    *xp = *yp; 
    *yp = *xp; 
} 

// Function used to sort the letters counted by most frequent to least
void sortInputArray() {
    int i,
        j,
        z,
        holder[LIMIT];     // Copycat to use to hold and compare with original arrray
  
    // Copies the original array to the holder
    for (i = 0; i < LIMIT; ++i) {
        holder[i] = combinedInputCount[i];
    }
    
    // Checks the number in each index and compares it to the next and if the next is bigger it calls the swap function
    for (i = 0; i < LIMIT; ++i) {
        for (j = 0; j < LIMIT - 1; ++j) {
            if (holder[j + 1] > holder[j]) {
                swap(holder[j+1], holder[j]);
            }
        }
    }
    
    // Takes the input recieved from the user that is in number form and changes it to letter form
    for (i = 0; i < LIMIT; ++i) {
        for (j = 0; j < LIMIT; ++j) {
            if (holder[i] == combinedInputCount[j]) {
                // Makes sure it does not repeat letters into the new array and breaks if there is a letter similar
                for (z = 0; z < LIMIT; ++z) {
                    if ((char)(j + 'A') == frequencyInputOrder[z]) {
                        break;
                    }
                }
                // Checks again to make sure no repeats happen and then skips to next iteration if there has been
                if ((char)(j + 'A') == frequencyInputOrder[z]) {
                        continue;
                }
                frequencyInputOrder[i] = (char)(j + 'A');
                break;
            }
        }
    }
}

// Function for the use of the user input, it takes the Macbeth array of letters and combines it with the user input array
void appendFrequencyArrays() {
    for (int i = 0; i < LIMIT; ++i) {
        frequencyLanguageInputOrder[i][COLUMNS] = frequencyInputOrder[i];   // Adds the last column with user input array of letters
        for (int j = 0; j < COLUMNS; ++j) {
            frequencyLanguageInputOrder[i][j] = frequencyArrayOrder[i][j];  // Adds the original Macbeth array of letters
        }
    }
}

// Function to count the letters used in the user input
void letterInputCount() {
   string userInput = " ";    // initialize the string
   
   cout << "\nCopy and paste a paragraph of text to be anayzed:\n\n";
        
   cin.ignore();              // Make sure the input stream is empty
   getline(cin, userInput);   // Get the user input
   
   // Checks if the user input are letters, changes them to upper case, then checks for the 
   // index by subtracting by the 'A' character and incremetning the index for each letter found
   for (int i = 0; i < userInput.size(); ++i) {
       if (isalpha(userInput[i])) {
           userInput[i] = toupper(userInput[i]);
           combinedInputCount[userInput[i] - 'A']++;
       }
   }
    
   sortInputArray();           // Calls the sortInputArray function
                   
   appendFrequencyArrays();    // Calls the appendFrequencyArrays function
}

// Function to sort the Macbeth arrays while taking the columnNumber parameter
void sortArray(int& columnNumber) {
    int i,
        j,
        holder[LIMIT];    // Holder to copy original count to
    
    for (i = 0; i < LIMIT; ++i) {
        holder[i] = combinedArrayCount[i][columnNumber];   // Copies array into holder
    }
    
    // Holder is checked for order and is set from big to small using swap
    for (i = 0; i < LIMIT; ++i) {
        for (j = 0; j < LIMIT - 1; ++j) {
            if (holder[j + 1] > holder[j]) {
                swap(holder[j+1], holder[j]);
            }
        }
    }
    
    // References back to the original array and gets index of array to assign characters
    for (i = 0; i < LIMIT; ++i) {
        for (j = 0; j < LIMIT; ++j) {
            if (holder[i] == combinedArrayCount[j][columnNumber]) {
                frequencyArrayOrder[i][columnNumber] = (char)(j + 'A');
            }
        }
    }
    ++columnNumber;   // Column number is increased everytime function is called
}

// Function to print the first option of the program
void printCase1() {
    // Prints the header for the output
    cout << endl << "Letter Frequency Counts:\n"
         << setw(9) << "Engl" << setw(8) << "Finn" << setw(8) << "Fren" << setw(8) << "Germ"
         << setw(8) << "Hung" << setw(8) << "Ital" << setw(8) << "Port" << setw(9) << "Span\n";
    
    // Reads through combined array of Macbeth and prints it in neat fashion using setw
    for (int i = 0; i < LIMIT; ++i) {
        cout << (char)(i + 'A') << ":";
       for (int j = 0; j < COLUMNS; ++j) {
           if (combinedArrayCount[i][j] != 0){
               cout << setw(7) << combinedArrayCount[i][j] << ' ';
           }
       }
       cout << endl;
    }
}

// Function to print the second option of the program
void printCase2() {
    // Header of the output
    cout << endl << "Letter Frequency Order:\n"
         << setw(9) << "Engl" << setw(8) << "Finn" << setw(8) << "Fren" << setw(8) << "Germ"
         << setw(8) << "Hung" << setw(8) << "Ital" << setw(8) << "Port" << setw(9) << "Span\n";
    
    // Reads through frequency order to print frquency characters of the Macbeth files
    for (int i = 0; i < LIMIT; ++i) {
       for (int j = 0; j < COLUMNS; ++j) {
           if (frequencyArrayOrder[i][j] != '#') {
               cout << setw(7) << frequencyArrayOrder[i][j] << ' ';
           }
       }
       cout << endl;
    }
}

// Function to print the third option of the program
void printCase3() {
    letterInputCount();    // Calls the letterInputCount function
    
    // Prints the user input character counts
    cout << endl;
    for (int i = 0; i < LIMIT; ++i) {
        cout << (char)(i + 'A') << ":" << combinedInputCount[i] << " ";
    }
    
    // Prints header of the output for this option
    cout << "\n\nLetter Frequency Order:\n"
         << setw(9) << "Engl" << setw(8) << "Finn" << setw(8) << "Fren" << setw(8) << "Germ"
         << setw(8) << "Hung" << setw(8) << "Ital" << setw(8) << "Port" << setw(8) << "Span"
         << setw(9) << "User\n";
    
    // Prints the combination of Macbeth and user input frequency characters
    for (int i = 0; i < LIMIT; ++i) {
        for (int j = 0; j < EXTRA_COLUMN; ++j) {
            cout << setw(7) << frequencyLanguageInputOrder[i][j] << ' ';
        }
        cout << endl;
    }
}

// Function to initialize the arrays of the program
void initializeProgram(int& columnNumber) {
   initializeArrays();
   initializeCountAssignments(columnNumber);
   
   columnNumber = 0;
   for (int i = 0; i < COLUMNS; ++i) {
       sortArray(columnNumber);
   }
}

// Function to just initliaze the user input arrays in case option is chosen again
void initializeUserInputArrays() {
    for (int i = 0; i < LIMIT; ++i) {
        combinedInputCount[i] = 0;
        frequencyInputOrder[i] = '#';
        for (int j = 0; j < EXTRA_COLUMN; ++j) {
            frequencyLanguageInputOrder[i][j] = '#';
        }
    }
}

// Function to find the matches of the user input frequency order and the other languages
// to figure out what language the user input in
int findMatches(int& columnNumber) {
    int matches = 0;
    
    // Checks both arrays for matches
    for (int i = 0; i < LIMIT; ++i) {
        if (frequencyInputOrder[i] == frequencyArrayOrder[i][columnNumber]) {
            ++matches;
        }
    }
    ++columnNumber;
    
    return matches;   // Returns matches
}

// Function to figure out which language is being used
void findLanguage(int& columnNumber) {
    int letterMatch[COLUMNS],   // Array using each index as a language
        holder[COLUMNS],        // Holder array to compare things with letterMatch array
        index = 0,
        i;
    
    // Calls function findMatches 8 times and assigns each index with whatever has returned
    for (i = 0; i < COLUMNS; ++i) {
        letterMatch[i] = findMatches(columnNumber);
        cout << letterMatch[i] << endl;
    }
    
    // letterMatch array gets copied to holder array
    for (i = 0; i < COLUMNS; ++i) {
        holder[i] = letterMatch[i];
    }
    
    // Holder array organizes from highest matches to lowest using swap
    for (i = 0; i < COLUMNS; ++i) {
        for (int j = 0; j < COLUMNS - 1; ++j) {
            if (holder[j + 1] > holder[j]) {
                swap(holder[j+1], holder[j]);
            }
        }
    }
    
    // Holder 0 would be the highest matches so it checks which index it was in the original array
    for (i = 0; i < COLUMNS; ++i) {
        if (holder[0] == letterMatch[i]) {
            index = i;
        }
    }
    
    // The index checked correlates to a certain language so whatever holder[0] came to be will print out
    switch (index) {
        case 0: cout << "Language is English\n";
                break;
            
        case 1: cout << "Language is Finnish\n";
                break;
            
        case 2: cout << "Language is French\n";
                break;
            
        case 3: cout << "Language is German\n";
                break;
            
        case 4: cout << "Language is Hungarian\n";
                break;
            
        case 5: cout << "Language is Italian\n";
                break;
            
        case 6: cout << "Language is Portuguese\n";
                break;
            
        case 7: cout << "Language is Spanish\n";
                break;
    }
}

// Main function of the program to bring everything together
int main() {
   int functionChoice,    // Variable to check what the user wants to do
       columnNumber = 0,  // Initializes the columnNumber to 0
       i;
   
   cout << "Language Decipher\n"
        << "For the program to work properly, please add significant amount of text for option 4.\n"; 
    
   initializeProgram(columnNumber);    // Calls the initializeProgram function before anything else is done
    
   while (true) {
       printHeader();              // Prints the headers of the program
       
       cin >> functionChoice;      // Gets user input for what they want to do
       
       switch (functionChoice) {   // Switch case to execute users decision
           case 1: printCase1();   // Runs case 1 and calls the printCase1 function
                   break;
               
           case 2: printCase2();   // Runs case 2 and calls the printCase2 function
                   break;
               
           case 3: initializeUserInputArrays();    // Calls initializeUserInputArrays to clean everything out before any arrays are used
                   printCase3();                   // Runs case 3 and calls the printCase3 function
                   break;
               
           case 4: initializeUserInputArrays();    // Calls initializeUserInputArrays to clean everything out before any arrays are used
                   printCase3();                   // Runs case 3 and calls the printCase3 function
                   columnNumber = 0;               // Initializes the columnNumber to 0
                   findLanguage(columnNumber);     // Calls the findLanguage function 
                   break;
               
           case 0: exit(0);       // Case 0 if user wants to exit the program
       }
   }
    
   return 0;   // Returns 0 to end the program once all lines in main have executed
}//end main()