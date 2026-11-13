#include "movies.h"

using namespace std;

ostream& operator<<(ostream& os, const movies& movie) {
    os << movie.name << ", " << movie.rating;
    return os;
}

bool operator<(const movies& curr, const movies& ri){
    return curr.name < ri.name;
}


