#include "collisionfile.h"

CollisionFile::CollisionFile(std::experimental::filesystem::path file_name) {
	bStream::CFileStream reader(file_name.generic_string(), bStream::Big, bStream::In);

	if (!reader.getStream().is_open()) {
		std::cout << "Could not open file " << file_name << "!" << std::endl;
		return;
	}

	std::string ext = file_name.extension().generic_string();

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

	int vert_count = (normalized_offset - vertex_offset) / 12; // Each vertex is three floats, which is 12 bytes total
	int normal_count = (tridata_offset - normalized_offset) / 12; // See above
	int tri_count = (groupdata_offset - tridata_offset) / 24; // Each triangle is 24 bytes long

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
	CArrayT<glm::vec3 *> vertices = ReadVertsFromObj(reader);
	ReadFacesFromObj(reader, vertices);
}

CArrayT<glm::vec3 *> CollisionFile::ReadVertsFromObj(bStream::CFileStream * reader) {
	CArrayT<glm::vec3 *> verts;

	// We're going to scan the entire file for lines containing vertices.
	// That is, lines beginning with the token 'v'.

	while (!reader->getStream().eof()) {
		std::string line = reader->readLine();
		CArrayT<std::string> split_line = Util::Split(line, " ");

		if (split_line.size() > 0 && split_line[0] == "v") {
			glm::vec3 * new_vert = new glm::vec3(std::stof(split_line[1]), std::stof(split_line[2]), std::stof(split_line[3]));
			verts.append(new_vert);
		}
	}

	// Return the reader to the beginning of the file
	reader->seek(0);

	return verts;
}

void CollisionFile::ReadFacesFromObj(bStream::CFileStream * reader, CArrayT<glm::vec3 *> vertices) {
	// Just like reading vertices, we're going to scan the entire file for faces.
	// We're looking for lines starting with the token 'f' now.

	while (!reader->getStream().eof()) {
		std::string line = reader->readLine();
		CArrayT<std::string> split_line = Util::Split(line, " ");

		if (split_line.size() > 0 && split_line[0] == "f") {
		}
	}

	// Return the reader to the beginning of the file
	reader->seek(0);
}

void CollisionFile::Write(std::experimental::filesystem::path file_name, FileType output_type) {
	if (mFaces.size() == 0) {
		std::cout << "Unable to write file - no data is loaded!" << std::endl;
		return;
	}

	bStream::CFileStream writer(file_name.generic_string(), bStream::Big, bStream::Out);

	if (!writer.getStream().is_open()) {
		std::cout << "Could not open file " << file_name << "!" << std::endl;
		return;
	}

	if (output_type == FileType::mp) {
		WriteToMp(&writer);
	}
	else {
		WriteToObj(&writer);
	}
}

void CollisionFile::WriteToObj(bStream::CFileStream * writer) {
	CArrayT<glm::vec3 *> vertices;
	CArrayT<std::string> faces;

	for (int i = 0; i < mFaces.size(); i++) {
		CArrayT<glm::vec3 *> face_verts = mFaces[i]->GetVertices();
		int face_indices[3] { 0,0,0 };

		for (int j = 0; j < 3; j++) {
			size_t index = 0;

			bool already_present = vertices.contains(face_verts[j], &index);

			if (!already_present) {
				face_indices[j] = vertices.size() + 1;
				vertices.append(face_verts[j]);
			}
			else {
				face_indices[j] = index + 1;
			}
		}

		char buff[100];
		snprintf(buff, sizeof(buff), "f %d %d %d\n", face_indices[0], face_indices[1], face_indices[2]);
		std::string buffAsStdStr = buff;
		faces.append(buff);
	}

	for (int i = 0; i < vertices.size(); i++) {
		char buff[100];
		snprintf(buff, sizeof(buff), "v %f %f %f\n", vertices[i]->x, vertices[i]->y, vertices[i]->z);
		std::string buffAsStdStr = buff;

		writer->writeString(buffAsStdStr);
	}

	for (int i = 0; i < faces.size(); i++) {
		writer->writeString(faces[i]);
	}
}

void CollisionFile::WriteToMp(bStream::CFileStream * writer) {

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