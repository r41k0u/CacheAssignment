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
bool AbstractCache<K, V>::use(const K& key) {
	return cache_map.find(key) != cache_map.end();
}

template<typename K, typename V>
V AbstractCache<K, V>::get(const K& key) {
	if (!use(key)) {
		std::cerr << "Invalid Key requested. Returning default value\n";
		return NULL;
	}
	return cache_map[key];
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
FIFOCache<K, V>::~FIFOCache() {
	while (!fifo_map.empty())
		fifo_map.pop();
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
LIFOCache<K, V>::~LIFOCache() {
	while (!lifo_map.empty())
		lifo_map.pop();
	cache_map.clear();
}

template<typename K, typename V>
void LRUCache<K, V>::insert(const K& key, const V& value) {
	if (cache_map.size() == max_cache_size) {
		cache_map.erase(lru_map.front());
		lru_map.pop_front();
	}
	cache_map[key] = value;
	lru_map.push_back(key);
}

template<typename K, typename V>
bool LRUCache<K, V>::use(const K& key) {
	auto it = std::find(lru_map.begin(), lru_map.end(), key);
	if (it == lru_map.end())
		return false;

	lru_map.remove(key);
	lru_map.push_back(key);
	return true;
}

template<typename K, typename V>
LRUCache<K, V>::~LRUCache() {
	while (!lru_map.empty())
		lru_map.pop_front();
	cache_map.clear();
}

int main()
{
	// Tests for the LRUCache class
	LRUCache<int, int> cachedt(4);
	cachedt.insert(1, 1);
	cachedt.insert(2, 2);
	cachedt.insert(3, 3);
	cachedt.insert(4, 4);

	std::cout << cachedt[1] << std::endl; // 1
	std::cout << cachedt[2] << std::endl; // 2
	std::cout << cachedt[3] << std::endl; // 3
	std::cout << cachedt[4] << std::endl; // 4
	std::cout << cachedt[5] << std::endl; // NULL

	cachedt.insert(5, 5);
	cachedt.insert(6, 6);
	cachedt.insert(7, 7);
	cachedt.insert(8, 8);
	
	std::cout << cachedt[7] << std::endl; // 7
	std::cout << cachedt[8] << std::endl; // 8
	std::cout << cachedt[1] << std::endl; // 1

	cachedt.insert(9, 9);

	std::cout << cachedt[1] << std::endl; // 1
	std::cout << cachedt[2] << std::endl; // NULL
	std::cout << cachedt[9] << std::endl; // 9

	return 0;
}
