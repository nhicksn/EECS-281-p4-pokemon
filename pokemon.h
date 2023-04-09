// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#ifndef POKEMON_H
#define POKEMON_H
#include <getopt.h>
#include <iostream>
#include <vector>
#include <limits>

enum class Mode {
    MST,
    FASTTSP,
    OPTTSP,
    None
};

enum class TerrainType {
    Land,
    Sea,
    Coast
};

struct vertex {
    int32_t x;
    int32_t y;

    // only for part A
    TerrainType terrain;
    bool visited;
    double distance; // will be the distance squared --> square root when outputting!
    uint32_t parentIndex;

    vertex() : x(0), y(0) { }
    vertex(int32_t xIn, int32_t yIn) : x(xIn), y(yIn) { }
    vertex(int32_t xIn, int32_t yIn, TerrainType tIn) : x(xIn), y(yIn), terrain(tIn) { }
};

class pokemon {

private:

    std::vector<vertex> map;
    Mode mode = Mode::None;
    uint32_t numCoords;

    void printHelp() {
        std::cout << "Usage: ./poke [-h] || [-m && [MST || FASTTSP || OPTTSP]]\n";
        std::cout << "You must specify a mode for the program; the options are:\n";
        std::cout << "MST (minimum spanning tree);\n";
        std::cout << "FASTTSP (find the fastest solution);\n";
        std::cout << "OPTTSP (find the optimal solution)\n";
    } // printHelp

    void getMode(const int &argc, char *argv[]) {
        opterr = false;
        int choice;
        int index = 0;
        option long_options[] = {
            { "help",         no_argument,       nullptr, 'h'  },
            { "mode",         required_argument, nullptr, 'm'  },
            { nullptr,        0,                 nullptr, '\0' },
        }; // long_options

        while((choice = getopt_long(argc, argv, "hm:", long_options, &index)) != -1) {
            switch(choice) {
                case 'h': {
                    printHelp();
                    std::exit(0);
                } // case 'v'
                
                case 'm' : {
                    std::string arg{optarg};
                    if(arg == "MST") {
                        mode = Mode::MST;
                    }
                    else if(arg == "FASTTSP") {
                        mode = Mode::FASTTSP;
                    }
                    else if(arg == "OPTTSP") {
                        mode = Mode::OPTTSP;
                    }
                    else {
                        std::cerr << "Error: Invalid mode\n";
                        std::exit(1);
                    }
                    break;
                } // case 'm'

                default: {
                    std::cerr << "Error: Invalid command line option\n";
                    exit(1);
                } // default case
            } // switch choice
        } // while choice
        if(mode == Mode::None) {
            std::cerr << "Error: No mode specified\n";
            exit(1);
        } // if
    } //getMode

    // FINDDISTANCE
    // used by calculateMST to find the distance between two vertices
    // returns infinity if they cannot be connected
    double distance(vertex v1, vertex v2) {

        // calculate euclidian distance and return
        if(v1.terrain == v2.terrain || v1.terrain == TerrainType::Coast || v2.terrain == TerrainType::Coast) {
            return (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y);
        }

        return std::numeric_limits<double>::infinity();

    }

    // PART A
    // CALCULATEMST
    // used by run sim once all the input has been done to do part A
    void calculateMST() {

        // first vertex is starting point
        map[0].distance = 0;

        for(uint32_t i = 0; i < numCoords; i++) {
            
            // update distances
            for(uint32_t j = 0; j < numCoords; j++) {

                // make sure the node hasn't been visited
                if(map[j].visited == true || j == i) {
                    continue;
                }

                // calculate distance and see if it's better than the current distance
                double dis = distance(map[i], map[j]);

                // if so, update
                if(dis < map[j].distance) {
                    map[j].distance = dis;
                    map[j].parentIndex = i;
                }

            }

            
        }

    }

    // PART B
    // CALCULATEFASTTSP
    // used by run sim once all the input has been done to do part B
    void calculateFastTSP() {

    }

    // PART C
    // CALCULATEOPTTSP
    // used by run sim once all the input has been done to do part C
    void calculateOptTSP() {

    }

public:
    
    pokemon(int argc, char* argv[]) { getMode(argc, argv); }

    // RUN
    // called by the user to find the solution they're looking for, reads input
    // and calls helper function to find the correct tree
    void run() {

        // read the input
        std::cin >> numCoords;
        for(uint32_t i = 0; i < numCoords; i++) {

            // initialize coord with x and y coordinate
            vertex coord;
            int32_t xIn; std::cin >> xIn;
            int32_t yIn; std::cin >> yIn;
            coord.x = xIn; coord.y = yIn;

            // fill in info needed for prims
            if(mode == Mode::MST) {

                coord.visited = false;
                coord.distance = std::numeric_limits<double>::infinity();

                // check terrain type
                if(xIn > 0 || yIn > 0) coord.terrain = TerrainType::Land;
                else if (xIn < 0 && yIn < 0) coord.terrain = TerrainType::Sea;
                else coord.terrain = TerrainType::Coast;

            }

            map.push_back(coord);
        }

        if(mode == Mode::MST) calculateMST();
        else if(mode == Mode::FASTTSP) calculateFastTSP();
        else calculateOptTSP();

    }
};

#endif