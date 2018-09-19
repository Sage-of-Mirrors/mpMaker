#pragma once
#include <experimental/filesystem>
#include <vec3.hpp>
#include <cstring>

#include "bstream.h"
#include "array.h"
#include "face.h"
#include "util.h"

class CollisionFile {
private:
	CArrayT<Face *> mFaces;
	glm::vec3 mScale;
	glm::vec3 mMinimumBounds;
	glm::vec3 mAxisLengths;

	void LoadFromObj(bStream::CFileStream * reader);
	void LoadFromMp(bStream::CFileStream * reader);
public:
	CollisionFile();
	CollisionFile(std::experimental::filesystem::path file_name);

	void Debug_ExportObjWithUnk2Colors(std::experimental::filesystem::path file_name);
};