#include "face.h"

Face::Face() {
	mVertex1 = nullptr;
	mVertex2 = nullptr;
	mVertex3 = nullptr;
	mNormal = nullptr;
	mEdge1Tangent = nullptr;
	mEdge2Tangent = nullptr;
	mEdge3Tangent = nullptr;

	mUnknown1 = nullptr;
	mUnknown2 = 0.0f;
	mUnknownShort1 = 0x8000;
	mUnknownShort2 = 0;
}

Face::Face(bStream::CFileStream * reader, CArrayT<glm::vec3 *> vertex_positions, CArrayT<glm::vec3 *> normalized_floats) {
	mVertex1 = vertex_positions[reader->readUInt16()];
	mVertex2 = vertex_positions[reader->readUInt16()];
	mVertex3 = vertex_positions[reader->readUInt16()];
	mNormal  = normalized_floats[reader->readUInt16()];
	mEdge1Tangent = normalized_floats[reader->readUInt16()];
	mEdge2Tangent = normalized_floats[reader->readUInt16()];
	mEdge3Tangent = normalized_floats[reader->readUInt16()];
	mUnknown1     = normalized_floats[reader->readUInt16()];
	mUnknown2     = reader->readFloat();
	mUnknownShort1 = reader->readUInt16();
	mUnknownShort2 = reader->readUInt16();
}

Face::Face(std::string face_indices, CArrayT<glm::vec3 *> vertices) {

}

void Face::CalculateVectorData() {
	glm::vec3 edge1 = (*mVertex2) - (*mVertex1);
	glm::vec3 edge2 = (*mVertex1) - (*mVertex3);
	glm::vec3 edge3 = (*mVertex3) - (*mVertex2);

	glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
	glm::vec3 eTan1 = glm::normalize(glm::cross(normal, edge1));
	glm::vec3 eTan2 = glm::normalize(glm::cross(normal, edge2));
	glm::vec3 eTan3 = glm::normalize(glm::cross(normal, edge3));

	mNormal = new glm::vec3(normal.x, normal.y, normal.z);
	mEdge1Tangent = new glm::vec3(eTan1.x, eTan1.y, eTan1.z);
	mEdge2Tangent = new glm::vec3(eTan2.x, eTan2.y, eTan2.z);
	mEdge3Tangent = new glm::vec3(eTan3.x, eTan3.y, eTan3.z);
	mUnknown2 = glm::dot(eTan3, edge1);
}

CArrayT<glm::vec3 *> Face::GetVertices() {
	CArrayT<glm::vec3 *> verts;
	verts.append(mVertex1);
	verts.append(mVertex2);
	verts.append(mVertex3);
	return verts;
}

glm::vec3 * Face::GetNormal() {
	return mNormal;
}

CArrayT<glm::vec3 *> Face::GetTangents() {
	CArrayT<glm::vec3 *> tans;
	tans.append(mEdge1Tangent);
	tans.append(mEdge2Tangent);
	tans.append(mEdge3Tangent);
	return tans;
}

uint16_t Face::GetUnknownShort2() {
	return mUnknownShort2;
}