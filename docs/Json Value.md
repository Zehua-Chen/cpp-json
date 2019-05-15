# Json Value

* Json Value are implemented using std::variant to be able to switch between
object, array, string, number, boolean and null;
* Despite the size increase as a result of std::variant, cache misses are reduced as a result 
of a more compact data type;
    * Consider what happens if data are stored using pointers