#pragma once

#include <initializer_list>
#include <memory>


template <typename T>
class Vector {
private:
    size_t factor;
    size_t vector_capacity;
    std::unique_ptr<T[]> data_ptr;
    size_t data_size;

    void reallocate(size_t);

public:
    Vector();

    Vector(const Vector<T>&);

    Vector<T>& operator=(const Vector<T>&);

    Vector(Vector<T>&&) noexcept;

    Vector<T>& operator=(Vector<T>&&) noexcept;

    Vector(const std::initializer_list<T>&);

    ~Vector();


    T& at(size_t);
    const T& at(size_t) const;

    T& operator[](size_t);
    const T& operator[](size_t) const;

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;


    T* begin();
    const T* begin() const;

    T* end();
    const T* end() const;


    bool empty() const;

    size_t size() const;

    size_t capacity() const;


    void clear();

    void insert(size_t, const T&);
    void insert(size_t, T&&);

    void erase(size_t);

    void push_back(const T&);
    void push_back(T&&);

    void pop_back();

    void resize(size_t);

    void swap(Vector<T>&) noexcept;
};


#include "vector_impl.cpp"
