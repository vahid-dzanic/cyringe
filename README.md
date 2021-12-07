<b>cyringe - Dependency injection framework for C++</b>
===
cyringe pronunciation /sɪˈrɪndʒ/

---
<!-- TOC depthfrom:2 orderedlist:false updateonsave:true -->

- [Description](#description)
    - [Advantage](#advantage)
    - [Disadvantage](#disadvantage)
- [Improve legacy code](#improve-legacy-code)
- [How to clone, build, test and install](#how-to-clone-build-test-and-install)
- [How to declare as CMake link-dependency](#how-to-declare-as-cmake-link-dependency)
- [How to use](#how-to-use)
    - [Setup an instance](#setup-an-instance)
    - [Release the instance before deleting the object](#release-the-instance-before-deleting-the-object)
    - [Get and use the instance](#get-and-use-the-instance)
    - [Setup and release within the instance-class itself](#setup-and-release-within-the-instance-class-itself)
- [Requires](#requires)
- [License](#license)

<!-- /TOC -->

## Description
cyringe is an easy to use dependency injection framework for C++. It allows to distribute/provide objects within the whole application-process borders.
The framework can provide single-instance of one type or multiple-instances of a type
* In case of a single-instance, which is singleton-compatible, is no need to define the ID-Type and
  also no need to set the argument id when calling the cyringe-methods.
* In case of a multi-instance, a proper ID-Type has to be defined (must be an integral-type or enum-type)
  and an unique ID per object has to be passed with every method call.

### Advantage
Availability or globally available instances, similar to a Singleton pattern but WITHOUT of it's drawbacks for testing. In fact it makes testing even easier.

### Disadvantage
Globally availability can be misused to shortcut some architectural rules. Good news; you can search for usage and define rules in your merge/pull-request process.

## Improve legacy code
Within legacy code often Singletons are used all over the place. One good step forward is to get rid of the singletons by simply replacing them with cyringe.

## How to clone, build, test and install
Clone the project from here: https://github.com/vahid-dzanic/cyringe

```bash
# clone
git clone https://github.com/vahid-dzanic/cyringe.git

# build
mkdir -p cyringe/build
cd cyringe/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# test
ctest -j$(nproc)

# install
sudo make install
```

## How to declare as CMake link-dependency
In case your build system is CMake, you can setup the cyringe Header-only-library as link-dependency.

```
...

find_package(cyringe REQUIRED)

add_executable(my_app
  ...)

target_link_libraries(my_app
  PRIVATE
    vdc::cyringe
    ...
    ... )
```

## How to use
### Setup an instance
```cpp
#include <cyringe/cyringe.h>

class XYZ {
  float origin();
}

int main() {
  XYZ xyz;

  cyringe<XYZ>::setup(xyz);

  ...
}
```

### Release the instance before deleting the object
```cpp
#include <cyringe/cyringe.h>

int main() {
  XYZ* xyz = new XYZ();

  cyringe<XYZ>::setup(*xyz);

  ...

  cyringe<XYZ>::release();
  delete xyz;
  xyz = nullptr;
}
```

### Get and use the instance
```cpp
#include "XYZ.h"
#include <cyringe/cyringe.h>

float someCodeSomewhere() {
  auto origin = cyringe<XYZ>::get().origin()
  
  do something with origin...
}
```

### Setup and release within the instance-class itself
```cpp
#include <cyringe/cyringe.h>

class XYZ {
  XYZ() {
    cyringe<XYZ>::setup(*this);
  }
  ~XYZ() {
    cyringe<XYZ>::release();
  }
  float origin();
}
```

## Requires
C++11 capable compiler

## License
Mozilla Public License Version 2.0
