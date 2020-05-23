#pragma once
#ifndef FAST_PIMPL
#define FAST_PIMPL

#include <type_traits>

namespace utility
{
	template<typename T, std::size_t SizeType, std::size_t AlignType>
	class FastPimpl
	{
	private:
		std::aligned_storage_t<SizeType, AlignType>    RawData;

	private:
		const T* Ptr()const noexcept
		{
			return reinterpret_cast<T*>(&RawData);
		}
		T* Ptr()noexcept
		{
			return reinterpret_cast<T*>(&RawData);
		}
		template<std::size_t CorrectSize, std::size_t CorrectAlign>
		static void CheckParams() noexcept
		{
			static_assert(SizeType == CorrectSize, "Size type and sizeof(Type) is mismatch");
			static_assert(AlignType == CorrectAlign, "Alignment Type and alignof(Type) is mismatch");
		}
	public:

		FastPimpl()
		{
			new(Ptr()) T();
		}
		template<typename... Args>
		explicit FastPimpl(Args&&... args)
		{
			new(Ptr()) T(std::forward<Args>(args)...);
		}
		FastPimpl(const FastPimpl& other)
		{
			new(Ptr()) T(*other.Ptr());
		}
		T* operator->()noexcept
		{
			return Ptr();
		}
		const T* operator->()const noexcept
		{
			return Ptr();
		}
		T& operator*()noexcept
		{
			return *Ptr();
		}
		const T& operator*()const noexcept
		{
			return &Ptr();
		}
		FastPimpl& operator=(const FastPimpl& other)
		{
			if (this == &other)
				return *this;

			*Ptr() = (*other);
			return *this;
		}
		FastPimpl& operator=(FastPimpl&& other)
		{
			*Ptr() = std::move(*other);
			return *this;
		}
		FastPimpl& operator=(const T& other)
		{
			if (Ptr() == &other)
				return *this;
			*Ptr() = other;

			return *this;
		}
		FastPimpl& operator=(T&& other)
		{
			if (Ptr() == &other)
				return *this;
			*Ptr() = std::move(other);

			return *this;
		}
		~FastPimpl()
		{
			CheckParams<sizeof(T), alignof(T)>();
			Ptr()->~T();
		}
	};
}


#endif //FAST_PIMPL