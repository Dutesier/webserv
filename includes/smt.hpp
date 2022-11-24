#ifndef SMT_HPP
# define SMT_HPP
# include <cstddef>

namespace smt {

template<typename T>
class unique_ptr {
    public:

        unique_ptr() : pointer(NULL) {}

        unique_ptr(T* ptr) : pointer(ptr) {}


        ~unique_ptr() {
            if (pointer != NULL) { delete pointer; }
        }

        T* operator->() { return this->pointer; }

        T& operator*() { return *(this->pointer); }
        
        operator bool() { return (this->pointer != NULL); }

    private:
        unique_ptr(const unique_ptr& other);
        unique_ptr& operator=(const unique_ptr& other);

        T* pointer;
};

template<typename T>
class shared_ptr {
    public:

        shared_ptr() : pointer(NULL), referenceCount(new unsigned int(0)) {}

        shared_ptr(T* ptr) : pointer(ptr), referenceCount(new unsigned int(1)) {}

        shared_ptr(const shared_ptr& other) {
            this->pointer = other.pointer;
            this->referenceCount = other.referenceCount;
            if (this->pointer != NULL) (*(this->referenceCount))++;
        }

        shared_ptr& operator=(const shared_ptr& other) {
            destroy();
            this->pointer = other.pointer;
            this->referenceCount = other.referenceCount;
            if (this->pointer != NULL) (*(this->referenceCount))++;
            return (*this);
        }

        ~shared_ptr() { destroy(); }

        T* operator->() { return this->pointer; }

        T& operator*() { return *(this->pointer); }

        operator bool() { return (this->pointer != NULL); }

    private:

        void destroy() {
            if (*referenceCount) (*referenceCount)--;
            if (*referenceCount == 0) {
                if (pointer != NULL) delete pointer;
                delete referenceCount;
            }
        }

        T*    pointer;
        unsigned int* referenceCount;
};

} // namespace smt

#endif
