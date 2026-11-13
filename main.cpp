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
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    set<movies> list;

    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        list.insert(movies(movieName, movieRating));
    }

    movieFile.close();

    if (argc == 2){
        for (const movies& n : list){
            cout << n << endl;
        }
        return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message

    vector<movies> bestMovies;
    vector<bool> found;

    for (const string& s : prefixes){
        vector<movies> matches;
        auto it = list.lower_bound(movies(s, 0));

        while (it != list.end() && it->name.find(s) == 0){
            matches.push_back(*it);
            ++it;
        }

        sort(matches.begin(), matches.end(), [](const movies& a, const movies& b) {
            if (a.rating == b.rating)
                return a.name < b.name;
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

For each of m prefixes, I search n movies, taking O(mnl).
I sort up to k matches per prefix, taking O(mkl log k).
Printing all matches takes O(mkl).
Finding the best movie takes O(m) using the first element.

Total: O(mnl + mkl log(k+1))

Runtimes using prefix_large.txt:
20 movies: 20 ms
100 movies: 40 ms
1000 movies: 450 ms
76920 movies: 79690 ms

Runtime increases as n increases

Part 3b: Space Complexity

I store up to k movies for each of m prefixes.
Each movie name has at most l characters.

Total additional space: O(mkl)


Part 3c: Time/Space Tradeoffs

I designed my algorithm to sort movies by rating and
alphabetically for ties, making the best movie easy to find.

My target was O(mnl + mkl log(k+1)) time and O(mkl) space.
I achieved both targets, but scanning all n movies for
each prefix makes the algorithm slower for large datasets.

Improving time complexity would require a faster
prefix-search method, such as lower_bound().
*/


bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
