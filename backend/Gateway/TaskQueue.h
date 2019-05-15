#ifndef TASKQUEUE_H
#define TASKQUEUE_H
#include <Wt/Http/Message>
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

namespace Services {
enum Service {
    News = 0,
    Comments,
    Users,
    LikeHistory,

    ServiceCount
};
}

class TaskQueue {
    using Task = std::function<const Wt::Http::Message()>;
    struct TaskEntity {
        TaskEntity(Task task)
            : func(task)
            , counter(50)
        {
        }
        std::function<const Wt::Http::Message()> func;
        uint32_t counter;
    };

public:
    static TaskQueue&
    Instance()
    {
        static TaskQueue s;
        return s;
    }
    void addTask(Task task)
    {
        std::lock_guard<std::mutex> guard(tasksMutex);
        std::cout << "ADDED TASK" << std::endl;
        tasks.push_back(task);
    }

private:
    void runTask()
    {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(20));
            std::lock_guard<std::mutex> guard(tasksMutex);
            for (auto it = tasks.begin(); it != tasks.end();) {
                std::cout << " TASK RUN size " << tasks.size() << std::endl;

                auto result = it->func();
                std::cout << "RESULT STATUS " << result.status() << std::endl;
                it->counter--;
                if (result.status() == 200 or it->counter == 0)
                    it = tasks.erase(it);
                else
                    ++it;
            }
        }
    }
    TaskQueue()
    {
        std::thread t(&TaskQueue::runTask, this);
        t.detach();
    } // конструктор недоступен
    ~TaskQueue() {}

    TaskQueue(TaskQueue const&);
    TaskQueue& operator=(TaskQueue const&);
    std::vector<TaskEntity> tasks;
    std::mutex tasksMutex;
};

#endif //TASKQUEUE_H
