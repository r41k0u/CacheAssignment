﻿#pragma once

#include <iostream>
#include <cstddef>
#include <unordered_map>
#include <queue>
#include <stack>
#include <algorithm>
#include <mutex>

template<typename K, typename V>
class AbstractCache {
	protected:
		std::unordered_map<K, V> cache_map;
		size_t max_cache_size;
		mutable std::mutex safe_op;
		virtual bool use(const K& key);
	public:
		AbstractCache(size_t cache_size);
		virtual ~AbstractCache() {};

		virtual void insert(const K& key, const V& value) = 0;
		V get(const K& key);
		V operator[](const K& key) { return get(key); }
};

template<typename K, typename V>
class FIFOCache : public AbstractCache<K, V> {
	private:
		std::queue<K> fifo_map;
	public:
		FIFOCache(size_t cache_size = 1024) : AbstractCache<K, V>(cache_size) {}
		~FIFOCache();
		void insert(const K& key, const V& value);
};

template<typename K, typename V>
class LIFOCache : public AbstractCache<K, V> {
	private:
		std::stack<K> lifo_map;
	public:
		LIFOCache(size_t cache_size = 1024) : AbstractCache<K, V>(cache_size) {}
		~LIFOCache();
		void insert(const K& key, const V& value);
};

template<typename K, typename V>
class LRUCache : public AbstractCache<K, V> {
	private:
		std::list<K> lru_map;
		bool use(const K& key);
	public:
		LRUCache(size_t cache_size = 1024) : AbstractCache<K, V>(cache_size) {}
		~LRUCache();
		void insert(const K& key, const V& value);
};