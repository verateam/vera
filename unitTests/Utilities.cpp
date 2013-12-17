#include "Utilities.h"

using namespace Vera::Structures;

namespace Testing
{

Vera::Structures::Tokens::TokenSequence
Utilities::getSubCollection(unsigned offset, unsigned size, const Vera::Structures::Tokens::TokenSequence& collection)
{
  Tokens::TokenSequence response;

  if (collection.size()<offset+size)
    throw std::runtime_error("Error out of range.");

  Tokens::TokenSequence::const_iterator it = collection.begin() + offset;
  Tokens::TokenSequence::const_iterator end = collection.begin() + (offset+size);
  std::copy(it, end ,std::back_inserter<Vera::Structures::Tokens::TokenSequence>(response));
  return response;
}

} // Testing namespace
