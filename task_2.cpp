#include <array>
#include <stdexcept>
#include <iostream>
#include <random>
#include <vector>
#include <fstream>

template <class T, size_t CAPACITY>
class FIFOBuffer {
private:
    std::array<T, CAPACITY> _storage;
    size_t _out;
    size_t _size;
public:
    FIFOBuffer() : _out(0), _size(0), _storage() {}
    ~FIFOBuffer() = default;

    bool empty() const {
        return _size == 0;
    }

    T pop() {
        if (empty()) {
            std::__throw_out_of_range("Buffer is empty");
        }
        T tmp;
        std::swap(tmp, _storage[_out]);
        _out = (_out + 1) % CAPACITY;
        --_size;
        return tmp;
    }

    void push(T value) {
        if (_size == CAPACITY) {
            _storage[_out++] = std::move(value);
            _out %= CAPACITY;
        } else {
            _storage[(_out+_size++) % CAPACITY] = std::move(value);
        }
    }

    size_t size() const {
        return _size;
    }
};

template <class T>
class FIFOBufferResizable {
private:
    std::vector<T> _storage;
    size_t _max_capacity;
    size_t _out;
    size_t _size;

    void realloc() {
        std::vector<T> tmp2;
        tmp2.resize(std::min(size_t(_size*1.5 + 0.5), _max_capacity));
        for (int i = 0; i < _size; ++i) {
            std::swap(tmp2[i], _storage[(_out+i) % _storage.size()]);
        }
        std::swap(_storage, tmp2);
        _out = 0;
    }
public:
    FIFOBufferResizable(size_t max_cap) : _out(0), _size(0), _max_capacity(max_cap), _storage() {}
    ~FIFOBufferResizable() = default;

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    T pop() {
        if (empty()) {
            std::__throw_out_of_range("Buffer is empty");
        }
        if (_size * 2 < _storage.size()) { realloc(); }
        T tmp;
        std::swap(tmp, _storage[_out]);
        _out = (_out + 1) % _storage.size();
        --_size;
        if (empty()) { _storage.resize(0); _out = 0; }
        return tmp;
    }

    void push(T value) {
        if (_size == _max_capacity) {
            _storage[_out++] = std::move(value);
            _out %= _storage.size();
        } else {
            if (_size == 0) { _storage.resize(1); }
            else if (_storage.size() == _size) { realloc(); }
            _storage[(_out+_size++) % _storage.size()] = std::move(value);
        }
    }
};
