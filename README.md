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