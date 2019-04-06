#ifndef POPHEAD_RESOURCE_RESOURCE_H_
#define POPHEAD_RESOURCE_RESOURCE_H_

#include <memory>

namespace PopHead {
namespace Resources {

template< typename ResourceType >
class Resource
{
  public:
    static auto createResource( const std::string& path )
    -> std::unique_ptr< ResourceType >;

  private:
    virtual bool loadFromFile( std::string path ) =0;
};

template< typename ResourceType >
auto Resource<ResourceType>::createResource( const std::string& path )
-> std::unique_ptr< ResourceType >
{
    auto r = std::make_unique< ResourceType >();
    if( r->loadFromFile( path ) ) return r;
    return std::unique_ptr< ResourceType >( nullptr );
}

}}

#endif // !POPHEAD_RESOURCE_RESOURCE_H_
