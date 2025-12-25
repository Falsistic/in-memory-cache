# Cache Implementations: LRU & LFU with TTL (C++)

## Overview

This repository contains high-performance **in-memory cache implementations** in C++, supporting:

- **LRU (Least Recently Used) Cache**
- **LFU (Least Frequently Used) Cache**
- **TTL (Time-to-Live) based expiration**
- **O(1)** average time complexity for core operations

The project focuses on efficient data structures and realistic cache eviction strategies commonly used in backend and systems programming.

---

## Features

### LRU Cache
- Evicts the least recently accessed key when capacity is exceeded
- Implemented using:
  - Hash map for constant-time lookup
  - Doubly linked list for recency tracking
- Supports optional TTL-based expiration
- Uses lazy expiration (no background cleanup thread)

### LFU Cache
- Evicts the least frequently accessed key
- Uses LRU as a tie-breaker within the same frequency
- Maintains frequency buckets with minimum frequency tracking
- Supports optional TTL-based expiration
- Lazy expiration integrated with eviction logic

### TTL (Time-to-Live)
- Each key can have an optional TTL in milliseconds
- Expired keys are:
  - Removed on access (`get`)
  - Cleaned up during insertion (`put`)
- Implemented using a monotonic clock to avoid time drift issues

---

## Design & Data Structures

### Common Components
- `unordered_map<int, Node*>` for O(1) key access
- Custom doubly linked lists for eviction ordering
- Lazy cleanup to preserve performance guarantees

### LRU Design
- Single doubly linked list ordered by recency
- Head represents most recently used
- Tail represents least recently used

### LFU Design
- Multiple doubly linked lists grouped by access frequency
- `minfreq` tracks the lowest frequency present in the cache
- Eviction order:
  1. Expired keys
  2. Lowest frequency
  3. Least recently used within that frequency

---

## API Overview

### LRU Cache
```cpp
LRUCache cache(capacity);
cache.put(key, value);
cache.put(key, value, ttl_ms);
cache.get(key);
