#ifndef SMT_HPP
# define SMT_HPP

namespace smt {

template<typename T>
class unique_ptr {
public:
    unique_ptr(): pointer(nullptr) {}
    unique_ptr(T* ptr): pointer(ptr) {}

    unique_ptr(const unique_ptr& other) = delete;
    unique_ptr& operator=(const unique_ptr& other) = delete;

    ~unique_ptr(){
        if (pointer != nullptr) {
            delete pointer;
        }
    }

    T* operator->(){ return this->pointer; }
    T& operator*(){ return *(this->pointer); }

private:
    T* pointer = nullptr;
};

template<typename T>
class shared_ptr {
public:
    shared_ptr(): pointer(nullptr), referenceCount(new uint(0)) {
    }
    shared_ptr(T* ptr): pointer(ptr), referenceCount(new uint(1)) {
    }

    shared_ptr(const shared_ptr& other){
        this->pointer = other.pointer;
        this->referenceCount = other.referenceCount;
        if (this->pointer != nullptr) {
            (*(this->referenceCount))++;
        }
    }

    shared_ptr& operator=(const shared_ptr& other){
        destroy();
        this->pointer = other.pointer;
        this->referenceCount = other.referenceCount;
        if (this->pointer != nullptr) {
            (*(this->referenceCount))++;
        }
        return (*this);
    }

    ~shared_ptr(){
        destroy();
    }

    T* operator->(){ return this->pointer; }
    T& operator*(){ return *(this->pointer); }

private:
    void destroy(){
        if (*referenceCount)
            (*referenceCount)--;
        if (*referenceCount == 0) {
            if (pointer != nullptr) {
                delete pointer;
            }
            delete referenceCount;
        }
    }

    T*      pointer = nullptr;
    uint*   referenceCount = nullptr;
};

}

#endif