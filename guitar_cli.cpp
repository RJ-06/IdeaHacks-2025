#include <iostream>
#include <string>    
#include <vector>
#include <hashmap>
#include <fstream>
#include <sstream>
using namespace std;


hashmap <string, int> stringNametoNumber = { //map of string names to their numbers
    {"Elow", 1}, {"A", 2}, {"D", 3}, {"G", 4}, {"B", 5}, {"Ehigh", 6}
};
vector <string> possibleChords = { //all possible chords - use as keys
    "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#",
    "Am", "A#m", "Bm", "Cm", "C#m", "Dm", "D#m", "Em", "Fm", "F#m", 
    "Gm", "G#m"
};


double delayTime = 1.0; //time to wait between notes in seconds - default half a second
bool isPlaying = false; //flag to check if the program is    playing a chord or not
int index = 0; //index to keep track of the current chord being played

//note - noteMap uses string noteName as key, value as [string, fingerPosition]
hashmap <string, [int, int]> noteMap = {}; //map of notes to their fret positions
vector <string, string> chordMap = {}; //map of chords to their notes
vector <string> chordScore = {}; //list of chords to autoplay

//cli stuff - use for parsing, delimiter
char betweenArgs = ' ';
char flagIndicator = '-';
char argIndicator = '/';


/*
*print info
*/

void printHelp() {
    cout << "Available commands:" << endl;
    cout << "1. /play <chord> - Play a chord - standard play must be stopped" << endl;
    cout << "2. /stop - Stop playing" << endl;
    cout << "3. /help - Show this help message" << endl;
    cout << "4. /delay <time> - Change delay time between notes" << endl;
    cout << "5. /chords - Show available chords" << endl;
    cout << "6. /exit - Exit the program" << endl;
}

void printAvailableChords() { //these are all possible chords available to play
    cout << "Available chords:" << endl;
    for (const auto& chord : chordScore) {
        cout << chord << " ";
    }
    cout << endl;
}

void printChordsList() //this is the specific list of chords
{
    cout << "Available chords:" << endl;
    for (const auto& chord : possibleChords) {
        cout << chord << " | ";
    }
    cout << endl;
}

void printCurrentChord() { //print the current chord being played
    if (isPlaying) {
        cout << "Currently playing: " << chordScore[index] << endl;
    } else {
        cout << "Not currently playing any chord." << endl;
    }
}

void printChordsNear(int n) { //print the chords in a range from -n to n
    cout << "Chords near " << value << ":" << endl;
    for(int i = index - n; i <= index + n; i++) { //print the chords in the range
        if (i >= 0 && i < chordScore.size()) { //check for valid index
            cout << chordScore[i] << " | ";
        }
    }
}

void printChordScore() { //print the list of chords to be played
    cout << "Chord score:" << endl;
    for (const auto& chord : chordScore) {
        cout << chord << " | ";
    }
    cout << endl;
}

/*
*edit values
*/
void changeDelayTime(string newDelay) { //set time between chord changes
    delay = stod(delayGiven); //convert string to double
    if (delay < 0) { //check for negative time
        cout << "Delay time cannot be negative." << endl;
        return;
    }
    delayTime = delay;
    cout << "Delay time changed to: " << delayTime << " seconds" << endl;
}

void resetScore() { //reset the playing flag
    isPlaying = false;
    chordScore.clear(); //clear the list of chords
    cout << "Reset Score." << endl;
}

void addToPossibleChords(int chordName, int strToPlay, int frtToPlay) 
{ //add a chord to the list of chords to be played - note must create new chord, and only then add to the list
    if (chordName == "") { //check for empty chord name
        cout << "Chord name cannot be empty." << endl;
        return;
    }
    if (strToPlay < 1 || strToPlay > 6) { //check for invalid string number
        cout << "Invalid string number. Must be between 1 and 6." << endl;
        return;
    }
    if (frtToPlay < 0 || frtToPlay > 5) { //check for invalid fret number
        cout << "Invalid fret number. Must be between 0 and 5." << endl;
        return;
    }
    noteMap[chordName] = { strToPlay, frtToPlay }; //add the chord to the map of chords to their notes
    cout << "Added chord: " << chordName << " to the list of chords to be played." << endl;
}

void deleteChordAtIndex(int index) { //delete a chord at a specific index
    if (index < 0 || index >= chordScore.size()) { //check for invalid index
        cout << "Invalid index. Must be between 0 and " << chordScore.size() - 1 << "." << endl;
        return;
    }
    chordScore.erase(chordScore.begin() + index); //remove the chord at the specified index
    cout << "Deleted chord at index: " << index << endl;
}

void addChordToScore(string chord) { //add a chord to the list of chords to be played
    if (chord == "") { //check for empty chord name
        cout << "Chord name cannot be empty." << endl;
        return;
    }
    if (find(possibleChords.begin(), possibleChords.end(), chord) == possibleChords.end()) { //check if the chord is valid
        cout << "Invalid chord name. Must be one of the available chords." << endl;
        return;
    }
    chordScore.push_back(chord); //add the chord to the list of chords to be played
    cout << "Added chord: " << chord << " to the list of chords to be played." << endl;
}

/*
*playback functionality
*/

void startPlaying()//play from 
{

}

void pausePlaying() { //stop playing the chord
    isPlaying = false;
    cout << "Stopped playing." << endl;
}

void continuePlaying()
{
    isPlaying = true; //set the flag to true
    cout << "Continuing to play." << endl;
}

void restartPlaying() { //restart the playing flag
    index = 0; //reset the index to 0
    cout << "Restarted playing from the beginning of the song." << endl;
}

void playChord(string chord) { //play a chord
    //INSERT FUNCTIONALITY HERE - use the noteMap to get the fret positions and play the notes
    int positions[2] = noteMap[chord]; //get the fret positions from the map
    int stringToPlay = positions[0]; //get the string to play from the map
    int fretToPlay = positions[1]; //get the fret to play from the map
    //play the note here - use the string and fret to play the note

    cout << "Playing chord: " << chord << endl; //remove after debugging
    //play the chord here - use the noteMap to get the fret positions and play the notes
    //use the delayTime to wait between notes
}