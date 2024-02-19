#pragma once

template <typename T>
class shared_ptr
{
private:
	T* data_ = nullptr;
	struct refcount
	{
		size_t use_count;
		T* object;	
	}* refCount;
public:
	shared_ptr() {}
	shared_ptr(std::nullptr_t) noexcept {}
	shared_ptr(T* data) try : data_(data), refCount(new refcount())
	{
		refCount->object = data;
		if (data)
			++refCount->use_count;
	}
	catch (...)
	{
		delete data;
	}
	~shared_ptr() 
	{
		if (data_)
			reset();
	}
	shared_ptr(const shared_ptr& other) : data_(other.data_), refCount(other.refCount)
	{
		if (data_)
			refCount->use_count++;
	}
	shared_ptr& operator=(const shared_ptr& other)
	{
		if (this == &other) return *this;
		other.swap(*this);
		return *this;
	}
	shared_ptr(shared_ptr&& ptr) noexcept : data_(std::move(ptr.data_)), refCount(std::move(ptr.refCount)) { ptr.reset(); }
	shared_ptr& operator=(shared_ptr&& ptr) noexcept 
	{ 
		data_ = std::move(ptr.data_);
		refCount = std::move(ptr.refCount);
		ptr.reset();
		return *this; 
	}
	void reset()
	{
		data_ = nullptr;
		if (refCount)
			--refCount->use_count;
		if (refCount->use_count <= 0)
		{
			delete refCount->object;
			delete refCount;
		}
	}
	void swap(shared_ptr& other) noexcept
	{
		T* tmp = data_;
		data_ = other.data_;
		other.data_ = tmp;
		int tmpCount = refCount;
		refCount = other.refCount;
		other.refCount = tmpCount;
	}
	int getCount() const { return refCount->use_count; }
	T* get() { return data_; }
	explicit operator bool() const { return data_ != nullptr; }
	T* operator->() const { return data_; }
	T& operator*() const { return *data_; }
};
