#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));

    int randomNumber = rand() % 100 + 1;
    int guess;
    int attempts = 0;

    cout << "===== NUMBER GUESSING GAME =====" << endl;
    cout << "Guess a number between 1 and 100." << endl;

    do {
        cout << "\nEnter your guess: ";
        cin >> guess;
        attempts++;

        if (guess > randomNumber) {
            cout << "Too high! Try again." << endl;
        }
        else if (guess < randomNumber) {
            cout << "Too low! Try again." << endl;
        }
        else {
            cout << "Congratulations! You guessed the correct number."
                 << endl;
            cout << "Number of attempts: " << attempts << endl;
        }

    } while (guess != randomNumber);

    return 0;
}

