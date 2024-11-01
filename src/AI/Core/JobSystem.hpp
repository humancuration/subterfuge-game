#pragma once
#include <vector>
#include <queue>
#include <functional>
#include "ThreadPool.hpp"

class JobSystem {
public:
    enum class Priority {
        HIGH,
        MEDIUM,
        LOW
    };

    struct Job {
        std::function<void()> task;
        Priority priority;
        uint64_t dependencies{0};
        
        bool operator<(const Job& other) const {
            return priority < other.priority;
        }
    };

private:
    ThreadPool& threadPool;
    std::priority_queue<Job> highPriorityJobs;
    std::queue<Job> mediumPriorityJobs;
    std::queue<Job> lowPriorityJobs;
    std::mutex jobMutex;
    
    static constexpr size_t CACHE_LINE_SIZE = 64;
    alignas(CACHE_LINE_SIZE) std::atomic<size_t> completedJobs{0};

public:
    explicit JobSystem(ThreadPool& pool) : threadPool(pool) {}

    template<typename F>
    void schedule_job(F&& task, Priority priority = Priority::MEDIUM) {
        std::lock_guard<std::mutex> lock(jobMutex);
        
        Job job{std::forward<F>(task), priority};
        
        switch (priority) {
            case Priority::HIGH:
                highPriorityJobs.push(std::move(job));
                break;
            case Priority::MEDIUM:
                mediumPriorityJobs.push(std::move(job));
                break;
            case Priority::LOW:
                lowPriorityJobs.push(std::move(job));
                break;
        }
    }

    void process_jobs() {
        std::vector<std::future<void>> futures;
        
        {
            std::lock_guard<std::mutex> lock(jobMutex);
            
            // Process high priority jobs first
            while (!highPriorityJobs.empty()) {
                auto job = std::move(const_cast<Job&>(highPriorityJobs.top()));
                highPriorityJobs.pop();
                futures.push_back(threadPool.enqueue(std::move(job.task)));
            }
            
            // Then medium priority
            while (!mediumPriorityJobs.empty()) {
                auto job = std::move(mediumPriorityJobs.front());
                mediumPriorityJobs.pop();
                futures.push_back(threadPool.enqueue(std::move(job.task)));
            }
            
            // Finally low priority
            while (!lowPriorityJobs.empty()) {
                auto job = std::move(lowPriorityJobs.front());
                lowPriorityJobs.pop();
                futures.push_back(threadPool.enqueue(std::move(job.task)));
            }
        }
        
        for (auto& future : futures) {
            future.wait();
            ++completedJobs;
        }
    }
}; 