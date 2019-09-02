# Formatting principles
-----------------------

### example.hpp
```cpp
#ifndef POPHEAD_DIRECTORY_EXAMPLE_H_
#define POPHEAD_DIRECTORY_EXAMPLE_H_

#include "Module/something.hpp"
#include "fileInTheSameDirectory.hpp"

#include <vector>
#include <memory>

namespace ph {

class Example
{
public:
    Example(const std::string& name);

    void foo();
    auto foo2(std::vector<std::string>& names) -> std::vector<std::unique_ptr<Someting>>;
    
private:
    char helperMethod() const;

private:
    std::string mName;
    int mVariable;
}

}

#endif //!POPHEAD_DIRECTORY_CLASS_H_
```

### example.cpp
```cpp
#include "example.hpp"
#include "Utilities/debug.hpp"

#include <iostream>

ph::Example::Example(const std::string& name)
    :mName(name)
    ,mVariable1(38)
{
}

void ph::Example::foo()
{
    if(mName == "Roman") {
        // more then one line of code
    }
    else if(mName == "Pablito") {
        // more then one line of code
    } 
    else {
        // more then one line of code
    }

    if(mVariable == 1)
        mName = "Slender";
    else if(mVariable == 2)
        mName = "Maximilian";
    else
        mName = "Raymond";
}

auto ph::Example::foo2(std::vector<std::string>& names) -> std::vector<std::unique_ptr<Someting>>
{
    for(auto& name : names)
        name = mName == "Roman" ? helperMethod() : "Twoja stara";

    for(auto& name : names) {
        name += "ski";
        PH_LOG(LogType::Info, "Added ski ending.");
    }
}
 
char ph::Example::helperMethod() const
{
    switch(mVariable)
    {
    case 1: return 'a'; break;
    case 2:
        int rocket = 666;
        throw rocket;
        break; 
    default: return '*'; break;
    }
}
```
