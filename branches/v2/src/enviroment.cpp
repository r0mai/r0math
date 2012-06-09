
#include "enviroment.hpp"

#include <iostream>
#include <exception>
#include <ctime>
#include <cstdlib>

//#include <boost/filesystem.hpp>

namespace r0 {

enviroment::enviroment() : print_error_messages(true) {}

int enviroment::run() {
    try {
#if 0
        //create directory graphics
        {
            namespace fs = boost::filesystem;
            fs::path pt("graphics");
            if ( !fs::exists( pt ) ) {
                fs::create_directory( pt );
            }
        }
#endif
        std::srand( static_cast<unsigned>(std::time(0)) );

        std::string line;
        std::cout << "> ";
        while ( std::getline(std::cin, line) ) {
            if ( line == "exit" ) {
                break;
            } else {
                work_on_input(line);
            }
            std::cout << "\n> ";
        }
    } catch ( const std::exception& x ) {
        std::cout << "Exception caught " << x.what() << std::endl;
        std::cin.ignore();
        return 1;
    } catch ( ... ) {
        std::cout << "Unknown object caught" << std::endl;
        std::cin.ignore();
        return 1;
    }
 
    return 0;
}

void enviroment::raise_error(const std::string& function_name, const std::string& error_text) {
    if (print_error_messages) {
        std::cout << "Error @ " << function_name << "[] : " << error_text << std::endl;
    }
}

symbol_map& enviroment::get_symbols() {
    return symbols;
}

const symbol_map& enviroment::get_symbols() const {
    return symbols;
}

void enviroment::work_on_input(std::string input) {

    parser_expression_tree parse_result;

    std::clock_t start = std::clock();

    if ( !input.empty() && input[0] == '/' ) {
        input = input.substr( 1, input.size() - 1 );
        print_debug = true;
    } else {
        print_debug = false;
    }

    bool success = lex.do_lex(input, parse_result);

    if (!success) {
        std::cout << "parsing failed" << std::endl;
    } else {
        
        expression_tree result = parse_result.to_expression_tree();

        std::cout << '\n';
        //std::cout << "Parsed : " << result.to_fullform(*this) << '\n' << std::endl;

        result.evaluate(*this);
         
        std::cout << "Time : " << std::clock() - start << "ms\n";

        //std::cout << "Result :\n" << result.output(*this) << std::endl;


        
        //std::cout << "\nFull : " << result.to_fullform(*this);
        std::cout << "\nResult: " << result.output(*this) << std::endl;
        
        
    }

}

void enviroment::set_print_error_messages(bool b) {
    print_error_messages = b;
}

} //namespace r0