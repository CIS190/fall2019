#ifndef UNIQUE_POINTER_HPP
#define UNIQUE_POINTER_HPP

template <typename T>
class unique_pointer
{
public:
    unique_pointer(T *) noexcept;
    unique_pointer(const unique_pointer &) = delete;
    unique_pointer(unique_pointer &&) noexcept;
    ~unique_pointer()

    unique_pointer & operator=(unique_pointer &&) noexcept;

    T * release() noexcept;
    void reset(T *) noexcept;
    T * get() const noexcept;

    T operator*() const;
    T * operator->() const noexcept;
};

#endif
