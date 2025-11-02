// #include "vector.h"
#include <algorithm>
#include <initializer_list>
#include <memory>


template <typename T>
void Vector<T>::reallocate(size_t new_capacity)
{
    std::unique_ptr<T[]> new_data_ptr = std::make_unique<T[]>(new_capacity);

    if (std::is_nothrow_move_constructible<T>::value) {
        std::copy_n(std::make_move_iterator(data_ptr.get()), data_size, new_data_ptr.get());
    }
    else {
        std::copy_n(data_ptr.get(), data_size, new_data_ptr.get());
    }

    data_ptr = std::move(new_data_ptr);
    vector_capacity = new_capacity;
}


template <typename T>
Vector<T>::Vector() : factor(2),
                      vector_capacity(8),
                      data_ptr(std::make_unique<T[]>(vector_capacity)),
                      data_size(0) {}


template <typename T>
Vector<T>::Vector(const Vector<T>& other) : factor(other.factor),
                                            vector_capacity(other.vector_capacity),
                                            data_ptr(std::make_unique<T[]>(vector_capacity)),
                                            data_size(other.data_size)
{
    std::copy_n(other.data_ptr.get(), other.data_size, this->data_ptr.get());
}


template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
    if (this != &other) {
        Vector<T> temp(other);
        this->swap(temp);
    }

    return *this;
}


template <typename T>
Vector<T>::Vector(Vector<T>&& other) noexcept : factor(std::move(other.factor)),
                                                vector_capacity(std::move(other.vector_capacity)),
                                                data_ptr(std::move(other.data_ptr)),
                                                data_size(std::move(other.data_size))
{
    other.factor = 2;
    other.vector_capacity = 8;
    other.data_ptr = std::make_unique<T[]>(other.vector_capacity);
    other.data_size = 0;
}


template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept
{
    if (this != &other) {
        Vector<T> temp(std::move(other));
        this->swap(temp);
    }

    return *this;
}


template <typename T>
Vector<T>::Vector(const std::initializer_list<T>& other) : factor(2),
                                                           vector_capacity(std::max(other.size() * factor, size_t(8))),
                                                           data_ptr(std::make_unique<T[]>(vector_capacity)),
                                                           data_size(other.size())
{
    std::copy_n(other.begin(), other.size(), data_ptr.get());
}


template <typename T>
Vector<T>::~Vector() = default;


template <typename T>
T& Vector<T>::at(size_t index)
{
    if (index >= data_size) { throw std::runtime_error("Vector out of bounds!"); }

    return data_ptr[index];
}


template <typename T>
const T& Vector<T>::at(size_t index) const
{
    if (index >= data_size) { throw std::runtime_error("Vector out of bounds!"); }

    return data_ptr[index];
}


template <typename T>
T& Vector<T>::operator[](size_t index)
{
    return data_ptr[index];
}


template <typename T>
const T& Vector<T>::operator[](size_t index) const
{
    return data_ptr[index];
}


template <typename T>
T& Vector<T>::front()
{
    return data_ptr[0];
}


template <typename T>
const T& Vector<T>::front() const
{
    return data_ptr[0];
}


template <typename T>
T& Vector<T>::back()
{
    return data_ptr[data_size - 1];
}


template <typename T>
const T& Vector<T>::back() const
{
    return data_ptr[data_size - 1];
}


template <typename T>
T* Vector<T>::begin()
{
    return &data_ptr[0];
}


template <typename T>
const T* Vector<T>::begin() const
{
    return &data_ptr[0];
}


template <typename T>
T* Vector<T>::end()
{
    return &data_ptr[data_size];
}


template <typename T>
const T* Vector<T>::end() const
{
    return &data_ptr[data_size];
}


template <typename T>
bool Vector<T>::empty() const
{
    return data_size == 0;
}


template <typename T>
size_t Vector<T>::size() const
{
    return data_size;
}


template <typename T>
size_t Vector<T>::capacity() const
{
    return vector_capacity;
}


template <typename T>
void Vector<T>::clear()
{
    for (size_t i = 0; i < data_size; ++i) {
        data_ptr[i] = T();
    }

    data_size = 0;
}


template <typename T>
void Vector<T>::insert(size_t index, const T& value)
{
    if (index > data_size) { throw std::runtime_error("Vector out of bounds!"); }

    if (data_size + 1 > vector_capacity) {
        reallocate(vector_capacity * factor);
    }

    for (size_t i = data_size; i > index; --i) {
        data_ptr[i] = std::move(data_ptr[i - 1]);
    }

    data_ptr[index] = value;
    ++data_size;
}


template <typename T>
void Vector<T>::insert(size_t index, T&& value)
{
    if (index > data_size) { throw std::runtime_error("Vector out of bounds!"); }

    if (data_size + 1 > vector_capacity) {
        reallocate(vector_capacity * factor);
    }

    for (size_t i = data_size; i > index; --i) {
        data_ptr[i] = std::move(data_ptr[i - 1]);
    }

    data_ptr[index] = std::move(value);
    ++data_size;
}


template <typename T>
void Vector<T>::erase(size_t index)
{
    if (index >= data_size) { return; }

    for (size_t i = index; i < data_size - 1; ++i) {
        data_ptr[i] = data_ptr[i + 1];
    }

    --data_size;
}


template <typename T>
void Vector<T>::push_back(const T& value)
{
    if (data_size + 1 > vector_capacity) {
        reallocate(vector_capacity * factor);
    }

    data_ptr[data_size] = value;
    ++data_size;
}


template <typename T>
void Vector<T>::push_back(T&& value)
{
    if (data_size + 1 > vector_capacity) {
        reallocate(vector_capacity * factor);
    }

    data_ptr[data_size] = std::move(value);
    ++data_size;
}


template <typename T>
void Vector<T>::pop_back()
{
    if (empty()) { return; }

    data_ptr[data_size - 1] = T();
    --data_size;
}


template <typename T>
void Vector<T>::resize(size_t new_size)
{
    if (new_size == data_size) { return; }

    if (new_size < data_size) {
        for (size_t i = new_size; i < data_size; ++i) {
            data_ptr[i] = T();
        }

        data_size = new_size;
        return;
    }

    if (new_size > vector_capacity) {
        reallocate(new_size);
    }

    for (size_t i = data_size; i < new_size; ++i) {
        data_ptr[i] = T();
        ++data_size;
    }
}


template <typename T>
void Vector<T>::swap(Vector<T>& other) noexcept
{
    using std::swap;
    swap(this->vector_capacity, other.vector_capacity);
    swap(this->data_ptr, other.data_ptr);
    swap(this->data_size, other.data_size);
    swap(this->factor, other.factor);
}
