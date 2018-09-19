#pragma once
#include <stdint.h>
#include <vec3.hpp>
#include <glm.hpp>

#include "array.h"
#include "bstream.h"

class Face {
private:
	glm::vec3 * mVertex1;
	glm::vec3 * mVertex2;
	glm::vec3 * mVertex3;
	glm::vec3 * mNormal;
	glm::vec3 * mEdge1Tangent;
	glm::vec3 * mEdge2Tangent;
	glm::vec3 * mEdge3Tangent;

	glm::vec3 * mUnknown1;
	float mUnknown2;

	uint16_t mUnknownShort1;
	uint16_t mUnknownShort2;

	void CalculateVectorData();
public:
	Face();
	Face(bStream::CFileStream * reader, CArrayT<glm::vec3 *> vertex_positions, CArrayT<glm::vec3 *> normalized_floats);
	Face(std::string face_indices, CArrayT<glm::vec3 *> vertices);

	CArrayT<glm::vec3 *> GetVertices();
	glm::vec3 *  GetNormal();
	CArrayT<glm::vec3 *> GetTangents();
	uint16_t GetUnknownShort2();
};