#pragma once

#include <iostream>
#include <cstddef>
#include <unordered_map>
#include <queue>

template<typename K, typename V>
class FIFOCache {
	private:
		size_t max_cache_size;
		std::unordered_map<K, V> cache_map;
		std::queue<K> fifo_map;
	public:
		FIFOCache(size_t cache_size = 1024) : max_cache_size(cache_size) {
			if (cache_size < 8) {
				std::cout << "Too low cache size. Setting it to 8 elements\n";
				max_cache_size = 8;
			}
			if (cache_size > 65536) {
				std::cout << "Too large cache size. Setting it to 65536 elements\n";
				max_cache_size = 65536;
			}
		}

		~FIFOCache();

		void insert(const K& key, const V& value);
		V get(const K& key);
};
