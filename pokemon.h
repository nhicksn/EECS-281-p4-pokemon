// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#ifndef POKEMON_H
#define POKEMON_H
#include <getopt.h>
#include <iostream>
#include <vector>
#include <cfloat>
#include <cmath>
#include <numeric>

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
    bool visited = false;
    uint32_t distance = UINT32_MAX; // will be the distance squared --> square root when outputting!
    uint32_t parentIndex;
};

struct vertex {
    int32_t x;
    int32_t y;
    uint32_t index; // for part C

    vertex() : x(0), y(0) { }
    vertex(const int32_t &xIn, const int32_t &yIn, const int32_t &indexIn) : x(xIn), y(yIn), index(indexIn) { }
};

class pokemon {

private:

    std::vector<vertex> map;
    Mode mode = Mode::None;
    uint32_t numCoords;

    std::vector<primsInfo> prims; // for part A

    // for part C
    double upperBound;
    std::vector<uint32_t> optPath;
    std::vector<uint32_t> currentPath;
    double currentWeight = 0;
    //

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
    double distance(const uint32_t &index1, const uint32_t &index2) {

        // calculate euclidian distance and return
        if(prims[index1].terrain == prims[index2].terrain || prims[index1].terrain == TerrainType::Coast || prims[index2].terrain == TerrainType::Coast) {
            return double(map[index1].x - map[index2].x)*double(map[index1].x - map[index2].x) 
                + double(map[index1].y - map[index2].y)*double(map[index1].y - map[index2].y);
        }

        return DBL_MAX;

    }

    // PART A
    // CALCULATEMST
    // used by run to find the MST
    double calculateMST(const bool &output) {

        // first vertex is starting point
        prims[0].distance = 0;

        uint32_t currentIndex = 0;
        double minDistance;
        double dis;

        for(uint32_t i = 0; i < numCoords; i++) {

            minDistance = UINT32_MAX;
            
            // find the vertex with the smallest distance, use that as current node
            for(uint32_t j = 0; j < numCoords; j++) {
                if(prims[j].visited == false && prims[j].distance < minDistance) {
                    minDistance = prims[j].distance;
                    currentIndex = j;
                }
            }
            //

            // verify that a valid node has been found
            // i.e. check that an MST can be constructed
            if(minDistance == UINT32_MAX) {
                std::cerr << "Cannot construct MST\n";
                std::exit(1);
            }
            //

            // set current node to visited
            prims[currentIndex].visited = true;

            // update all distances
            for(uint32_t j = 0; j < numCoords; j++) {
                dis = distance(currentIndex, j);
                if(prims[j].visited == false && dis < prims[j].distance) {
                    prims[j].distance = uint32_t(dis);
                    prims[j].parentIndex = currentIndex;
                }
            }
            //

        }

        double totalWeight = 0;

        // need to find total weight of all edges
        for(uint32_t i = 1; i < numCoords; i++) {
            totalWeight += std::sqrt(prims[i].distance);
        }

        if(!output) return totalWeight;

        std::cout << totalWeight << '\n';

        for(uint32_t i = 1; i < numCoords; i++) {
            if(prims[i].parentIndex < i) {
                std::cout << prims[i].parentIndex << ' ' << i << '\n';
            }
            else {
                std::cout << i << ' ' << prims[i].parentIndex << '\n';
            }
        }

        return totalWeight;

    }

    // DISTANCETSP
    // calculates the distance between two points, disregarding terrain constraints
    // used by calculateFastTsp and calculateOptTSP
    double distanceTSP(const uint32_t &index1, const uint32_t &index2) {
        return std::sqrt(double(map[index1].x - map[index2].x) * double(map[index1].x - map[index2].x) +
                double(map[index1].y - map[index2].y) * double(map[index1].y - map[index2].y));
    }

    // PART B
    // CALCULATEFASTTSP
    // used by run sim once all the input has been done to do part B
    double calculateFastTSP(const bool &output) {

        std::vector<uint32_t> path; path.reserve(numCoords + 1);
        path.push_back(0); path.push_back(1);
        path.push_back(2); path.push_back(0);
        // path starts out with A -> B -> C -> A

        double minDistance;
        double calcDistance;
        uint32_t bestIndex;

        for(uint32_t i = 3; i < numCoords; i++) {
            minDistance = DBL_MAX;
            for(uint32_t j = 0; j < path.size() - 1; j++) {
                calcDistance = distanceTSP(path[j], i) + distanceTSP(i, path[j + 1]) - distanceTSP(path[j], path[j + 1]);
                if(minDistance > calcDistance) {
                    minDistance = calcDistance;
                    bestIndex = j;
                }
            }
            path.insert(path.begin() + bestIndex + 1, i);
        }

        double totalWeight = 0;

        // calculate weight
        for(uint32_t i = 0; i < numCoords; i++) {
            totalWeight += distanceTSP(path[i], path[i + 1]);
        }

        if(output) {
            std::cout << totalWeight << '\n';

            for(uint32_t i = 0; i < numCoords; i++) {
                std::cout << path[i] << ' ';
            }

            std::cout << '\n';
        }

        return totalWeight;
    }

    // PARTIALMST --> TODO
    // used by promising to calculate a partial MST containing only vertices not in the partial solution
    double partialMST(const std::vector<std::pair<uint32_t, uint32_t>> &vertices) {
        vertices[0];
        return 0;
    }

    // PROMISING --> TODO
    // used by genPerms to see if a partial solution should be pruned
    bool promising(const size_t &permLength) {

        // from IA notes:
        // calculate MST of remaining vertices, and connect it to the partial solution
        // with the shortest arms possible
        // compare to the upper bound, and decide if promising

        // if cost of calculating is more than just returning true
        if(currentPath.size() - permLength < 5) return true;


        // calculate MST of remaining vertices
        double expectedWeight = 0;


        // connect to partial solution with the shortest arms possible


        // calculate total weight


        // compare to upper bound
        if(expectedWeight > upperBound) return false;
        return true;
    }

    // GENPERMS
    // finds the optimal TSP solution
    // used by calculateOPTTSP for part C
    void genPerms(const size_t &permLength) {
        if (permLength == numCoords) {
            // connect path back to the beginning
            currentWeight += distanceTSP(currentPath[numCoords - 1], currentPath[0]);
            if(currentWeight < upperBound) {
                optPath = currentPath;
                upperBound = currentWeight;
            }
            return;
        }  // if ..complete path

        if (!promising(permLength)) {
            return;
        }  // if ..not promising

        for (size_t i = permLength; i < numCoords; ++i) {
            std::swap(currentPath[permLength], currentPath[i]);
            currentWeight += distanceTSP(currentPath[permLength - 1], currentPath[permLength]);
            genPerms(permLength + 1);
            currentWeight -= distanceTSP(currentPath[permLength - 1], currentPath[permLength]);
            std::swap(currentPath[permLength], currentPath[i]);
        }  // for ..unpermuted elements
    }  // genPerms()


    // PART C
    // CALCULATEOPTTSP
    // used by run sim once all the input has been done to do part C
    void calculateOptTSP() {
        upperBound = calculateFastTSP(false); // this is the upper bound used for pruning
        optPath.resize(numCoords); std::iota(begin(optPath), end(optPath), 0);
        currentPath.resize(numCoords); std::iota(begin(currentPath), end(currentPath), 0);

        genPerms(1);

        std::cout << upperBound << '\n';

        for(uint32_t i = 0; i < numCoords; i++) {
            std::cout << optPath[i] << ' ';
        }

        std::cout << '\n';
    }

public:
    
    pokemon(const int &argc, char* argv[]) { getMode(argc, argv); }

    // RUN
    // called by the user to find the solution they're looking for, reads input
    // and calls helper function to find the correct tree
    void run() {

        // read the input
        std::cin >> numCoords;
        map.reserve(numCoords);
        if(mode == Mode::MST) prims.reserve(numCoords);
        vertex coord;
            for(uint32_t i = 0; i < numCoords; i++) {

            // initialize coord with x and y coordinate
            int32_t xIn; std::cin >> xIn;
            int32_t yIn; std::cin >> yIn;
            coord.x = xIn; coord.y = yIn; coord.index = i;

            // fill in info needed for prims
            if(mode == Mode::MST) {

                primsInfo coordInfo;

                // check terrain type
                if(xIn > 0 || yIn > 0) coordInfo.terrain = TerrainType::Land;
                else if (xIn < 0 && yIn < 0) coordInfo.terrain = TerrainType::Sea;
                else coordInfo.terrain = TerrainType::Coast;

                prims.push_back(coordInfo);

            }

            map.push_back(coord);
        }

        if(mode == Mode::MST) calculateMST(true);
        else if(mode == Mode::FASTTSP) calculateFastTSP(true);
        else calculateOptTSP();

    }
};

#endif