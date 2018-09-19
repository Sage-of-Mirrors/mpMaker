#include "util.h"

CArrayT<std::string> Util::Split(std::string src, std::string separator) {
	CArrayT<std::string> result;

	size_t last_pos = 0;
	size_t cur_pos = src.find(separator, 0);

	while (cur_pos != std::string::npos) {
		result.append(src.substr(last_pos, cur_pos - last_pos));

		last_pos = cur_pos + 1;
		cur_pos = src.find(separator, last_pos);
	}

	if (result.size() == 0) {
		return result;
	}

	result.append(src.substr(last_pos, src.size() - last_pos));

	return result;
}