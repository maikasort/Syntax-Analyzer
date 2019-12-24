#include <iostream>
#include<algorithm>
#include <string>
#include <cctype>
using namespace std;

//checks the vowel, goes through the array and sees if it matches then the function will go through the next function
//that will then delete the vowel from the whatever are in the command-line argument
bool vowelCheck(char ch) {
    //vowel check to see if a vowel exist if -> true
    char vowels[] = {'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'};
    char vcheck = find(begin(vowels), end(vowels), ch) != end(vowels);
    return vcheck;
}
//the actual function that will delete vowels from whatever argument it is
string deleteVowel(string dt) {
    auto del_vowel = remove_if(dt.begin(), dt.end(), vowelCheck);
    dt.erase(del_vowel, dt.end());
    return dt;
}
//this will just read the
int main(int argc, char *argv[]) {
    //true/false flags (basically everything in the command-line argument is set to off
    bool revStr = false;
    bool toUp = false;
    bool nonVowel = false;
    //so this real read the command-line argument starting from 1 because if we start from 0 it will read the title of
    //the argument as well (so basically no, read only the commands)
    for (int i = 1; i < argc; i++) {
        // basically put the command-line arguments into one variable which is m which makes it easier
        string m(argv[i]);

        //the on and off toggles -> if false do the toggles sets it on
        if (revStr == false) {
            if (m.length() % 2 == 0) {
                revStr = true;
            }
            //the on and off toggles -> if it is true then does the toggle but sets it off
        } else if (revStr == true) {
            if (m.length() % 2 == 0) {
                revStr = false;
            }
        }
        if (toUp == false) {
            if (m.length() % 3 == 0) {
                toUp = true;
            }
        } else if (toUp == true) {
            if (m.length() % 3 == 0) {
                toUp = false;
            }
        }

        if (nonVowel == false) {
            if (m.length() % 5 == 0) {
                nonVowel = true;
            }
        } else if (nonVowel == true) {
            if (m.length() % 5 == 0) {
                nonVowel = false;
            }
        }
        //if the toggle stands true then perform the function -> in this case reverse
        if (revStr == true) {
            for (int i = m.length() - 1; i >= 0; --i){

            }
            m = string(m.rbegin(), m.rend());

        }
        //perform the uppercase function
        if (toUp == true) {
            for (int i = 0; i < m.length(); i++) {
                m[i] = toupper(m[i]);
            }
        }

        if (nonVowel == true) {
            //calls the deleteVowel function and then sets the argument (m) and then sets the m equal to
            //whatever you get for the delete vowel function
            m = deleteVowel(m);

        }
        //will print out all the outputs 
        cout << m << endl;


    }
    return 0;
}
