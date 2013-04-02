#pragma once

#include <string>
#include <sstream>
#include <limits>
#include <iomanip>
#include <iosfwd>
#include <boost/integer.hpp>

namespace Deg {
namespace Runtime {
namespace Math {

template <typename T, size_t Q> class Fixed {
	static_assert(boost::integer_traits<T>::is_integral, "T must be integral");
	typedef typename boost::int_t<std::numeric_limits<T>::digits * 2>::fast Tbig;

private:
	static constexpr T internal_scale_factor(size_t k) {
		return (k == 0) ? (1) : (10 * internal_scale_factor(k - 1));
	}

	static const T scale_factor = internal_scale_factor(Q);

	T data;

	void load_from_string(const std::string& value) {
		data = 0;

		if(value.empty()) {
			return;
		}

		bool is_negative = false;

		size_t i = 0;

		if(value[i] == '-') {
			is_negative = true;
			++i;
		}
		else if(value[i] == '+') {
			++i;
		}

		while(i < value.size() && std::isdigit(value[i])) {
			data *= 10;
			data += value[i] & 0x0F;
			++i;
		}

		size_t q_remaining = Q;

		if(i < value.size() && value[i] == '.') {
			++i;

			while(i < value.size() && q_remaining > 0 && std::isdigit(value[i])) {
				data *= 10;
				data += value[i] & 0x0F;
				++i;
				--q_remaining;
			}
		}

		while(q_remaining > 0) {
			data *= 10;
			--q_remaining;
		}

		if(is_negative) {
			data = -data;
		}
	}

public:
	Fixed() : data(0) {
		return;
	}

	explicit Fixed(const std::string& value)
		: data(0) {
		load_from_string(value);
	}

	explicit Fixed(T raw_value)
		: data(raw_value) {
		return;
	}

	Fixed(const Fixed& d) {
		*this = d;
	}

	Fixed operator+(const Fixed& d) const {
		return Fixed(data + d.data);
	}

	Fixed operator-(const Fixed& d) const {
		return Fixed(data - d.data);
	}

	Fixed operator*(const Fixed& d) const {
		return Fixed(static_cast<T>((static_cast<Tbig>(data) * static_cast<Tbig>(d.data)) / static_cast<Tbig>(scale_factor)));
	}

	Fixed operator/(const Fixed& d) const {
		return Fixed(static_cast<T>((static_cast<double>(data) / static_cast<double>(d.data)) * static_cast<double>(scale_factor)));
	}

	Fixed operator-() const {
		return Fixed(-data);
	}

	const Fixed& operator=(const Fixed& d) {
		data = d.data;
		return d;
	}

	const Fixed& operator+=(const Fixed& d) {
		data += d.data;
		return *this;
	}

	const Fixed& operator-=(const Fixed& d) {
		data -= d.data;
		return *this;
	}

	const Fixed& operator*=(const Fixed& d) {
		data = (data * d.data) / scale_factor;
		return *this;
	}

	const Fixed& operator/=(const Fixed& d) {
		data = static_cast<T>((static_cast<double>(data) / static_cast<double>(d.data)) * static_cast<double>(scale_factor));
		return *this;
	}

	bool operator==(const Fixed& d) const {
		return data == d.data;
	}

	bool operator!=(const Fixed& d) const {
		return data != d.data;
	}

	bool operator >(const Fixed& d) const {
		return data > d.data;
	}

	bool operator >=(const Fixed& d) const {
		return data >= d.data;
	}

	bool operator <(const Fixed& d) const {
		return data < d.data;
	}

	bool operator <=(const Fixed& d) const {
		return data <= d.data;
	}

	operator std::string() const {
		bool neg = false;
		int d = data;
		if(d < 0) {
			d = -d;
			neg = true;
		}

		std::stringstream ss;
		if(neg) {
			ss << "-";
		}

		ss << (d / scale_factor) << "." << std::setw(4) << std::setfill('0') << (d % scale_factor);
		return ss.str();
	}

	inline T GetRawValue() const {
		return data;
	}
};

typedef Fixed<int, 4> DefaultFixed;

inline std::ostream& operator<<(std::ostream& os, DefaultFixed v) {
	return os << static_cast<std::string>(v);
}

}
}
}

namespace std {

template <> class numeric_limits<Deg::Runtime::Math::DefaultFixed> {

	static const bool is_specialized = true;

public:

	static Deg::Runtime::Math::DefaultFixed min() {
		return Deg::Runtime::Math::DefaultFixed(1);
	}

	static Deg::Runtime::Math::DefaultFixed max() {
		return Deg::Runtime::Math::DefaultFixed(numeric_limits<int>::max());
	}

	static Deg::Runtime::Math::DefaultFixed lowest() {
		return Deg::Runtime::Math::DefaultFixed(numeric_limits<int>::lowest());
	}

	static Deg::Runtime::Math::DefaultFixed epsilon() {
		return Deg::Runtime::Math::DefaultFixed(1);
	}
};

}
