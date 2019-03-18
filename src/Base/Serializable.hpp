#ifndef POPHEAD_BASE_SERIALIZABLE_H_
#define POPHEAD_BASE_SERIALIZABLE_H_

#include <iostream>

namespace PopHead {
namespace Base {

/// Interface for serializable classes.
class Serializable
{
  public:
    /// Save content to provided stream.
    virtual bool save( std::ostream& ) =0;

    /// Load content from provided stream.
    virtual bool load( std::istream& ) =0;
};

}}

#endif // !POPHEAD_BASE_SERIALIZABLE_H_
