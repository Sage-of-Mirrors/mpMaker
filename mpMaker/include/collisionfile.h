#pragma once
#include <experimental/filesystem>
#include <vec3.hpp>
#include <cstring>

#include "bstream.h"
#include "array.h"
#include "face.h"
#include "util.h"

enum FileType {
	mp,
	obj
};

class CollisionFile {
private:
	CArrayT<Face *> mFaces;
	glm::vec3 mScale;
	glm::vec3 mMinimumBounds;
	glm::vec3 mAxisLengths;

	void LoadFromObj(bStream::CFileStream * reader);
	void LoadFromMp(bStream::CFileStream * reader);

	void WriteToObj(bStream::CFileStream * writer);
	void WriteToMp(bStream::CFileStream * writer);

	CArrayT<glm::vec3 *> ReadVertsFromObj(bStream::CFileStream * reader);
	void ReadFacesFromObj(bStream::CFileStream * reader, CArrayT<glm::vec3 *> vertices);
public:
	CollisionFile();
	CollisionFile(std::experimental::filesystem::path file_name);

	void Write(std::experimental::filesystem::path file_name, FileType output_type);
	void Debug_ExportObjWithUnk2Colors(std::experimental::filesystem::path file_name);
};