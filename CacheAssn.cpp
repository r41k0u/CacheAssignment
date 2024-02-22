#include "CacheAssn.h"

template<typename K, typename V>
AbstractCache<K, V>::AbstractCache(size_t cache_size) : max_cache_size(cache_size) {
	if (cache_size < 8) {
		std::cout << "Too low cache size. Setting it to 8 elements\n";
		max_cache_size = 8;
	}
	if (cache_size > 65536) {
		std::cout << "Too large cache size. Setting it to 65536 elements\n";
		max_cache_size = 65536;
	}
}

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
	FIFOCache<int, int> cachedt;
	cachedt.insert(1, 1);
	std::cout << cachedt.get(1) << std::endl; // 1
	return 0;
}
