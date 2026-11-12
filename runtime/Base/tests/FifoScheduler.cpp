#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include <gtest/gtest.h>
#include <stdexec/execution.hpp>

#include <Volcano/FifoScheduler.h>

namespace {
using Volcano::FifoScheduler;

TEST(FifoSchedulerTest, SatisfiesSchedulerConceptAndRunsEnqueuedWork) {
    FifoScheduler queue;
    auto sched = queue.get_scheduler();
    EXPECT_EQ(sched, queue.get_scheduler());

    int count = 0;
    auto sndr = stdexec::schedule(sched) | stdexec::then([&count] {
        count += 10;
    });

    struct Receiver {
        using receiver_concept = stdexec::receiver_tag;
        int* counter = nullptr;

        void set_value() noexcept {
            *counter += 1;
        }
        void set_error(std::exception_ptr) noexcept {}
        void set_stopped() noexcept {}
    };

    auto op = stdexec::connect(std::move(sndr), Receiver{&count});
    op.start();

    EXPECT_EQ(count, 0);
    EXPECT_EQ(queue.run_all_tasks(), 1);
    EXPECT_EQ(count, 11);
    EXPECT_EQ(queue.run_all_tasks(), 0);
}

TEST(FifoSchedulerTest, EnqueuesFromAnotherThreadSafely) {
    FifoScheduler queue;
    std::atomic<bool> done{false};

    struct Receiver {
        using receiver_concept = stdexec::receiver_tag;
        std::atomic<bool>* flag = nullptr;

        void set_value() noexcept {
            flag->store(true);
        }
        void set_error(std::exception_ptr) noexcept {}
        void set_stopped() noexcept {}
    };

    auto op = stdexec::connect(stdexec::schedule(queue.get_scheduler()), Receiver{&done});

    std::thread producer([&] {
        op.start();
    });
    producer.join();

    EXPECT_FALSE(done.load());
    EXPECT_EQ(queue.run_all_tasks(), 1);
    EXPECT_TRUE(done.load());
}
}
