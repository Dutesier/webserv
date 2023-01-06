#ifndef SMT_HPP
#define SMT_HPP

#include <cstddef>

namespace smt {

template<typename T>
class unique_ptr {
    public:

        unique_ptr() : pointer(NULL) {}

        unique_ptr(T* ptr) : pointer(ptr) {}

        unique_ptr(unique_ptr const& other) = delete;
        unique_ptr& operator=(unique_ptr const& other) = delete;

        ~unique_ptr() {
            if (pointer != nullptr) delete pointer;
        }

        T* operator->() { return this->pointer; }

        T& operator*() { return *(this->pointer); }

    private:

        T* pointer;
};

template<typename T>
class shared_ptr {
    public:

        typedef unsigned int
            uint; // uint was unrecognized, so I added this line

        shared_ptr() : pointer(NULL), referenceCount(new uint(0)) {}

        shared_ptr(T* ptr) : pointer(ptr), referenceCount(new uint(1)) {}

        shared_ptr(shared_ptr const& other) {
            this->pointer = other.pointer;
            this->referenceCount = other.referenceCount;
            if (this->pointer != NULL) (*(this->referenceCount))++;
        }

        shared_ptr& operator=(shared_ptr const& other) {
            destroy();
            this->pointer = other.pointer;
            this->referenceCount = other.referenceCount;
            if (this->pointer != NULL) (*(this->referenceCount))++;
            return (*this);
        }

        ~shared_ptr() { destroy(); }

        T* operator->() { return this->pointer; }

        T& operator*() { return *(this->pointer); }

    private:

        void destroy() {
            if (*referenceCount) (*referenceCount)--;
            if (*referenceCount == 0) {
                if (pointer != NULL) delete pointer;
                delete referenceCount;
            }
        }

        T*    pointer;
        uint* referenceCount;
};

} // namespace smt

#endif
