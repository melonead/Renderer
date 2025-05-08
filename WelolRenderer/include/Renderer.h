#pragma once

#include "shader.h"
#include <glad/glad.h>
#include <vector>
#include <map>

namespace Welol {

	enum PrimitiveType : unsigned int
	{
		WL_TRIANGLES = 0,
		WL_POINTS = 1,
		WL_LINES = 2,
		WL_LINE_LOOP = 3
	};

	enum VertexDataType : unsigned int
	{
		WL_FLOAT = 0,
		WL_FLOAT2 = 1, 
		WL_FLOAT3 = 2,
		WL_FLOAT4 = 3
	};

	// REVISIT: can a lone function compile without the static keyword?

	static GLenum getGlDataType(VertexDataType type)
	{
		switch (type)
		{
		case WL_FLOAT:
		case WL_FLOAT2:
		case WL_FLOAT3:
		case WL_FLOAT4:
			return GL_FLOAT;
		default:
			return 0;
		}
	}

	static size_t getSizeOfDataType(VertexDataType type)
	{
		switch (type)
		{
		case WL_FLOAT:
			return sizeof(float);
		case WL_FLOAT2:
			return sizeof(float) * 2;
		case WL_FLOAT3:
			return sizeof(float) * 3;
		case WL_FLOAT4:
			return sizeof(float) * 4;
		default:
			return 0;
		}
	}

	static unsigned int getTypeCount(VertexDataType type)
	{
		switch (type)
		{
		case WL_FLOAT:
			return 1;
		case WL_FLOAT2:
			return 2;
		case WL_FLOAT3:
			return 3;
		case WL_FLOAT4:
			return 4;
		default:
			return 0;
		}
	}

	/*
		Instanced Arrays vertex attributes carry data unique per instance while standard vertex attributes
		carry data unique per vertex.
	*/

	// REVISIT: what should the lifetime of this object be? 
	// should I destroy it immediately after the attribute is
	// recorded in the render operations?

	// Should this class a reference to the data?
	class VertexAttribute
	{
	public:
		VertexAttribute();
		VertexAttribute(unsigned int index, VertexDataType type, void* data_ptr, unsigned int verticesCount, bool instanced);
		unsigned int getIndex();
		VertexDataType getTypeOfData();
		void* getDataPtr() { return dataPtr; };
		unsigned int getVertexCount() { return numberOfVertices; };
		bool getIsInstanced();
		void setSizeOfData(unsigned int size) {sizeOfDataInBytes = size; };
		unsigned int getSizeOfData() {return sizeOfDataInBytes; }
		unsigned int& getBufferID() {return bufferID;}
		void setBufferID(unsigned int id) { bufferID = id;}
	private:
		unsigned int bindingIndex;
		VertexDataType typeOfData;
		void* dataPtr;
		unsigned int numberOfVertices;
		bool isInstanced;
		unsigned int sizeOfDataInBytes;
		unsigned int bufferID;
	};

	typedef std::vector<unsigned int> VertexIndices;

	class RenderOperation
	{
	public:

		typedef std::map<unsigned int, VertexAttribute> RenderOperationAttributes;
		RenderOperation(PrimitiveType shapeType, unsigned int numVertices,
			unsigned int offset, unsigned int numberOfInstances, bool instanced, bool ind);
		RenderOperation() = default;

		PrimitiveType getPrimitiveShapeType();
		unsigned int getVertexCount();
		unsigned int getOffset();
		unsigned int getInstanceCount();
		bool getIsInstanced();
		unsigned int& getOperationID();
		bool isInitialized();
		void setInitialized(bool init);

		void addVertexAttribute(VertexAttribute& attribute);
		RenderOperationAttributes& getAttributes();
		VertexAttribute& getAttribute(unsigned int index) {
			return attributes[index];
		};

		bool getIsIndexed() { return isIndexed; };
		void addVertexIndices(std::vector<unsigned int>& ind);
		VertexIndices& getIndices() { return indices; };
		void clearIndices() { indices.clear(); };
		void updateVertexAttributeData(unsigned int index);

	private:
		RenderOperationAttributes attributes;
		VertexIndices indices;
		PrimitiveType primitiveShapeType;
		unsigned int numberOfVertices;
		unsigned int vBufferOffset;
		unsigned int numberOfInstances;
		bool isInstanced;
		bool initialized{ false };
		unsigned int operationID{ 0 };
		bool isIndexed;
	};

	/*
		This is the class that all the OpenGl calls are called.
	*/


	class Renderer
	{
	public:
		Renderer();

		// Opengl draw call.
		void render(RenderOperation& renderOperation);
		void useCurrentShaderProgram();
		void createRenderOperation();
		void setShaderProgram(Shader& shaderProgram);
		void setRenderOperation(RenderOperation& renderOperation);
		void initializeRenderOperation(RenderOperation& renderOperation);

		// Update vertex attribute's data when it changes in memory.
		void updateRenderOperationVertexAttribute(RenderOperation& renderOperation, unsigned int index, unsigned int offset, void* data);


		void _activateRenderOperation(RenderOperation& renderOperation);
		void _deactivateRenderOperation(RenderOperation& renderOperation);

		void _activateRenderOperationAttributes(RenderOperation& renderOperation);
	private:
	};

	

}
