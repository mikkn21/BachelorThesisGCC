#include <iostream>

#include "grammar/ast.hpp"
#include "grammar/sum.hpp"

using namespace std;

int main()
{
    cout << "/////////////////////////////////////////////////////////\n\n";
    cout << "\t\t A summ parser for Spirit...\n\n";
    cout << "/////////////////////////////////////////////////////////\n\n";

    cout << "Give me a sum: \n";
    cout << "Type [q or Q] to quit\n\n";

    using boost::spirit::x3::ascii::space;
    using iterator_type = std::string::const_iterator;
    using grammar::sum;

    string str;
    while (getline(cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        grammar::sum_ast::sum mSum;
        iterator_type iter = str.begin();
        iterator_type const end = str.end();
        bool r = phrase_parse(iter, end, sum(), space, mSum);
        
        if (r && iter == end)
        {
            cout << boost::fusion::tuple_open('[');
            cout << boost::fusion::tuple_close(']');
            cout << boost::fusion::tuple_delimiter(", ");

            cout << "-------------------------\n";
            cout << "Parsing succeeded\n";
            cout << "got: " << mSum << endl;
            cout << "\n-------------------------\n";
        }
        else
        {
            cout << "-------------------------\n";
            cout << "Parsing failed\n";
            cout << "-------------------------\n";
        }
    }

    cout << "Bye... :-) \n\n";
    return 0;
}
