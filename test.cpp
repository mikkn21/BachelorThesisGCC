#include <algorithm>
#include <boost/spirit/home/x3.hpp> 
#include <iostream>
#include <ostream>
#include <unistd.h>


namespace client 
{
  namespace x3 = boost::spirit::x3; 
  namespace ascii = boost::spirit::x3::ascii; 


  // Number list parser 
  
  template <typename Iterator> 
  bool parse_numbers(Iterator first, Iterator last) 
  {
    using x3::double_; 
    using x3::phrase_parse; 
    using ascii::space; 
    
    bool r = phrase_parse(

      first,                            // Start Iterator  
      last,                            // End Iterator  
      double_ >> *(',' >> double_),   // The Parser 
      space                          // The Skip-Parser 
    ); 
  
    if (first != last) // fail if we did not get a full match 
      return false; 

    return r; 
  }
}

// The main program 
int main () {
  
  std::cout << "/////////////////////////////////////////////////////////\n\n";
  std::cout << "\t\tA comma separated list parser for Spirit...\n\n";
  std::cout << "/////////////////////////////////////////////////////////\n\n";

  std::cout << "Give me a comma seperated list of numbers.\n";
  std::cout << "Type [q or Q] to quit\n";  

  std::string str;
  while (getline(std::cin, str)) 
  {
    if (str.empty() || str[0] == 'q' || str[0] == 'Q')
      break; 

    if (client::parse_numbers(str.begin(), str.end()))
    {
      std::cout << "-------------------------\n";
      std::cout << "Parsing succeeded\n"; 
      std::cout << str << " Parse OK: " << std::endl;
    }

  }


  std::cout << "Bye... :-) \n\n";
  
  return 0;
}
