#include "tokeniser.hpp"

#include <algorithm>
#include <iostream>

// All possible program operators
std::vector<char> Goob::Operators = {
    ' ', ';',
    '(', ')',
    '{', '}',
    '[', ']',
    '"', '"',
    '!', ','
};

// All arithmetic operators
std::vector<char> Goob::Arithmetic = {
    '+', '-', '*', '/', '^'
};

// All keywords the language uses
std::vector<std::string> Goob::Keywords = {
    "SET", "SAY", "MSGBOX"
};

// Return whether or not a character is considered as an operator
bool Goob::is_operator(char value) {
    return std::find(Operators.begin(), Operators.end(), value) != Operators.end();
}

// Return whether or not a character is considered as an arithmetic operator
bool Goob::is_arithmetic(char value) {
    return std::find(Arithmetic.begin(), Arithmetic.end(), value) != Arithmetic.end();
}


// Reads through the program character by character
Goob::Input::Input(std::string input, bool destroy_newlines = true) : m_Pos(0) {
    m_Input = input;

    if (destroy_newlines) {
        m_Input.erase( std::remove_if(
            m_Input.begin(),
            m_Input.end(),
            [](auto ch) { return (ch == '\n' || ch == '\r'); }
            ),
        m_Input.end());
    }
}

/* Gets the character at the current position
Can also accept an optional index, and whether it should be cached */
char Goob::Input::get_char(int32_t optional_index = 0, bool add_to_cache = true) {
    int index = m_Pos + optional_index;
    if (index < 0) return '\0';

    char a = m_Input[m_Pos + optional_index];
    if (add_to_cache && !is_operator(a))
        m_Cache += a;

    return a;
}

// Peek at the next character
char Goob::Input::peek_char() { return get_char(1, false); }
// Review the last character
char Goob::Input::review_char() { return get_char(-1, false); }
// Check whether the pointer has passed the end of the program
bool Goob::Input::has_next_char() const { return m_Pos < m_Input.length(); }
// Increment the pointer
void Goob::Input::increment() { m_Pos++; }
// Decrement the pointer
void Goob::Input::decrement() { m_Pos--; }
// Clear the Input cache
void Goob::Input::clear_cache() { m_Cache = ""; }
// Clears the last character in cache
void Goob::Input::pop_cache() { m_Cache.pop_back(); }

// Get the original program input
std::string Goob::Input::get_input() const { return m_Input; }
/* Pull the saved cache
Can also be cleared using this */
std::string Goob::Input::pull_cache(bool should_clear_cache = true) {
    std::string temp = m_Cache;
    if (should_clear_cache) clear_cache();
    return temp;
}

// Tokens

// Simple Token to represent parts of the program
Goob::Token::Token(std::string type, std::string value) {
    this -> type = type == "" ? "NAME" : type;
    this -> value = value;
}

// Getter for the Token's type
std::string Goob::Token::get_type() const { return type; }
// Getter for the Token's value
std::string Goob::Token::get_value() const { return value; }

std::ostream& operator<<(std::ostream& stream, const Goob::Token& token) {
    return stream << static_cast<std::string>(token);
};

// Tokeniser

// Split the program into it's constituant tokens
Goob::Tokeniser::Tokeniser(std::string program) {
    // 1) Deal with comments
    // 2) Tokenise!

    bool inlineComment = false;
    bool multilineComment = false;
    bool shouldBeString = false;

    std::string next;

    Input commentPass = Input(program, false);

    while (commentPass.has_next_char()) {
        char current = commentPass.get_char();

        switch (current) {
            case '"':
                if (inlineComment || multilineComment)
                    break;
                
                if (commentPass.review_char() == '\\')
                    break;
                
                shouldBeString = !shouldBeString;
                break;
            
            case '!':
                if (shouldBeString)
                    break;
                
                if (commentPass.peek_char() == '[')
                    if (!inlineComment)
                        multilineComment = true;
                else
                    inlineComment = true;
                
                break;

            case ']':
                if (multilineComment) {
                    multilineComment = false;
                    commentPass.increment();
                    continue;
                }
                break;

            case '\n':
                inlineComment = false;
                break;


            default:
                break;
        }

        if (!multilineComment && !inlineComment)
            next += current;
        
        commentPass.increment();
    }

    
    bool isString = false;

    std::string string_cache = "";
    std::cout << next;

    Input cInput = Input(next, true);
    while (cInput.has_next_char()) {
        char current = cInput.get_char();
        // std::cout << current;


        if (isString) {
            if (current == '"' && commentPass.review_char() != '\\') {
                isString = false;
                Token cur_string = Token("STRING", string_cache);
                tokens.push_back(cur_string);
                cInput.clear_cache();
            } else if (current == '\\') { // Handle escape characters
                switch (cInput.peek_char()) {
                    case 'a':
                        string_cache += '\a';
                        break;

                    case 'b':
                        string_cache += '\b';
                        break;

                    case 'f':
                        string_cache += '\f';
                        break;
                    
                    case 'n':
                        string_cache += '\n';
                        break;
                    
                    case 'r':
                        string_cache += '\r';
                        break;

                    case 't':
                        string_cache += '\t';
                        break;
                    
                    case 'v':
                        string_cache += '\v';
                        break;

                    case '0':
                        string_cache += '\0';
                        break;

                    default:
                        string_cache += cInput.peek_char();
                        break;
                }    
                
                cInput.increment();
            } else {
                string_cache += current;
            }
        } else {
            if (is_operator(current)) {
                switch (current) {                    
                    case ' ': {
                        pull_from_cache(&cInput);
                        break;
                    }

                    case ';': {
                        pull_from_cache(&cInput);
                        break;
                    }

                    case '"':
                        cInput.clear_cache();
                        string_cache = "";
                        isString = true;
                        break;
                    
                    default:
                        Token op = Token("OPERATOR", std::string(1, current));
                        tokens.push_back(op);
                        break;
                }
            } else if (is_arithmetic(current)) {
                Token op = Token("ARITHMETIC", std::string(1, current));
                tokens.push_back(op);
                cInput.clear_cache();
            }
        }

        cInput.increment();
    }
}

// Pulls data from cache and stores it as a NAME
void Goob::Tokeniser::pull_from_cache(Input* cInput) {
    std::string data = cInput->pull_cache();
    if (data == "") return;

    std::string token_type = "NAME";

    if (std::find(Keywords.begin(), Keywords.end(), data) != Keywords.end())
        token_type = "KEYWORD";

    Token name = Token(token_type, data);
    tokens.push_back(name);
}

// Return all the processed tokens
std::vector<Goob::Token> Goob::Tokeniser::get_tokens() { return this -> tokens; }