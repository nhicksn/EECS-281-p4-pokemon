// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#ifndef POKEMON_H
#define POKEMON_H
#include <getopt.h>
#include <iostream>

enum class Mode {
    MST,
    FASTTSP,
    OPTTSP,
    None
};

struct vertex {

}

class pokemon {

private:

    Mode mode = Mode::None;

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

public:

    pokemon(int argc, char* argv[]) { getMode(argc, argv); }

};

#endif