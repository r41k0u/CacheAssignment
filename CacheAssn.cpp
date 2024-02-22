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

template<typename K, typename V>
void LIFOCache<K, V>::insert(const K& key, const V& value) {
	if (cache_map.size() == max_cache_size) {
		cache_map.erase(lifo_map.top());
		lifo_map.pop();
	}
	cache_map[key] = value;
	lifo_map.push(key);
}

template<typename K, typename V>
V LIFOCache<K, V>::get(const K& key) {
	if (cache_map.find(key) == cache_map.end()) {
		std::cerr << "Invalid Key requested. Returning default value\n";
		return NULL;
	}
	return cache_map[key];
}

template<typename K, typename V>
LIFOCache<K, V>::~LIFOCache() {
	while (!lifo_map.empty()) {
		lifo_map.pop();
	}
	cache_map.clear();
}

int main()
{
	// Tests for the LIFOCache class
	LIFOCache<int, int> cachedt(4);
	cachedt.insert(1, 1);
	cachedt.insert(2, 2);
	cachedt.insert(3, 3);
	cachedt.insert(4, 4);

	std::cout << cachedt.get(1) << std::endl; // 1
	std::cout << cachedt.get(2) << std::endl; // 2
	std::cout << cachedt.get(3) << std::endl; // 3
	std::cout << cachedt.get(4) << std::endl; // 4
	std::cout << cachedt.get(5) << std::endl; // NULL

	cachedt.insert(5, 5);
	cachedt.insert(6, 6);
	cachedt.insert(7, 7);
	cachedt.insert(8, 8);
	
	std::cout << cachedt.get(7) << std::endl; // 7
	std::cout << cachedt.get(8) << std::endl; // 8

	cachedt.insert(9, 9);

	std::cout << cachedt.get(7) << std::endl; // 7
	std::cout << cachedt.get(8) << std::endl; // NULL
	std::cout << cachedt.get(9) << std::endl; // 9

	return 0;
}
