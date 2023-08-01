#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <tuple>

// vector, list, string
/** \brief Метафункция проверки на итерабельность
 *
 */
template<typename, typename = void>
constexpr bool is_iterable{};

template<typename T>
constexpr bool is_iterable<T, std::void_t <decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> = true;

/** \brief Метафункция проверки на соответствие std::tuple
 *
 */
template<typename T>
constexpr bool is_tuple{};

template<typename ... Ts>
constexpr bool is_tuple < std::tuple < Ts ... >>  = true;

/** \brief Метафункция проверки, что все типы std::tuple одинаковые
 *
 */
constexpr bool all_same(const std::tuple<>&) {
	return true;
}

template<typename Head, typename ... Tail>
constexpr bool all_same(const std::tuple < Head, Tail ... > &) {
	return (std::is_same_v<Head, Tail> && ...);
}

/** \brief Функция печати кортежа
 *
 * \tparam TupleT кортеж (std::tuple)
 * \tparam Is - последовательность индексов для кортежа TupleT
 */
template<typename TupleT, std::size_t ... Is>
void print_tuple(const TupleT& tp, std::index_sequence < Is ... >) {
	size_t index = 0;
	auto print_elem = [&index](const auto&x) {
		if (index++ > 0)
			std::cout << ".";
		std::cout << x;
	};

	(print_elem(std::get<Is>(tp)), ...);
	std::cout << "\n";
}

/** \brief Функция печати условного IP-адреса
 *
 * \tparam T IP-адрес (std::string, std::vector, std::list )
 */
template < class T, typename Fake = std::enable_if_t < is_iterable < T >>>
void print_ip(T && ip) {
//	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << static_cast<typename T::value_type>(*ip.begin());

	std::string d = std::is_same_v<typename T::value_type, char> ? "" : ".";

	std::for_each(++ip.begin(), ip.end(), [d](const typename T::value_type & n) {std::cout << d << n;});

	std::cout << std::endl;
}

/** \brief Функция печати условного IP-адреса
 *
 * \tparam T IP-адрес (целочисленный тип )
 */
template < class T, typename Fake = std::enable_if_t < std::is_integral < T >::value>>
void print_ip(T ip) {
//	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::vector<int>ip_parts;

	ip_parts.reserve(sizeof(T));

	for (decltype(sizeof(T)) i = 0; i < sizeof(T); ++i) {
		ip_parts.push_back(static_cast<unsigned char>(ip));
		ip >>= 8;
	}

	std::reverse(ip_parts.begin(), ip_parts.end());
	print_ip(std::move(ip_parts));
}

/** \brief Функция печати условного IP-адреса
 *
 * \tparam T IP-адрес (std::tuple )
 */
template<typename T, typename Fake = std::enable_if_t< is_tuple<T> > >
void print_ip(const T & tp) {
//	std::cout << __PRETTY_FUNCTION__ << std::endl;
	if (all_same(tp)) {
		print_tuple(tp, std::make_index_sequence < std::tuple_size_v < T >> {});
	}
	else {
		std::cout << "print ip error: different types\n";
	}

}

int main(int, char **)
{
	print_ip(int8_t {-1}); // 255
	print_ip(int16_t {0}); // 0.0
	print_ip(int32_t {2130706433}); // 127.0.0.1
	print_ip(int64_t {8875824491850138409}); // 123.45.67.89.101.112.131.41
	print_ip(std::string {"Hello, World!"}); // Hello, World!
	print_ip(std::vector<int> {100, 200, 300, 400}); // 100.200.300.400
	print_ip(std::list<short> {400, 300, 200, 100}); // 400.300.200.100
	print_ip(std::make_tuple(123, 456, 789, 0));  // 123.456.789.0
//	print_ip(std::make_tuple(123, 456, 789, '0'));  // print ip error: different types

    return 0;
}
