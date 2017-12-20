#ifndef OBJVEC_H
#define OBJVEC_H

#include <iostream>
#include <QDebug>

template <class T>
class ObjVec
{
public:
    ObjVec();
    explicit ObjVec(size_t const & i);
    ObjVec(ObjVec<T> const & vec);
    void append(T const & data_);
    size_t size() const;
    bool isEmpty() const;
    void remove(size_t const & n);
    T& operator[](size_t const & n);
    ~ObjVec();
private:
    T * data;
    size_t size_;
};

template <class T>
ObjVec<T>::ObjVec()
        :size_(0)
{    qDebug() << "ObjVec(size_t const & i)/n"; }

template <class T>
ObjVec<T>::ObjVec(size_t const & i)
        :size_(i)
{    qDebug() << "ObjVec(size_t const & i)/n"; }

template <class T>
ObjVec<T>::ObjVec(ObjVec<T> const & vec)
        :size_(vec.size())
{
    data = new T[size_];
    for (size_t i = 0; i < size_; ++i){
        data[i] = vec.data[i];
    }
    qDebug() << "ObjVec(ObjVec<T> const & vec)/n";
}

template <class T>
void ObjVec<T>::append(T const & data_){
    T * copy = new T [size_ + 1];
    for (size_t i = 0; i < size_; ++i){
        copy[i] = data[i];
    }
    copy[size_] = data_;
    if (size_ > 0)
        delete[] data;
    data = copy;
    size_++;
    qDebug() << "append(T const & data_)/n";
}

template <class T>
bool ObjVec<T>::isEmpty() const{
    return size_ == 0;
    qDebug() << "isEmpty()/n";
}

template <class T>
void ObjVec<T>::remove(size_t const & n){
    if (n < 0)
        return;
    T * copy = new T[size_ - 1];
    T * ptr = copy;
    for (size_t i = 0; i < n; ++i){
        if (i != n){
            *ptr++ = data[i];
        }
    }
    if (size_ > 0)
        delete[] data;
    data = copy;
    size_--;
    qDebug() << "remove(size_t const & /n)\n";
}

template <class T>
T& ObjVec<T>::operator[](size_t const & n){
    return data[n];
    qDebug() << "operator[]/n)\n";
}

template <class T>
ObjVec<T>::~ObjVec(){
    if (size_ > 0)
        delete[] data;
    qDebug() << "~ObjVec()/n)\n";
}

template <class T>
size_t ObjVec<T>::size() const{
    return size_;
}


#endif // OBJVEC_H
