template<typename T>
void ThreadSafeQueue<T>::push(T item) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(std::move(item));
    cond_.notify_one();
}

template <typename T>
std::optional<T> ThreadSafeQueue<T>::front()
{
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return std::nullopt;
    }

    return queue_.front();
}

template<typename T>
bool ThreadSafeQueue<T>::try_pop(T& item) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return false;
    }
    item = std::move(queue_.front());
    queue_.pop();
    return true;
}

template<typename T>
std::optional<T> ThreadSafeQueue<T>::pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this] { return !queue_.empty(); });

    T item = std::move(queue_.front());
    queue_.pop();

    return item;
}

template<typename T>
bool ThreadSafeQueue<T>::empty() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
}

template<typename T>
size_t ThreadSafeQueue<T>::size() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.size();
}
