#pragma once
#include <iostream>
#include <cstdint>
#include "array.h"

class Util {
public:
	static CArrayT<std::string> Split(std::string src, std::string separator);
};