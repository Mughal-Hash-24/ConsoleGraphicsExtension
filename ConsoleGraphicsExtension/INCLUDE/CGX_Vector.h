#pragma once

namespace CGX {

    template <typename T>
    class Vector {
    private:
        T* data;
        int capacity;
        int count;

    public:
        Vector() : data(nullptr), capacity(0), count(0) {}

        ~Vector() {
            clear();
            if (data) delete[] data;
        }

        // Copy constructor
        Vector(const Vector& other) : data(nullptr), capacity(0), count(0) {
            resize(other.count);
            for (int i = 0; i < other.count; ++i) {
                data[i] = other.data[i];
            }
        }

        // Assignment operator
        Vector& operator=(const Vector& other) {
            if (this != &other) {
                resize(other.count);
                for (int i = 0; i < other.count; ++i) {
                    data[i] = other.data[i];
                }
            }
            return *this;
        }

        void push_back(const T& value) {
            if (count >= capacity) {
                int newCap = (capacity == 0) ? 4 : capacity * 2;
                reserve(newCap);
            }
            data[count] = value;
            count++;
        }

        // NEW: pop_back
        void pop_back() {
            if (count > 0) {
                count--;
            }
        }

        // NEW: empty
        bool empty() const {
            return count == 0;
        }

        // NEW: front
        T& front() {
            return data[0];
        }
        const T& front() const {
            return data[0];
        }

        // NEW: back
        T& back() {
            return data[count - 1];
        }
        const T& back() const {
            return data[count - 1];
        }

        // NEW: erase (pointer based)
        T* erase(T* pos) {
            int index = pos - data;
            if (index < 0 || index >= count) return end();

            // Shift elements left
            for (int i = index; i < count - 1; ++i) {
                data[i] = data[i + 1];
            }
            count--;
            return data + index;
        }

        // NEW: erase (index based)
        void erase_at(int index) {
            if (index < 0 || index >= count) return;
            for (int i = index; i < count - 1; ++i) {
                data[i] = data[i + 1];
            }
            count--;
        }

        void reserve(int newCapacity) {
            if (newCapacity <= capacity) return;

            // new[] default constructs elements. 
            // This is required since we cannot use malloc/realloc with non-POD types safely without placement new
            T* newData = new T[newCapacity];

            // Copy existing elements
            for (int i = 0; i < count; ++i) {
                newData[i] = data[i];
            }

            if (data) delete[] data;
            data = newData;
            capacity = newCapacity;
        }

        void resize(int newSize) {
            if (newSize > capacity) {
                reserve(newSize);
            }
            // If shrinking, we just change the count
            // If growing, new[] already default constructed them
            count = newSize;
        }

        void clear() {
            // Logic reset
            count = 0;
        }

        void fill(const T& value) {
            for (int i = 0; i < count; ++i) {
                data[i] = value;
            }
        }

        // Accessors
        T& operator[](int index) { return data[index]; }
        const T& operator[](int index) const { return data[index]; }

        int size() const { return count; }
        int get_capacity() const { return capacity; }

        // Iterators
        T* begin() { return data; }
        T* end() { return data + count; }
        const T* begin() const { return data; }
        const T* end() const { return data + count; }
    };
}