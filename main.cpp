
// Winter'24
// Instructor: Diba Mirza
// Student name: Ashvik Vivekan
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[0] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile(argv[1]);

    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }

    set<movies> list;

    string line, movieName;
    double movieRating;

    while (getline(movieFile, line) && parseLine(line, movieName, movieRating)){
        list.insert(movies(movieName, movieRating));
    }

    movieFile.close();

    if (argc == 2){
        for (const movies& n : list){
            cout << n << endl;
        }
        return 0;
    }

    ifstream prefixFile(argv[2]);

    if (prefixFile.fail()){
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;

    while (getline(prefixFile, line)){
        if (!line.empty()){
            prefixes.push_back(line);
        }
    }

    prefixFile.close();

    vector<movies> bestMovies;
    vector<bool> found;

    for (string s : prefixes){
        vector<movies> matches;

        auto it = list.lower_bound(movies(s.substr(0, 1), 0));

        while (it != list.end() && it->name.compare(0, 1, s, 0, 1) == 0){
            if (it->name.find(s) == 0){
                matches.push_back(*it);
            }
            ++it;
        }

        sort(matches.begin(), matches.end(), [](const movies& a, const movies& b){
            if (a.rating == b.rating){
                return a.name < b.name;
            }
            return a.rating > b.rating;
        });

        if (matches.empty()){
            found.push_back(false);
            bestMovies.push_back(movies("", 0));
            cout << "No movies found with prefix " << s << endl;
        }
        else {
            found.push_back(true);
            bestMovies.push_back(matches[0]);

            for (const movies& movie : matches){
                cout << movie << endl;
            }
            cout << endl;
        }
    }

    for (int i = 0; i < prefixes.size(); i++){
        if (found[i]){
            cout << "Best movie with prefix " << prefixes[i] << " is: "
                 << bestMovies[i].name << " with rating "
                 << fixed << setprecision(1) << bestMovies[i].rating << endl;
        }
    }

    return 0;
}


/*
Part 3a: Time Complexity

Building the set of n movies takes O(n log n).

For each prefix, lower_bound finds the first movie with
the same starting character in O(log n).

I then check each movie beginning with that character
to see if its name matches the entire prefix.

If c movies share the first character and k match
the prefix, searching takes O(log n + c).

Sorting the matching movies takes O(k log(k+1)).
Printing the matches takes O(k).

Total: O(n log n + m log n + sum(c + k log(k+1))).

Runtimes using prefix_large.txt:
20 movies: 0.01 seconds
100 movies: 0.01 seconds
1000 movies: 0.02 seconds
76920 movies: 2.25 seconds

Runtime using prefix_medium.txt:
76920 movies: 0.13 seconds

The runtime increases with the number of movies and
prefixes. The large prefix list takes longer because
more searches and comparisons are performed.


Part 3b: Space Complexity

The set stores n movies.
The current prefix stores up to k matching movies.
The best movie results and match flags require O(m) space.

Total: O(n + m + k), excluding string storage.


Part 3c: Time/Space Tradeoffs

I kept my original approach of checking movie names
and sorting matching movies by rating, using
alphabetical order to break ties.

I improved the runtime by using lower_bound to skip
movies that do not begin with the prefix's first letter.

This reduces unnecessary comparisons while still
checking individual movie names for each prefix.

My algorithm uses additional memory to store matching
movies and the best movie for each prefix, but avoids
searching the entire set for every prefix.

The runtime for 76920 movies with the medium prefix
list was 0.13 seconds, compared to 2.25 seconds with
the large prefix list.

This shows that the number of prefixes affects the
runtime, while the first-character search helps
reduce unnecessary work.
*/

bool parseLine(string &line, string &movieName, double &movieRating){
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex + 1));

    if (movieName[0] == '"'){
        movieName = movieName.substr(1, movieName.length() - 2);
    }

    return true;
}
