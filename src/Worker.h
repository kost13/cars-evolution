#ifndef CONCURRENT_WORKER_H
#define CONCURRENT_WORKER_H

#include <functional>
#include <memory>

namespace concurrent {
class Worker {
 public:
  Worker();
  ~Worker();
  bool start();
  bool stop();
  void async(std::function<void()> f);
  void priorityAsync(std::function<void()> f);

 private:
  struct Opaque;
  std::unique_ptr<Opaque> o_;
};
}

#endif  // CONCURRENT_WORKER_H
