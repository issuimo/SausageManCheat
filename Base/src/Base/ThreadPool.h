#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <shared_mutex>
#include <future>
#include <memory>
#include <deque>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <cstring>
#include <chrono>
#include <format>
#include <exception>

namespace tp {

    struct SyncStream {
        SyncStream(std::ostream& _out = std::cout) : out(_out) {}

        template<typename... Args>
        void print(Args&&... args) {
            std::lock_guard<std::mutex> lk(stream_mtx);
            (out << ... << std::forward<Args>(args));
        }

        template<typename... Args>
        void println(Args&&... args) {
            print(std::forward<Args>(args)..., '\n');
        }
    private:
        std::ostream& out;
        static inline std::mutex stream_mtx = {};
    };

    struct TimeGuard {
        using time_point = std::chrono::time_point<std::chrono::steady_clock>;
        using ms = std::chrono::microseconds;

        TimeGuard() : start(std::chrono::steady_clock::now()) {

        }

        void update_start() {
            start = std::chrono::steady_clock::now();
        }

        double get_duration() const {
            time_point end = std::chrono::steady_clock::now();
            auto       duration = duration_cast<ms>(end - start).count();
            return static_cast<double>(duration) / ms::period::den;
        }

        void print_duration() const {
            SyncStream().println(get_duration(), "s");
        }
    private:
        time_point start;
    };

    enum class ThreadPoolState {
        CLOSED,
        RUNNING,
        WAITING,
        PAUSED
    };

    template<typename T>
    struct Block {
        static_assert(std::is_integral_v<T>, "T must be integral.");
        Block(const T s, const T e, size_t nb) : start(s), end(e), num_blocks(nb) {
            total_size = static_cast<size_t>(end - start);
            block_size = total_size / num_blocks;
            if (block_size == 0) {
                block_size = 1;
                num_blocks = block_size;
            }
        }

        [[nodiscard]] T get_block_start(const size_t i) const {
            if (i >= num_blocks) throw std::runtime_error("Block index out of range.");
            return static_cast<T>(i * block_size) + start;
        }

        [[nodiscard]] T get_block_end(const size_t i) const {
            if (i >= num_blocks) throw std::runtime_error("Block index out of range.");
            return (i == num_blocks - 1) ? end : static_cast<T>((i + 1) * block_size) + start;
        }

        [[nodiscard]] size_t get_num_blocks() const noexcept {
            return num_blocks;
        }

        Block(const Block&) = default;
        Block& operator=(const Block&) = default;
    private:
        T start, end;
        size_t block_size = 0;
        size_t num_blocks = 0;
        size_t total_size = 0;
    };

    class ThreadPool {
    public:
        ThreadPool(size_t count = std::thread::hardware_concurrency(), bool destroy_idle = false) : threads_count(count), destroy_idle(destroy_idle) {
            threads.resize(THREADS_MAX + 1);
            create_pool(count);
            manager = std::thread(&ThreadPool::manager_call, this);
        }

        ~ThreadPool() {
            wait_until_done();
            destroy_pool();
        }

        template<typename F, typename... Args>
        decltype(auto) push(F&& f, Args&&... args) {
            // When the pool has been stopped, adding tasks is not allowed.
            if (is_closed())
                throw std::runtime_error("Error: Adding tasks on a closed thread-pool.");

            using return_type = std::invoke_result_t<F, Args...>;
            auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
            auto res = task->get_future();
            {
                std::lock_guard lk(tasks_mtx);
                tasks.push_back([task] { (*task)(); });
                ++tasks_count;
            }
            task_available_cv.notify_one();
            return res;
        }

        template<typename F, typename T>
        decltype(auto) push_loop(F&& f, const T start, const T end, const size_t num_blocks = 0) {
            static_assert(std::is_integral_v<T>, "Error: Loop ranges is non-integral.");
            if (start >= end)
                throw std::runtime_error(std::format("Error: Improper loop range from {} to {}.", start, end).c_str());
            Block bk(start, end, num_blocks ? num_blocks : std::thread::hardware_concurrency());
            for (size_t i = 0; i < bk.get_num_blocks(); ++i) {
                push(std::forward<F>(f), bk.get_block_start(i), bk.get_block_end(i));
            }
        }

        [[nodiscard]] size_t get_threads_count() const noexcept {
            return threads_count;
        }

        [[nodiscard]] size_t get_threads_running() const noexcept {
            return threads_running;
        }

        [[nodiscard]] size_t get_tasks_count() const noexcept {
            return tasks_count;
        }

        [[nodiscard]] size_t get_tasks_total() const noexcept {
            return tasks_count + threads_running;
        }

        void pause() {
            if (is_paused()) return;
            if (!is_running())
                throw std::runtime_error("Error: Pause a not-running thread-pool.");
            state = ThreadPoolState::PAUSED;
        }

        void resume() {
            if (is_running()) return;
            if (!is_paused())
                throw std::runtime_error("Error: Resume a not-paused thread-pool.");
            state = ThreadPoolState::RUNNING;
            // Awake paused threads.
            task_available_cv.notify_all();
        }

        void clear() {
            std::lock_guard<std::mutex> lk(tasks_mtx);
            tasks.clear();
            tasks_count = 0;
        }

        /**
         * @brief Wait for all tasks in queue to be completed, especially those that are being executed and still in queue.
         */
        void wait_until_done() {
            if (is_closed() || is_paused()) return;
            std::unique_lock<std::mutex> lk(tasks_mtx);
            state = ThreadPoolState::WAITING;
            pool_done_cv.wait(lk, [this] { return !threads_running && is_empty(); });
        }

        template<typename _Rep, typename _Period>
        bool wait_for(const std::chrono::duration<_Rep, _Period>& __rtime) {
            if (is_closed() || is_paused()) return true;
            std::unique_lock<std::mutex> lk(tasks_mtx);
            state = ThreadPoolState::WAITING;
            bool res = pool_done_cv.wait_for(lk, __rtime, [this] {
                return !threads_running && is_empty();
                });
            return res;
        }

        bool is_running() const noexcept {
            return state == ThreadPoolState::RUNNING;
        }

        bool is_closed() const noexcept {
            return state == ThreadPoolState::CLOSED;
        }

        bool is_waiting() const noexcept {
            return state == ThreadPoolState::WAITING;
        }

        bool is_paused() const noexcept {
            return state == ThreadPoolState::PAUSED;
        }

        bool is_empty() const noexcept {
            return tasks_count == 0;
        }
    private:
        /**
         * @brief Create a pool object and assign a worker to each thread.
         *
         * @param count The count of threads in thread-pool currently.
         */
        void create_pool(size_t count) {
            state = ThreadPoolState::RUNNING;
            for (size_t i = 0; i < count; ++i) {
                threads[i] = std::thread(&ThreadPool::worker_call, this);
            }
        }

        /**
         * @brief Destroy the all threads in pool.
         */
        void destroy_pool() {
            state = ThreadPoolState::CLOSED;
            task_available_cv.notify_all();
            if (manager.joinable()) manager.join();
            for (size_t i = 0; i <= THREADS_MAX; ++i) {
                if (threads[i].joinable()) threads[i].join();
            }
        }

        /**
         * @brief The call funtion of woker threads.
         */
        void worker_call() {
            std::function<void()> task;
            while (true) {
                std::unique_lock<std::mutex> lk(tasks_mtx);
                // Blocking thread when the task queue is empty.
                task_available_cv.wait(lk, [this]() {
                    return (!is_empty() || is_closed() || threads_destroy > 0) && !is_paused();
                    });
                // Destroying current thread because the idle threads are too many.
                if (threads_destroy > 0 && is_empty() && !is_closed()) {
                    --threads_destroy;
                    ids.emplace(std::this_thread::get_id());
                    break;
                }
                // The pool is closed and there are no tasks, break.
                if (is_closed() && is_empty()) break;
                task = std::move(tasks.front());
                tasks.pop_front();
                --tasks_count;
                lk.unlock();
                ++threads_running;
                task();
                --threads_running;
                // Wait for closing pool when there are no threads that are running and ready to run.  
                if (is_waiting() && !threads_running && is_empty()) {
                    pool_done_cv.notify_all();
                }
            }
        }

        /**
         * @brief The call funtion of manger threads.
         */
        void manager_call() {
            while (true) {
                if (is_closed()) break;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                if (tasks_count > threads_count && threads_count < THREADS_MAX) {
                    size_t add = std::min<size_t>(THREADS_ADD, THREADS_MAX - threads_count);
                    // Resuming the threads that have stopped.
                    size_t j = 0;
                    std::lock_guard<std::mutex> lk(tasks_mtx);
                    for (size_t i = 0; i < THREADS_MAX && j < add && !ids.empty(); ++i) {
                        if (!threads[i].joinable()) continue;
                        auto id = threads[i].get_id();
                        if (ids.contains(id)) {
                            threads[i].join();
                            threads[i] = std::thread(&ThreadPool::worker_call, this);
                            ids.erase(id);
                        }
                    }
                    for (size_t i = 0; i < THREADS_MAX && j < add; ++i) {
                        if (!threads[i].joinable()) {
                            threads[i] = std::thread(&ThreadPool::worker_call, this);
                            ++threads_count;
                            ++j;
                        }
                    }
                }

                // Recycling the threads that have stopped.
                {
                    if (!ids.empty()) {
                        std::lock_guard<std::mutex> lk(tasks_mtx);
                        for (size_t i = 0; i < THREADS_MAX && !ids.empty(); ++i) {
                            if (!threads[i].joinable()) continue;
                            auto id = threads[i].get_id();
                            if (ids.contains(id)) {
                                threads[i].join();
                                --threads_count;
                                ids.erase(id);
                            }
                        }
                    }
                }

                // Destroying the threads tha are idle.
                if (destroy_idle) {
                    if (threads_running * 2 < threads_count && threads_count > THREADS_MIN) {
                        size_t add = std::min<size_t>(THREADS_ADD, threads_count - THREADS_MIN);
                        this->threads_destroy = add;
                    }
                }
            }
        }
    private:
        /**
         * @brief A condition variable to notify woker threads that a new task has become available.
         */
        std::condition_variable task_available_cv = {};

        /**
         * @brief A condition variable to notify main threads that the all tasks have been done.
         */
        std::condition_variable pool_done_cv = {};

        /**
         * @brief A queue of tasks.
         */
        std::deque<std::function<void()>> tasks = {};

        /**
         * @brief A hashset of thread ids to be destroyed.
         */
        std::unordered_set<std::thread::id> ids = {};

        /**
         * @brief The total number of tasks in queue excluding the running tasks.
         */
        std::atomic<size_t> tasks_count = 0;

        /**
         * @brief A mutex to synchronize to access to the task queue.
         */
        std::mutex tasks_mtx = {};

        /**
         * @brief A array of worker threads.
         */
        std::vector<std::thread> threads;

        /**
         * @brief A daemon thread to manage the scheduling of thread-pool.
         */
        std::thread manager;

        /**
         * @brief The MAX number of threads in the pool.
         */
        static inline size_t THREADS_MAX = 4 * std::thread::hardware_concurrency();

        /**
         * @brief The MIN number of threads in the pool.
         */
        static inline size_t THREADS_MIN = std::min<size_t>(2, std::thread::hardware_concurrency() / 2);

        /**
         * @brief The interval of Adding or Destroy threads.
         */
        static constexpr size_t THREADS_ADD = 4;

        /**
         * @brief The total number of threads in the pool.
         */
        std::atomic<size_t> threads_count = 0;

        /**
         * @brief The number of currently running threads.
         */
        std::atomic<size_t> threads_running = 0;

        /**
         * @brief The number of threads to be destroyed.
         */
        std::atomic<size_t> threads_destroy = 0;

        /**
         * @brief A flag of the thread-pool state.
         */
        std::atomic<ThreadPoolState> state = ThreadPoolState::CLOSED;

        bool destroy_idle = false;
    };

}