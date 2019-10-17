#include <boost/dll/config.hpp>
#include <boost/dll.hpp>
#include <vector>

extern "C" BOOST_SYMBOL_EXPORT int vec_sum(std::vector<int> v);

int vec_sum(std::vector<int> v) {
	int s = 0;
	for (unsigned i = 0; i < v.size(); i++)
		s += v.at(i);
	return s;
}