#include "rfl/boost_serialization/Writer.hpp"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>

namespace rfl::boost_serialization {

template class Writer<boost::archive::binary_oarchive>;
template class Writer<boost::archive::text_oarchive>;

}  // namespace rfl::boost_serialization
