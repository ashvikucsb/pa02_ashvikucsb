#include <iostream>
#include <string>

struct movies
{
    std::string name;
    double rating;

    movies() : name(""),rating(0){}
    movies(std::string n, double r) : name(n),rating(r){}
};

std::ostream& operator<<(std::ostream& os, const movies& movie);

bool operator<(const movies& curr, const movies& ri);