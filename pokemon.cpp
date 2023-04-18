// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include "pokemon.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    // cerr << fixed << showpoint << setprecision(2) << boolalpha;
    cout << std::setprecision(2); //Always show 2 decimal places
    cout << std::fixed; //Disable scientific notation for large numbers
    pokemon p(argc, argv);
    // cerr << "Path" << setw(33) << "PL" << setw(10) << "curCost" << setw(10) << "arm 1";
    // cerr << setw(10) << "arm 2" << setw(10) << "MST" << setw(10) << "Total" << "  ";
    // cerr << "Promising?" << '\n';
    p.run();
}