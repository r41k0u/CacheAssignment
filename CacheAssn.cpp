#include "CacheAssn.h"

template<typename K, typename V>
void FIFOCache<K, V>::insert(const K& key, const V& value) {
	if (cache_map.size() == max_cache_size) {
		cache_map.erase(fifo_map.front());
		fifo_map.pop();
	}
	cache_map[key] = value;
	fifo_map.push(key);
}

template<typename K, typename V>
V FIFOCache<K, V>::get(const K& key) {
	if (cache_map.find(key) == cache_map.end()) {
		std::cerr << "Invalid Key requested. Returning default value\n";
		return NULL;
	}
	return cache_map[key];
}

template<typename K, typename V>
FIFOCache<K, V>::~FIFOCache() {
	while (!fifo_map.empty()) {
		fifo_map.pop();
	}
	cache_map.clear();
}

int main()
{
	// Tests for the FIFOCache class
	FIFOCache<int, int> cache(4);
	cache.insert(1, 1);
	cache.insert(2, 2);
	cache.insert(3, 3);
	cache.insert(4, 4);
	std::cout << cache.get(1) << std::endl; // 1
	std::cout << cache.get(2) << std::endl; // 2
	std::cout << cache.get(3) << std::endl; // 3
	std::cout << cache.get(4) << std::endl; // 4
	cache.insert(5, 5);
	cache.insert(6, 6);
	cache.insert(7, 7);
	cache.insert(8, 8);
	cache.insert(9, 9);
	std::cout << cache.get(1) << std::endl; // 0
	std::cout << cache.get(2) << std::endl; // 2
	return 0;
}
