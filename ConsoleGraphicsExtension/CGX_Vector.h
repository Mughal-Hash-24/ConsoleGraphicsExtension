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

        void push_back(const T& value) {
            if (count >= capacity) {
                int newCap = (capacity == 0) ? 4 : capacity * 2;
                reserve(newCap);
            }
            data[count] = value;
            count++;
        }

        void reserve(int newCapacity) {
            if (newCapacity <= capacity) return;

            // new[] default constructs elements. 
            // This is required since we cannot use malloc.
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
            // With new[], destructors are called when we delete[], 
            // but for logic we just reset count.
            count = 0;
        }

        void fill(const T& value) {
            for (int i = 0; i < count; ++i) {
                data[i] = value;
            }
        }

        T& operator[](int index) { return data[index]; }
        const T& operator[](int index) const { return data[index]; }
        int size() const { return count; }
        T* begin() { return data; }
        T* end() { return data + count; }
    };
}