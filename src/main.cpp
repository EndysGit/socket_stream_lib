#include <iostream>
#include <ostream>
#include <ios>

class socket
{
};

/* stream prefixes:
 *  - i = input
 *  - o = output
 *  - s = socket
 */

template <typename charT, typename traits, typename socketT>
class basic_isstreami : virtual public std::basic_ios<charT, traits>
{
};

template <typename charT, typename traits, typename socketT>
class basic_osstream : virtual public std::basic_ios<charT, traits> 
{

};

class socket_base
{
public:
// Flags:
// ------
//
// State:
// ------
//
//
};

int
main()
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
