In the README file, I want to talk about the task that needed to be completed, my solution is in UnorderedSet.h.

Condition

Implement the UnorderedSet template class, an analogue of std::unordered_set, which is based on hashing by the chain method with a dynamic increase in the number of buckets.
The chain method involves storing an array, in each cell of which there is a basket - a list of inserted elements with the same hash values. The average time of operations on such a structure is proportional to the degree of load on the table (load_factor = n_elements / n_buckets). When load_factor exceeds a certain value (in the task = 1), hashing occurs - creating a new array of buckets, for example, 2 times larger and transferring old items to new buckets.
In the simplest case, it is enough to store a vector (std::vector) of lists (std::list / std::forward_list) and perform operations on it. More effective techniques are described below in additional tasks.

The basic part

The UnorderedSet template class must be parameterized by the KeyT key type.
The expansion strategy is the same as in the String task - when adding an element to an empty table, the number of buckets becomes 1, when adding an element to the full table, the number of buckets increases by 2 times (the table is considered complete if the number of buckets matches the number of elements, that is, load_factor == 1).
Use std::hash as a hash function. std::hash maps objects to the size_t range. To get the bucket index, take the remainder of dividing the resulting number by the number of buckets.

The set of methods includes:
The default constructor. Creates an empty hash table.
The constructor depends on the number of buckets count. Creates a hash table with count empty buckets.
The constructor depends on the interval specified by two Forward iterators. Creates a hash table with the number of buckets equal to the number of items in the sequence, and then inserts the items into the table.
The copy, move, and assignment constructors must work correctly.
The methods Size, Empty, Clear with the usual semantics.
Insert methods Insert(const KeyT&), Insert(KeyT&&).
The removal method Erase(const KeyT&).
The search method is bool Find(const KeyT&).
Method Rehash(new_bucket_count). Changes the number of buckets in the hash table with hashing. If new_bucket_count matches the current number of buckets or is less than the number of items (load_factor becomes greater than 1), then nothing needs to be done.
The Reserve(new_bucket_count) method. The same as Rehash, but does not reduce the number of buckets, that is, it is triggered if new_bucket_count exceeds the current number of buckets.
Methods bucketCount (returns the number of buckets in the table), BucketSize(id) (returns the size of the bucket with the id number), Bucket(key) (returns the number of the bucket into which the key object falls), loadFactor() (returns the degree of occupancy of the table).

Note that it may be inefficient to create new list nodes each time when rehashing. To reuse old nodes (throwing pointers to old lists), consider using the std::list::splice method.
