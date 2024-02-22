# Cache Assignment
Implements FIFO, LIFO and LRU cache in C++
### Class Structure
- **AbstractCache class**
    - Pure abstract class
    - Pure virtual methods for insertion, erasure and updation of Cache
    - Implements read, but calls the updation method before completing the read, in case the eviction policy needs to keep track of reads
- **CacheKeyList class**
    - Base class for caches whose eviction policy is using an `std::list` data structure
    - Why `std::list`? Caches are small in size, there is a hard limit of 65536 elements in my cache library, so having this O(n) lookup time won't be a big problem. If it will be, we will optimize after profiling. But the usage of this single data structure allowed us to implement all 3 eviction policies
    - In the future, if you need to implement some other eviction policy which makes use of some other data structure, you can simply make another class inherited from AbstractCache like this
- **FIFOCache, LIFOCache and LRUCache**
    - These classes implement the pure virtual functions
    - Code duplicacy has been reduced by introducing CacheKeyList as it abstracts away the `std::list`
    - Duplicacy of `insert` could have been removed further, a couple of lines are being repeated in each function there. But it seemed to granular.
### How to add more eviction policies
Just inherit from AbstractCache class and implement the pure virtual left unimplemented there. You can also add more order, like adding an intermediate class which can handle your eviction policy's data structure. 
### How to try this code
You can use the CMake build system to build and run this code.
- Make a build directory and cd into it
- Run `cmake ..` and `make`
- Run the executable

Mutex locks have been used in all public functions to ensure thread safety.

### PS
I had to use a lot of `using` statements in the header due to [this](https://stackoverflow.com/questions/4643074/why-do-i-have-to-access-template-base-class-members-through-the-this-pointer). I wrote it on MSVC, where this rule is ignored and `using` statements are not needed. Templated base classes are treated like ordinary base classes. But to make it work on UNIX, I had to either add the `this` keyword everywhere, prefix the attributes with `Base_Class<K, V>` everywhere, write a preprocessor macro or use the using keyword.