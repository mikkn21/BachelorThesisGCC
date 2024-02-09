#include "sum_def.hpp"
#include "../config.hpp"

namespace grammar
{
    namespace parser
    {
        BOOST_SPIRIT_INSTANTIATE(sum_type, iterator_type, context_type);    
    } // namepace parser
} // namespace grammar


