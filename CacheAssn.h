#pragma once

#include <iostream>
#include <cstddef>
#include <unordered_map>
#include <queue>
#include <stack>

template<typename K, typename V>
class AbstractCache {
	protected:
		std::unordered_map<K, V> cache_map;
		size_t max_cache_size;
	public:
		AbstractCache(size_t cache_size);
		virtual void insert(const K& key, const V& value) = 0;
		virtual V get(const K& key) = 0;
		virtual ~AbstractCache() {}
};

template<typename K, typename V>
class FIFOCache : public AbstractCache<K, V> {
	private:
		std::queue<K> fifo_map;
	public:
		FIFOCache(size_t cache_size = 1024) : AbstractCache<K, V>(cache_size) {}
		~FIFOCache();
		void insert(const K& key, const V& value);
		V get(const K& key);
};

template<typename K, typename V>
class LIFOCache : public AbstractCache<K, V> {
	private:
		std::stack<K> lifo_map;
	public:
		LIFOCache(size_t cache_size = 1024) : AbstractCache<K, V>(cache_size) {}
		~LIFOCache();
		void insert(const K& key, const V& value);
		V get(const K& key);
};
