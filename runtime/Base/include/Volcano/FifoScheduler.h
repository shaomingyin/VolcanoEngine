#ifndef VOLCANO_FIFO_SCHEDULER_H
#define VOLCANO_FIFO_SCHEDULER_H

#include <atomic>
#include <cstddef>
#include <mutex>
#include <utility>

#include <stdexec/execution.hpp>

#include <Volcano/Common.h>

VOLCANO_BEGIN

class FifoScheduler {
    struct TaskNode {
        void (*execute)(TaskNode*) noexcept = nullptr;
        TaskNode* next = nullptr;
    };

    void enqueue(TaskNode* task) noexcept {
        {
            std::lock_guard lock(mutex_);
            if (tail_ != nullptr) {
                tail_->next = task;
            } else {
                head_ = task;
            }
            tail_ = task;
            task->next = nullptr;
        }
        enqueued_count_.fetch_add(1, std::memory_order_release);
    }

public:
    class Scheduler {
    public:
        using scheduler_concept = stdexec::scheduler_t;

        struct Env {
            FifoScheduler* queue_ = nullptr;

            template <typename Tag>
                requires stdexec::__one_of<Tag, stdexec::set_value_t, stdexec::set_stopped_t>
            Scheduler query(stdexec::get_completion_scheduler_t<Tag>) const noexcept {
                return Scheduler{queue_};
            }
        };

        struct Sender {
            using sender_concept = stdexec::sender_t;
            using completion_signatures = stdexec::completion_signatures<
                stdexec::set_value_t(),
                stdexec::set_stopped_t()
            >;

            FifoScheduler* queue_ = nullptr;

            template <typename Receiver>
            struct OpState : TaskNode {
                FifoScheduler* queue_ = nullptr;
                Receiver rcvr_;

                OpState(FifoScheduler* queue, Receiver rcvr)
                    : TaskNode{&OpState::run}, queue_(queue), rcvr_(std::move(rcvr)) {}

                static void run(TaskNode* base) noexcept {
                    auto* self = static_cast<OpState*>(base);
                    if (stdexec::get_stop_token(stdexec::get_env(self->rcvr_)).stop_requested()) {
                        stdexec::set_stopped(std::move(self->rcvr_));
                    } else {
                        stdexec::set_value(std::move(self->rcvr_));
                    }
                }

                void start() noexcept {
                    queue_->enqueue(this);
                }
            };

            template <typename Receiver>
            auto connect(Receiver rcvr) const {
                return OpState<Receiver>{queue_, std::move(rcvr)};
            }

            Env get_env() const noexcept {
                return Env{queue_};
            }
        };

        Scheduler() noexcept = default;
        explicit Scheduler(FifoScheduler* queue) noexcept : queue_(queue) {}

        bool operator==(const Scheduler& other) const noexcept = default;

        Sender schedule() const noexcept {
            return Sender{queue_};
        }

    private:
        FifoScheduler* queue_ = nullptr;
    };

    FifoScheduler() = default;
    FifoScheduler(const FifoScheduler&) = delete;
    FifoScheduler& operator=(const FifoScheduler&) = delete;

    Scheduler get_scheduler() noexcept {
        return Scheduler{this};
    }

    Scheduler::Sender schedule() noexcept {
        return get_scheduler().schedule();
    }

    std::size_t run_all_tasks() noexcept {
        TaskNode* list = nullptr;
        {
            std::lock_guard lock(mutex_);
            list = head_;
            head_ = nullptr;
            tail_ = nullptr;
        }
        std::size_t count = 0;
        while (list != nullptr) {
            auto* current = list;
            list = list->next;
            current->execute(current);
            ++count;
        }
        return count;
    }

    uint64_t enqueued_count() const noexcept {
        return enqueued_count_.load(std::memory_order_acquire);
    }

private:
    std::mutex mutex_;
    TaskNode* head_ = nullptr;
    TaskNode* tail_ = nullptr;
    std::atomic<uint64_t> enqueued_count_{0};
};

static_assert(stdexec::scheduler<FifoScheduler::Scheduler>);

VOLCANO_END

#endif // VOLCANO_FIFO_SCHEDULER_H
