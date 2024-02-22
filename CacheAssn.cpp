#pragma once
#include "CacheAssn.h"

// AbstractCache class constructor
// Min element count is 8
// Max element count is 65536
// Cache is supposed to be small in size
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
V AbstractCache<K, V>::get(const K& key) {
	std::lock_guard<std::mutex> lock{safe_op};
	if (!use(key)) {
		std::cerr << "Invalid Key requested. Returning default value\n";
		return NULL;
	}
	return cache_map[key];
}

// use function for CacheKeyList class
// This function is used to check if a key is present in the cache
template<typename K, typename V>
bool CacheKeyList<K, V>::use(const K& key) {
	return std::find(key_map.begin(), key_map.end(), key) != key_map.end();
}

template<typename K, typename V>
void CacheKeyList<K, V>::erase(const K& key) {
	std::lock_guard<std::mutex> lock{AbstractCache<K, V>::safe_op};
	if (std::find(key_map.begin(), key_map.end(), key) != key_map.end()) {
		key_map.remove(key);
		AbstractCache<K, V>::cache_map.erase(key);
	}
}

template<typename K, typename V>
void FIFOCache<K, V>::insert(const K& key, const V& value) {
	std::lock_guard<std::mutex> lock{AbstractCache<K, V>::safe_op};
	
	if (CacheKeyList<K, V>::use(key)) {
		CacheKeyList<K, V>::key_map.remove(key);
		CacheKeyList<K, V>::key_map.push_back(key);
		AbstractCache<K, V>::cache_map[key] = value;
		return;
	}

	if (AbstractCache<K, V>::cache_map.size() == AbstractCache<K, V>::max_cache_size) {
		AbstractCache<K, V>::cache_map.erase(CacheKeyList<K, V>::key_map.front());
		CacheKeyList<K, V>::key_map.pop_front();
	}
	AbstractCache<K, V>::cache_map[key] = value;
	CacheKeyList<K, V>::key_map.push_back(key);
}

template<typename K, typename V>
void LIFOCache<K, V>::insert(const K& key, const V& value) {
	std::lock_guard<std::mutex> lock{AbstractCache<K, V>::safe_op};
	if (CacheKeyList<K, V>::use(key)) {
		CacheKeyList<K, V>::key_map.remove(key);
		CacheKeyList<K, V>::key_map.push_back(key);
		AbstractCache<K, V>::cache_map[key] = value;
		return;
	}

	if (AbstractCache<K, V>::cache_map.size() == AbstractCache<K, V>::max_cache_size) {
		AbstractCache<K, V>::cache_map.erase(CacheKeyList<K, V>::key_map.back());
		CacheKeyList<K, V>::key_map.pop_back();
	}
	AbstractCache<K, V>::cache_map[key] = value;
	CacheKeyList<K, V>::key_map.push_back(key);
}

template<typename K, typename V>
void LRUCache<K, V>::insert(const K& key, const V& value) {
	std::lock_guard<std::mutex> lock{AbstractCache<K, V>::safe_op};
	if (use(key)) {
		AbstractCache<K, V>::cache_map[key] = value;
		return;
	}
	if (AbstractCache<K, V>::cache_map.size() == AbstractCache<K, V>::max_cache_size) {
		AbstractCache<K, V>::cache_map.erase(CacheKeyList<K, V>::key_map.front());
		CacheKeyList<K, V>::key_map.pop_front();
	}
	AbstractCache<K, V>::cache_map[key] = value;
	CacheKeyList<K, V>::key_map.push_back(key);
}

template<typename K, typename V>
bool LRUCache<K, V>::use(const K& key) {
	auto it = std::find(CacheKeyList<K, V>::key_map.begin(), CacheKeyList<K, V>::key_map.end(), key);
	if (it == CacheKeyList<K, V>::key_map.end())
		return false;

	CacheKeyList<K, V>::key_map.remove(key);
	CacheKeyList<K, V>::key_map.push_back(key);
	return true;
}

template<typename K, typename V>
void test_cache(AbstractCache<K, V>* cache) {
	std::cout << "Cache Sanity Check:\n";
	cache->policy();

	size_t max_size = cache->max_size();
	for (int i = 0; i < cache->max_size(); i++) {
		cache->insert(i, i);
	}

	std::cout << "Data filled. Access 2 indices\n";
	// Access the first 2 elements (sanity check for LRU)
	std::cout << cache->get(0) << "\n";
	std::cout << cache->get(1) << "\n";

	std::cout << "Adding 2 more entries\n";
	cache->insert(max_size, max_size);
	cache->insert(max_size + 1, max_size + 1);

	std::cout << "Trying to access all the data written to cache\n";
	for (int i = 0; i < max_size + 2; i++) {
		std::cout << cache->get(i) << "\n";
	}
	std::cout << "\n\n";
}

int main() {
	std::vector<AbstractCache<int, int>*> cache;
	cache.push_back(new FIFOCache<int, int>(8));
	cache.push_back(new LIFOCache<int, int>(8));
	cache.push_back(new LRUCache<int, int>(8));

	test_cache(cache[0]);
	test_cache(cache[1]);
	test_cache(cache[2]);

	return 0;
}