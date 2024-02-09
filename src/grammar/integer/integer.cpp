#include "integer_def.hpp"
#include "../../config.hpp"

namespace grammar 
{
    namespace parser
    {
        BOOST_SPIRIT_INSTANTIATE(integer_type, iterator_type, context_type);
    } // namespace parser
} // namespace grammar 