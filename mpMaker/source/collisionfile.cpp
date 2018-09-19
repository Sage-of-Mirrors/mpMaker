#include "collisionfile.h"

CollisionFile::CollisionFile(std::experimental::filesystem::path file_name) {
	std::string ext = file_name.extension().generic_string();
	bStream::CFileStream reader(file_name.generic_string(), bStream::Big, bStream::In);

	if (ext == ".mp") {
		LoadFromMp(&reader);
	}
	else if (ext == ".obj") {
		LoadFromObj(&reader);
	}
	else {
		std::cout << "The input file \"" << file_name.generic_string() << "\" was not a recognized type.";
	}
}

void CollisionFile::LoadFromMp(bStream::CFileStream * reader) {
	mScale = glm::vec3(reader->readFloat(), reader->readFloat(), reader->readFloat());
	mMinimumBounds = glm::vec3(reader->readFloat(), reader->readFloat(), reader->readFloat());
	mAxisLengths = glm::vec3(reader->readFloat(), reader->readFloat(), reader->readFloat());

	int vertex_offset = reader->readUInt32();
	int normalized_offset = reader->readUInt32();
	int tridata_offset = reader->readUInt32();
	int groupdata_offset = reader->readUInt32();
	int griddata1_offset = reader->readUInt32();
	int griddate2_offset = reader->readUInt32();
	int unk1_offset = reader->readUInt32();

	int vert_count = (normalized_offset - vertex_offset) / 12;
	int normal_count = (tridata_offset - normalized_offset) / 12;
	int tri_count = (groupdata_offset - tridata_offset) / 24;

	CArrayT<glm::vec3 *> vertices;
	CArrayT<glm::vec3 *> normals;

	for (int i = 0; i < vert_count; i++) {
		vertices.append(new glm::vec3(reader->readFloat(), reader->readFloat(), reader->readFloat()));
	}

	for (int i = 0; i < normal_count; i++) {
		normals.append(new glm::vec3(reader->readFloat(), reader->readFloat(), reader->readFloat()));
	}

	reader->seek(tridata_offset);

	for (int i = 0; i < tri_count; i++) {
		mFaces.append(new Face(reader, vertices, normals));
	}
}

void CollisionFile::LoadFromObj(bStream::CFileStream * reader) {
	std::string test = reader->readLine();
	CArrayT<std::string> test_2 = Util::Split(test, " ");
	
	glm::vec3 test_3(strtof(test_2[1].c_str(), nullptr), strtof(test_2[2].c_str(), nullptr), strtof(test_2[3].c_str(), nullptr));
}

void CollisionFile::Debug_ExportObjWithUnk2Colors(std::experimental::filesystem::path file_name) {
	std::string obj_file_name = file_name.generic_string();
	file_name.replace_extension(".mtl");
	std::string mtl_file_name = file_name.generic_string();

	bStream::CFileStream mtl(mtl_file_name, bStream::Big, bStream::Out);

	mtl.writeString("newmtl mat_0\n");
	mtl.writeString("Ns 96.078431\n");
	mtl.writeString("Ka 1.000000 1.000000 1.000000\n");
	mtl.writeString("Kd 1.000000 0.000000 0.000000\n");
	mtl.writeString("Ks 0.500000 0.500000 0.500000\n");
	mtl.writeString("Ke 0.000000 0.000000 0.000000\n");
	mtl.writeString("Ni 1.000000\n");
	mtl.writeString("d 1.000000\n");
	mtl.writeString("illum 2\n\n");

	mtl.writeString("newmtl mat_1\n");
	mtl.writeString("Ns 96.078431\n");
	mtl.writeString("Ka 1.000000 1.000000 1.000000\n");
	mtl.writeString("Kd 1.000000 0.150000 0.000000\n");
	mtl.writeString("Ks 0.500000 0.500000 0.500000\n");
	mtl.writeString("Ke 0.000000 0.000000 0.000000\n");
	mtl.writeString("Ni 1.000000\n");
	mtl.writeString("d 1.000000\n");
	mtl.writeString("illum 2\n\n");

	mtl.writeString("newmtl mat_2\n");
	mtl.writeString("Ns 96.078431\n");
	mtl.writeString("Ka 1.000000 1.000000 1.000000\n");
	mtl.writeString("Kd 1.000000 0.500000 0.000000\n");
	mtl.writeString("Ks 0.500000 0.500000 0.500000\n");
	mtl.writeString("Ke 0.000000 0.000000 0.000000\n");
	mtl.writeString("Ni 1.000000\n");
	mtl.writeString("d 1.000000\n");
	mtl.writeString("illum 2\n\n");

	mtl.writeString("newmtl mat_3\n");
	mtl.writeString("Ns 96.078431\n");
	mtl.writeString("Ka 1.000000 1.000000 1.000000\n");
	mtl.writeString("Kd 0.000000 1.000000 0.000000\n");
	mtl.writeString("Ks 0.500000 0.500000 0.500000\n");
	mtl.writeString("Ke 0.000000 0.000000 0.000000\n");
	mtl.writeString("Ni 1.000000\n");
	mtl.writeString("d 1.000000\n");
	mtl.writeString("illum 2\n\n");

	mtl.writeString("newmtl mat_4\n");
	mtl.writeString("Ns 96.078431\n");
	mtl.writeString("Ka 1.000000 1.000000 1.000000\n");
	mtl.writeString("Kd 0.000000 0.000000 1.000000\n");
	mtl.writeString("Ks 0.500000 0.500000 0.500000\n");
	mtl.writeString("Ke 0.000000 0.000000 0.000000\n");
	mtl.writeString("Ni 1.000000\n");
	mtl.writeString("d 1.000000\n");
	mtl.writeString("illum 2\n\n");

	mtl.writeString("newmtl mat_5\n");
	mtl.writeString("Ns 96.078431\n");
	mtl.writeString("Ka 1.000000 1.000000 1.000000\n");
	mtl.writeString("Kd 1.000000 0.500000 1.000000\n");
	mtl.writeString("Ks 0.500000 0.500000 0.500000\n");
	mtl.writeString("Ke 0.000000 0.000000 0.000000\n");
	mtl.writeString("Ni 1.000000\n");
	mtl.writeString("d 1.000000\n");
	mtl.writeString("illum 2\n\n");

	mtl.writeString("newmtl mat_6\n");
	mtl.writeString("Ns 96.078431\n");
	mtl.writeString("Ka 1.000000 1.000000 1.000000\n");
	mtl.writeString("Kd 0.500000 1.000000 1.000000\n");
	mtl.writeString("Ks 0.500000 0.500000 0.500000\n");
	mtl.writeString("Ke 0.000000 0.000000 0.000000\n");
	mtl.writeString("Ni 1.000000\n");
	mtl.writeString("d 1.000000\n");
	mtl.writeString("illum 2\n");

	CArrayT<glm::vec3 *> vertices;

	for (int i = 0; i < mFaces.size(); i++) {
		Face * face = mFaces[i];
		CArrayT<glm::vec3 *> face_vertices = face->GetVertices();
		vertices.append(face_vertices[0]);
		vertices.append(face_vertices[1]);
		vertices.append(face_vertices[2]);
	}

	bStream::CFileStream obj(obj_file_name, bStream::Big, bStream::Out);

	for (int i = 0; i < vertices.size(); i++) {
		char buff[100];
		snprintf(buff, sizeof(buff), "v %f %f %f\n", vertices[i]->x, vertices[i]->y, vertices[i]->z);
		std::string buffAsStdStr = buff;

		obj.writeString(buffAsStdStr);
	}

	obj.writeString("\n");

	obj.writeString("usemtl mat_0\n");
	for (int i = 0; i < mFaces.size(); i++) {
		if (mFaces[i]->GetUnknownShort2() != 0) {
			continue;
		}

		CArrayT<glm::vec3 *> face_vertices = mFaces[i]->GetVertices();
		size_t vert1, vert2, vert3;

		vertices.contains(face_vertices[0], &vert1);
		vertices.contains(face_vertices[1], &vert2);
		vertices.contains(face_vertices[2], &vert3);

		char buff[100];
		snprintf(buff, sizeof(buff), "f %i %i %i\n", vert1 + 1, vert2 + 1, vert3 + 1);
		std::string buffAsStdStr = buff;

		obj.writeString(buffAsStdStr);
	}

	obj.writeString("\n");

	obj.writeString("usemtl mat_1\n");
	for (int i = 0; i < mFaces.size(); i++) {
		if (mFaces[i]->GetUnknownShort2() != 1) {
			continue;
		}

		CArrayT<glm::vec3 *> face_vertices = mFaces[i]->GetVertices();
		size_t vert1, vert2, vert3;

		vertices.contains(face_vertices[0], &vert1);
		vertices.contains(face_vertices[1], &vert2);
		vertices.contains(face_vertices[2], &vert3);

		char buff[100];
		snprintf(buff, sizeof(buff), "f %i %i %i\n", vert1 + 1, vert2 + 1, vert3 + 1);
		std::string buffAsStdStr = buff;

		obj.writeString(buffAsStdStr);
	}

	obj.writeString("\n");

	obj.writeString("usemtl mat_2\n");
	for (int i = 0; i < mFaces.size(); i++) {
		if (mFaces[i]->GetUnknownShort2() != 2) {
			continue;
		}

		CArrayT<glm::vec3 *> face_vertices = mFaces[i]->GetVertices();
		size_t vert1, vert2, vert3;

		vertices.contains(face_vertices[0], &vert1);
		vertices.contains(face_vertices[1], &vert2);
		vertices.contains(face_vertices[2], &vert3);

		char buff[100];
		snprintf(buff, sizeof(buff), "f %i %i %i\n", vert1 + 1, vert2 + 1, vert3 + 1);
		std::string buffAsStdStr = buff;

		obj.writeString(buffAsStdStr);
	}

	obj.writeString("\n");

	obj.writeString("usemtl mat_3\n");
	for (int i = 0; i < mFaces.size(); i++) {
		if (mFaces[i]->GetUnknownShort2() != 3) {
			continue;
		}

		CArrayT<glm::vec3 *> face_vertices = mFaces[i]->GetVertices();
		size_t vert1, vert2, vert3;

		vertices.contains(face_vertices[0], &vert1);
		vertices.contains(face_vertices[1], &vert2);
		vertices.contains(face_vertices[2], &vert3);

		char buff[100];
		snprintf(buff, sizeof(buff), "f %i %i %i\n", vert1 + 1, vert2 + 1, vert3 + 1);
		std::string buffAsStdStr = buff;

		obj.writeString(buffAsStdStr);
	}

	obj.writeString("\n");

	obj.writeString("usemtl mat_4\n");
	for (int i = 0; i < mFaces.size(); i++) {
		if (mFaces[i]->GetUnknownShort2() != 4) {
			continue;
		}

		CArrayT<glm::vec3 *> face_vertices = mFaces[i]->GetVertices();
		size_t vert1, vert2, vert3;

		vertices.contains(face_vertices[0], &vert1);
		vertices.contains(face_vertices[1], &vert2);
		vertices.contains(face_vertices[2], &vert3);

		char buff[100];
		snprintf(buff, sizeof(buff), "f %i %i %i\n", vert1 + 1, vert2 + 1, vert3 + 1);
		std::string buffAsStdStr = buff;

		obj.writeString(buffAsStdStr);
	}

	obj.writeString("\n");

	obj.writeString("usemtl mat_5\n");
	for (int i = 0; i < mFaces.size(); i++) {
		if (mFaces[i]->GetUnknownShort2() != 5) {
			continue;
		}

		CArrayT<glm::vec3 *> face_vertices = mFaces[i]->GetVertices();
		size_t vert1, vert2, vert3;

		vertices.contains(face_vertices[0], &vert1);
		vertices.contains(face_vertices[1], &vert2);
		vertices.contains(face_vertices[2], &vert3);

		char buff[100];
		snprintf(buff, sizeof(buff), "f %i %i %i\n", vert1 + 1, vert2 + 1, vert3 + 1);
		std::string buffAsStdStr = buff;

		obj.writeString(buffAsStdStr);
	}

	obj.writeString("\n");

	obj.writeString("usemtl mat_6\n");
	for (int i = 0; i < mFaces.size(); i++) {
		if (mFaces[i]->GetUnknownShort2() != 6) {
			continue;
		}

		CArrayT<glm::vec3 *> face_vertices = mFaces[i]->GetVertices();
		size_t vert1, vert2, vert3;

		vertices.contains(face_vertices[0], &vert1);
		vertices.contains(face_vertices[1], &vert2);
		vertices.contains(face_vertices[2], &vert3);

		char buff[100];
		snprintf(buff, sizeof(buff), "f %i %i %i\n", vert1 + 1, vert2 + 1, vert3 + 1);
		std::string buffAsStdStr = buff;

		obj.writeString(buffAsStdStr);
	}
}