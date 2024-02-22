#pragma once

#include <iostream>
#include <cstddef>
#include <unordered_map>
#include <algorithm>
#include <mutex>
#include <vector>
#include <list

// AbstractCache class
//
// Pure virtual function: use
// this is called at the start of every get operation
// 
// Pure virtual functions: insert, erase
// these are called to insert and erase elements from the cache 

template<typename K, typename V>
class AbstractCache {
	protected:
		std::unordered_map<K, V> cache_map;
		size_t max_cache_size;
		mutable std::mutex safe_op;
		virtual bool use(const K& key) = 0;
	public:
		AbstractCache(size_t cache_size);
		~AbstractCache() { cache_map.clear(); }

		virtual void insert(const K& key, const V& value) = 0;
		virtual void erase(const K& key) = 0;
		virtual void policy() = 0;
		V get(const K& key);
		V operator[](const K& key) { return get(key); }
		size_t size() { return cache_map.size(); }
		size_t max_size() { return max_cache_size; }
};

// CacheKeyList class
//
// This class is a parent class for eviction policies which will use
// the std::list data structure. This is a decent choice as we are not dealing
// with a large amount of data and linear lookup and update time is good enough

template<typename K, typename V>
class CacheKeyList : public AbstractCache<K, V> {
	protected:
		std::list<K> key_map;
		virtual bool use(const K& key);
	public:
		CacheKeyList(size_t cache_size) : AbstractCache<K, V>(cache_size) {}
		~CacheKeyList() { key_map.clear(); }
		virtual void insert(const K& key, const V& value) = 0;
		virtual void policy() = 0;
		void erase(const K& key);
};

// FIFOCache class
//
// Implements the First In First Out eviction policy

template<typename K, typename V>
class FIFOCache : public CacheKeyList<K, V> {
	public:
		FIFOCache(size_t cache_size = 1024) : CacheKeyList<K, V>(cache_size) {}
		void insert(const K& key, const V& value);
		void policy() { std::cout << "FIFO\n";}
};

// LIFOCache class
//
// Implements the Last In First Out eviction policy

template<typename K, typename V>
class LIFOCache : public CacheKeyList<K, V> {
	public:
		LIFOCache(size_t cache_size = 1024) : CacheKeyList<K, V>(cache_size) {}
		void insert(const K& key, const V& value);
		void policy() { std::cout << "LIFO\n";}
};

// LRUCache class
//
// Implements the Least Recently Used eviction policy

template<typename K, typename V>
class LRUCache : public CacheKeyList<K, V> {
	private:
		bool use(const K& key);
	public:
		LRUCache(size_t cache_size = 1024) : CacheKeyList<K, V>(cache_size) {}
		void insert(const K& key, const V& value);
		void policy() { std::cout << "LRU\n";}
};