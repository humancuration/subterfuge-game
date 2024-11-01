#pragma once
#include <vector>
#include <mutex>
#include <memory>

template<typename T>
class ObjectPool {
private:
    struct Block {
        std::vector<T> objects;
        std::vector<bool> used;
        
        Block(size_t size) : objects(size), used(size, false) {}
    };

    std::vector<std::unique_ptr<Block>> blocks;
    size_t block_size;
    std::mutex mutex;

public:
    explicit ObjectPool(size_t initial_block_size = 1024) 
        : block_size(initial_block_size) {
        blocks.push_back(std::make_unique<Block>(block_size));
    }

    template<typename... Args>
    T* allocate(Args&&... args) {
        std::lock_guard<std::mutex> lock(mutex);
        
        for (auto& block : blocks) {
            for (size_t i = 0; i < block->used.size(); ++i) {
                if (!block->used[i]) {
                    block->used[i] = true;
                    T* obj = &block->objects[i];
                    new (obj) T(std::forward<Args>(args)...);
                    return obj;
                }
            }
        }

        // No free objects, create new block
        blocks.push_back(std::make_unique<Block>(block_size));
        blocks.back()->used[0] = true;
        T* obj = &blocks.back()->objects[0];
        new (obj) T(std::forward<Args>(args)...);
        return obj;
    }

    void deallocate(T* ptr) {
        std::lock_guard<std::mutex> lock(mutex);
        
        for (auto& block : blocks) {
            if (ptr >= &block->objects[0] && 
                ptr < &block->objects[0] + block->objects.size()) {
                size_t index = ptr - &block->objects[0];
                ptr->~T();
                block->used[index] = false;
                return;
            }
        }
    }
}; 