

#ifndef UNORDEREDSET_H
#define UNORDEREDSET_H
#include <list>
#include <vector>

template <class KeyT>
class UnorderedSet {
 private:
  std::vector<std::list<KeyT>> buffer_;
  size_t buckets_ = 0;
  size_t elements_ = 0;

 public:
  UnorderedSet() {
    buckets_ = 0;
    elements_ = 0;
    buffer_ = {};
  }
  explicit UnorderedSet(int count) {
    buffer_ = std::vector<std::list<KeyT>>(count);
    buckets_ = count;
    elements_ = 0;
  }
  template <class InputIt>
  UnorderedSet(InputIt first, InputIt last) {
    size_t count = 0;
    for (InputIt it = first; it != last; ++it) {
      ++count;
    }
    buckets_ = static_cast<int>(count);
    buffer_ = std::vector<std::list<KeyT>>(count);
    elements_ = 0;
    for (InputIt it = first; it != last; ++it) {
      Insert(*it);
    }
  }

  UnorderedSet(const UnorderedSet& other) {
    buffer_ = std::vector<std::list<int>>(other.buckets_);
    for (size_t i = 0; i < other.buckets_; ++i) {
      buffer_[i] = other.buffer_[i];
    }
    elements_ = other.elements_;
    buckets_ = other.buckets_;
  }
  UnorderedSet(UnorderedSet&& other) : buffer_(other.buffer_), buckets_(other.buckets_), elements_(other.elements_) {
    other.buffer_ = {};
    other.buckets_ = 0;
    other.elements_ = 0;
  }
  UnorderedSet& operator=(const UnorderedSet& other) {
    if (this != &other) {
      buffer_ = std::vector<std::list<int>>(other.buckets_);
      for (size_t i = 0; i < other.buckets_; ++i) {
        buffer_[i] = other.buffer_[i];
      }
      elements_ = other.elements_;
      buckets_ = other.buckets_;
    }
    return *this;
  }
  UnorderedSet& operator=(UnorderedSet&& other) {
    if (this != &other) {
      buffer_ = other.buffer_;
      elements_ = other.elements_;
      buckets_ = other.buckets_;
      other.Clear();
    }
    return *this;
  }

  [[nodiscard]] int Size() const {
    return elements_;
  }
  [[nodiscard]] bool Empty() const noexcept {
    return Size() == 0;
  }
  void Clear() {
    buckets_ = 0;
    elements_ = 0;
    buffer_ = {};
  }
  void Insert(const KeyT& obj) {
    if (Find(obj)) {
      return;
    }
    if (buckets_ == 0) {
      buffer_ = {{}};
      buckets_ = 1;
    } else if (elements_ == buckets_) {
      Rehash(buckets_ * 2);
    }
    buffer_[std::hash<KeyT>{}(obj) % buckets_].push_front(obj);
    ++elements_;
  }
  void Insert(KeyT&& obj) {
    Insert(obj);
  }
  void Erase(const KeyT& obj) {
    if (elements_ == 0 || buckets_ == 0) {
      return;
    }
    size_t bucket = static_cast<size_t>(std::hash<KeyT>{}(obj)) % buckets_;
    for (auto it = buffer_[bucket].begin(); it != buffer_[bucket].end(); ++it) {
      if (*it == obj) {
        buffer_[bucket].erase(it);
        --elements_;
        return;
      }
    }
  }

  bool Find(const KeyT& obj) const {
    if (elements_ == 0 || buckets_ == 0) {
      return false;
    }
    size_t index = static_cast<size_t>(std::hash<KeyT>{}(obj)) % buckets_;
    for (auto elem : buffer_[index]) {
      if (elem == obj) {
        return true;
      }
    }
    return false;
  }

  void Rehash(size_t size_buffer) {
    if (size_buffer == buckets_ || size_buffer < elements_) {
      return;
    }
    std::vector<std::list<KeyT>> new_buffer(size_buffer);
    for (size_t i = 0; i < buckets_; ++i) {
      for (auto elem : buffer_[i]) {
        new_buffer[static_cast<size_t>(std::hash<KeyT>{}(elem)) % size_buffer].push_front(elem);
      }
    }
    buffer_ = new_buffer;
    buckets_ = size_buffer;
  }

  void Reserve(size_t size_buffer) {
    if (size_buffer > buckets_) {
      Rehash(size_buffer);
    }
  }

  [[nodiscard]] size_t BucketCount() const {
    return buckets_;
  }

  [[nodiscard]] size_t BucketSize(size_t id) const {
    if (id >= buckets_) {
      return 0;
    }
    return buffer_[id].size();
  }

  size_t Bucket(const KeyT& key) const {
    return static_cast<size_t>(std::hash<KeyT>{}(key)) % buckets_;
  }

  [[nodiscard]] double LoadFactor() const {
    if (elements_ == 0) {
      return 0;
    }
    return 1.0 * elements_ / buckets_;
  }
};

#endif  // UNORDEREDSET_H
