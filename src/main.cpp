#include "tokeniser.hpp"
#include "interpreter.hpp"

#include <iostream>

using namespace Goob;

int main(void) {
    std::string input = "SET abc 100;\nSAY abc;\nMSGBOX \"Hello, World\";";
    Tokeniser myTokeniser = Tokeniser(input);
    
    for (auto const& token: myTokeniser.get_tokens()) {
        std::cout << token << std::endl;    
    }

    return 0;
}