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
		WL_LINES = 2
	};

	enum VertexDataType : unsigned int
	{
		WL_FLOAT = 0,
		WL_FLOAT2 = 1, 
		WL_FLOAT3 = 2
	};

	// REVISIT: can a lone function compile without the static keyword?

	static GLenum getGlDataType(VertexDataType type)
	{
		switch (type)
		{
		case WL_FLOAT:
		case WL_FLOAT2:
		case WL_FLOAT3:
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
		default:
			return 0;
		}
	}

	// REVISIT: what should the lifetime of this object be? 
	// should I destroy it immediately after the attribute is
	// recorded in the render operations?

	// Should this class a reference to the data?
	class VertexAttribute
	{
	public:
		VertexAttribute();
		VertexAttribute(unsigned int index, VertexDataType type, void* data_ptr, unsigned int verticesCount);
		unsigned int getIndex();
		VertexDataType getTypeOfData();
		void* getDataPtr() { return dataPtr; };
		unsigned int getVertexCount() { return numberOfVertices; };
	private:
		unsigned int bindingIndex;
		VertexDataType typeOfData;
		void* dataPtr;
		unsigned int numberOfVertices;
	};

	class RenderOperation
	{
	public:

		typedef std::map<unsigned int, VertexAttribute> RenderOperationAttributes;
		RenderOperation(PrimitiveType shapeType, unsigned int numVertices,
			unsigned int offset, unsigned int numberOfInstance, bool instanced);

		PrimitiveType getPrimitiveShapeType();
		unsigned int getVertexCount();
		unsigned int getOffset();
		unsigned int getInstances();
		bool getIsInstanced();
		unsigned int& getOperationID();
		bool isInitialized();
		void setInitialized(bool init);

		void addVertexAttribute(VertexAttribute& attribute);
		RenderOperationAttributes& getAttributes();

	private:
		RenderOperationAttributes attributes;
		PrimitiveType primitiveShapeType;
		unsigned int numberOfVertices;
		unsigned int vBufferOffset;
		unsigned int numberOfInstances;
		bool isInstanced;
		bool initialized{ false };
		unsigned int operationID{ 0 };
	};


	class Renderer
	{
	public:
		Renderer();
		void render(RenderOperation& renderOperation);
		void useCurrentShaderProgram();
		void createRenderOperation();
		void setShaderProgram(Shader& shaderProgram);
		void setRenderOperation(RenderOperation& renderOperation);
		void initializeRenderOperation(RenderOperation& renderOperation);


		void activateRenderOperation(RenderOperation& renderOperation);
		void deactivateRenderOperation(RenderOperation& renderOperation);

		void activateRenderOperationAttributes(RenderOperation& renderOperation);
	private:
	};

	

}