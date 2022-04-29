#include <iostream>


template <typename T>
class circul_buf {
private:
    friend class circul_iterator;
    template<class Iter>
    class circul_iterator {
        friend class circul_buf;

    public:
        typedef Iter iterator_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef int difference_type;
        typedef iterator_type value_type;
        typedef iterator_type& reference;
        typedef iterator_type* pointer;

        pointer value;
        circul_buf<iterator_type> buf;

    private:
        circul_iterator(circul_buf &buf, Iter* p) :
        value(p),
        buf(buf)

        {}

    public:
        circul_iterator(const circul_iterator& it) :
         value(it.value),
         buf(it.buf)
//         buffer(it.buffer),
//         size_alloc(it.size_alloc),
//         count_elem(it.count_elem),
//         start_end_equal(it.start_end_equal),
//         start(it.start),
//         end(it.end)
         {}

        circul_iterator& operator++() {
            if (value == (buf.buffer + buf.size_alloc - 1))
                value = buf.buffer;
            else
                value++;
            return *this;
        }

        circul_iterator& operator--() {
            if (value == buf.buffer) {
                value = buf.buffer + buf.size_alloc - 1;
            }
            else
                value--;
            return *this;
        }

        circul_iterator& operator+(int &n) {
            int index;
            if (value > buf.start)
                index = value - buf.start;
            else
                index = ((buf.buffer + buf.size_alloc) - buf.start) + (value - buf.buffer);

            if (n + index < buf.count_elem) {
                value = buf[n + index];
                return *this;
            }
        }

        iterator_type& operator*() {
            if (value != buf.end_)
                return *value;
        }


        bool operator==(circul_iterator const& other) const {
            return value == other.value;
        }

        bool operator!=(circul_iterator const& other) const {
            return value != other.value;
        }

    };

public:
    typedef T* pointer_t;
    typedef T value_t;



    circul_buf() {
        size_alloc = 1;
        buffer = new value_t[1];
        start = buffer;
        end_ = buffer;
        start_end_equal = true;
        count_elem = 0;
    }

    circul_buf(unsigned n) {
        size_alloc = n;
        buffer = new value_t[n];
        start = buffer;
        end_ = buffer;
        start_end_equal = true;
        count_elem = 0;
    }

    ~circul_buf() {
        free(buffer);
        free(start);
        free(end_);

    }

    typedef circul_iterator<T> iterator;
    typedef circul_iterator<const T> const_iterator;

    iterator begin() {
        iterator it(*this, start);
        return it;
    }

    iterator end() {
        iterator it(*this, end_);
        return it;
    }

    void push_back(value_t new_elem) {
        if (count_elem < size_alloc) {
            if (end_ == (buffer + (size_alloc)))
                end_ = buffer;
            *end_ = new_elem;
            end_++;
            count_elem++;
        }
        else {
            int transfer_st = start - buffer, transfer_end = end_ - buffer;
            size_alloc *= 2;
//            buffer = (value_t*)realloc(buffer, size_alloc * sizeof(value_t));

            pointer_t buffer2 = new value_t[size_alloc * 2];
            for (int i = 0; i < (size_alloc / 2); i++)
                buffer2[i] = buffer[i];
            delete[] buffer;
            buffer = buffer2;

            start = buffer + (transfer_st * sizeof(value_t));
            pointer_t temp= buffer + ((size_alloc / 2) * sizeof(value_t));
            if ((buffer + (transfer_end * sizeof(value_t))) == temp) {
                end_ = buffer + transfer_end;
            }
            else {
                end_ = temp + (transfer_end * sizeof(value_t));
                while (temp != end_) {
                    *temp = *(buffer - (buffer + ((size_alloc / 2) * sizeof(value_t)) - temp));
                    temp++;
                }
            }
            *end_ = new_elem;
            end_++;
            count_elem++;
        }

        if (end_ == start)
            start_end_equal = true;
        else
            start_end_equal = false;
    }

    void push_front(value_t new_elem) {
        if (count_elem < size_alloc) {

        }
        else {
            int transfer_st = start - buffer, transfer_end = end_ - buffer;
            size_alloc *= 2;

            pointer_t buffer2 = new value_t[size_alloc * 2];
            for (int i = 0; i < (size_alloc / 2); i++)
                buffer2[i] = buffer[i];
            delete[] buffer;
            buffer = buffer2;

            start = buffer + (transfer_st * sizeof(value_t));
            pointer_t temp= buffer + ((size_alloc / 2) * sizeof(value_t));
            if ((buffer + (transfer_end * sizeof(value_t))) == temp) {
                end_ = buffer + transfer_end;
            }
            else {
                end_ = temp + (transfer_end * sizeof(value_t));
                while (temp != end_) {
                    *temp = *(buffer - (buffer + ((size_alloc / 2) * sizeof(value_t)) - temp));
                    temp++;
                }
            }

        }
        if (start != buffer)
            start--;
        else
            start = buffer + (size_alloc - 1) ;

        *start = new_elem;
        count_elem++;

        if (end_ == start)
            start_end_equal = true;
        else
            start_end_equal = false;

    }

    void pop_front() {
        if (count_elem > 0) {
            if (start == buffer + (size_alloc - 1)) {
                start = buffer;
            }
            else
                start++;

            count_elem--;
        }
        if (end_ == start)
            start_end_equal = true;
        else
            start_end_equal = false;
    }

    void pop_back() {
        if (count_elem > 0) {
            if (end_ == buffer) {
                end_ = buffer + ((size_alloc - 1));
            }
            else
                end_--;
            count_elem--;
        }
        if (end_ == start)
            start_end_equal = true;
        else
            start_end_equal = false;
    }

    value_t get_start() {
        return *start;
    }
    value_t get_end() {
        if (end_ == buffer) {
            return  *(buffer + (size_alloc - 1));
        }
        else
            return *(end_ - 1);
    }

    value_t& operator[](const unsigned& n) {
        if (n < count_elem) {
            if (start - end_ > 0) {
                if (n < ((buffer + size_alloc) - start))
                    return *(start + n);
                else
                    return *(buffer + n - (buffer + size_alloc - start));
            } else {
                return *(start + n);
            }
        }
        else {
            return *end_;
        }
    }

    void change_capasity(unsigned n) {

    }

    unsigned size() {
        return count_elem;
    }



private:
    pointer_t buffer;
    unsigned size_alloc;
    unsigned count_elem;
    bool start_end_equal;
    pointer_t start;
    pointer_t end_;
};
int main() {
    circul_buf<int> arr;
    arr.push_back(5);
    arr.push_back(6);
    arr.push_back(7);
    arr.push_back(8);
    arr.push_back(9);
    arr.push_back(10);
    arr.push_back(11);
    arr.push_back(12);
    arr.pop_front();
    arr.pop_front();
    arr.pop_front();
//    arr.pop_back();
//    arr.pop_back();
//    arr.pop_back();
//    arr.push_front(13);
//    arr.push_front(14);
//    arr.push_front(15);
//    arr.push_front(16);
    arr.push_back(13);
    arr.push_back(14);
//    arr.push_back(15);

    circul_buf<int>::iterator i1 = arr.begin();
    circul_buf<int>::iterator i2 = arr.end();
    int k = 0;
    while (i1 != i2) {
        *i1 = 20;

        ++i1;
    }


//    arr.push_back(14);


    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
