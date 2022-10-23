#include "tokeniser.hpp"
#include "interpreter.hpp"

#include <iostream>
#include <fstream>

using namespace Goob;

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cout << "Please specify an input file" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::string path = argv[1];
    std::ifstream t(path);
    std::string input((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());

    Tokeniser myTokeniser = Tokeniser(input);
    
    for (auto const& token: myTokeniser.get_tokens()) {
        std::cout << token << std::endl;    
    }

    return 0;
}