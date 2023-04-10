// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#ifndef POKEMON_H
#define POKEMON_H
#include <getopt.h>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

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

// only for part A
struct primsInfo {
    TerrainType terrain;
    bool visited;
    double distance; // will be the distance squared --> square root when outputting!
    uint32_t parentIndex;
};

struct vertex {
    int32_t x;
    int32_t y;
    primsInfo prims;

    vertex() : x(0), y(0) { }
    vertex(const int32_t &xIn, const int32_t &yIn) : x(xIn), y(yIn) { }
};

class pokemon {

private:

    std::vector<vertex> map;
    Mode mode = Mode::None;
    uint32_t numCoords;

    double upperBound; // for part B

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
    double distance(const vertex &v1, const vertex &v2) {

        // calculate euclidian distance and return
        if(v1.prims.terrain == v2.prims.terrain || v1.prims.terrain == TerrainType::Coast || v2.prims.terrain == TerrainType::Coast) {
            return (static_cast<double>(v1.x - v2.x))*(static_cast<double>(v1.x - v2.x)) 
                + (static_cast<double>(v1.y - v2.y))*(static_cast<double>(v1.y - v2.y));
        }

        return std::numeric_limits<double>::infinity();

    }

    // OUTPUTMST
    // used by run to output the solution
    void outputMST() {

        std::cout << calculateMST() << '\n';

        for(uint32_t i = 1; i < numCoords; i++) {
            if(map[i].prims.parentIndex < i) {
                std::cout << map[i].prims.parentIndex << ' ' << i << '\n';
            }
            else {
                std::cout << i << ' ' << map[i].prims.parentIndex << '\n';
            }
        }

    }

    // PART A
    // CALCULATEMST
    // used by outputMST and calculate FASTTSP to find the MST
    double calculateMST() {

        // first vertex is starting point
        map[0].prims.distance = 0;

        uint32_t currentIndex = 0;
        double minDistance;

        for(uint32_t i = 0; i < numCoords; i++) {

            minDistance = std::numeric_limits<double>::infinity();
            
            // find the vertex with the smallest distance, use that as current node
            for(uint32_t j = 0; j < numCoords; j++) {
                if(map[j].prims.visited == false && map[j].prims.distance < minDistance) {
                    minDistance = map[j].prims.distance;
                    currentIndex = j;
                }
            }
            //

            // verify that a valid node has been found
            // i.e. check that an MST can be constructed
            if(minDistance == std::numeric_limits<double>::infinity()) {
                std::cerr << "Cannot construct MST\n";
                std::exit(1);
            }
            //

            // set current node to visited
            map[currentIndex].prims.visited = true;

            // update all distances
            double dis;
            for(uint32_t j = 0; j < numCoords; j++) {
                dis = distance(map[currentIndex], map[j]);
                if(map[j].prims.visited == false && dis < map[j].prims.distance) {
                    map[j].prims.distance = dis;
                    map[j].prims.parentIndex = currentIndex;
                }
            }
            //

        }

        double totalWeight = 0;

        // need to find total weight of all edges
        for(uint32_t i = 1; i < numCoords; i++) {
            totalWeight += std::sqrt(map[i].prims.distance);
        }

        return totalWeight;

    }

    bool promising(const std::vector<vertex> &path, size_t permLength) {
        // do things
        path[0]; permLength++; // for compilation
        return true;
    }

    void genPerms(std::vector<vertex> &path, size_t permLength) {
        if (permLength == path.size()) {
            // Do something with the path
            return;
        }  // if ..complete path

        if (!promising(path, permLength)) {
            return;
        }  // if ..not promising

        for (size_t i = permLength; i < path.size(); ++i) {
            std::swap(path[permLength], path[i]);
            genPerms(path, permLength + 1);
            std::swap(path[permLength], path[i]);
        }  // for ..unpermuted elements
    }  // genPerms()

    // PART B
    // CALCULATEFASTTSP
    // used by run sim once all the input has been done to do part B
    void calculateFastTSP() {

        // not sure if this is right
        upperBound = calculateMST();

        std::vector<vertex> path; path.reserve(numCoords);

        genPerms(path, numCoords);

        // output path
    }

    // PART C
    // CALCULATEOPTTSP
    // used by run sim once all the input has been done to do part C
    void calculateOptTSP() {

    }

public:
    
    pokemon(const int &argc, char* argv[]) { getMode(argc, argv); }

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

                coord.prims.visited = false;
                coord.prims.distance = std::numeric_limits<double>::infinity();

                // check terrain type
                if(xIn > 0 || yIn > 0) coord.prims.terrain = TerrainType::Land;
                else if (xIn < 0 && yIn < 0) coord.prims.terrain = TerrainType::Sea;
                else coord.prims.terrain = TerrainType::Coast;

            }

            map.push_back(coord);
        }

        if(mode == Mode::MST) outputMST();
        else if(mode == Mode::FASTTSP) calculateFastTSP();
        else calculateOptTSP();

    }
};

#endif