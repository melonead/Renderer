#pragma once

#include "shader.h"
#include <glad/glad.h>
#include <vector>
#include <map>
#include <iostream>

namespace Welol {

	enum PrimitiveType : unsigned int
	{
		WL_TRIANGLES = 0,
		WL_POINTS = 1,
		WL_LINES = 2,
		WL_LINE_LOOP = 3,
		WL_TRIANGLE_STRIP = 4
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
		VertexAttribute() = default;

		/*
			verticeCount - the number of vertices for this particular attribute NOT the number of vertices
			to be rendered.
		*/
		VertexAttribute(
			unsigned int index, 
			VertexDataType type, 
			void* data_ptr, 
			unsigned int verticesCountInBuffer,			
			bool instanced
		);
	
		unsigned int getIndex();
		VertexDataType getTypeOfData();
		void* getDataPtr() { return dataPtr; };
		/*unsigned int getVertexCount() { return numberOfVertices; };*/
		bool getIsInstanced();
		unsigned int getSizeOfBuffer() {return bufferSize; }
		unsigned int& getBufferID() {return bufferID;}
		void setBufferID(unsigned int id) { bufferID = id;}
	private:
		unsigned int bindingIndex;
		VertexDataType typeOfData;
		void* dataPtr;
		bool isInstanced;
		unsigned int bufferSize;
		unsigned int bufferID;
	};


	class RenderOperation
	{
	public:

		typedef std::vector<unsigned int> VertexIndices;
		typedef std::map<unsigned int, VertexAttribute> RenderOperationAttributes;

		/*
			numVerticesToRender - The number of indices that determine how many triangles we render.
		*/


		RenderOperation(
			PrimitiveType shapeType,  
			unsigned int numVerticesToRender, 
			unsigned int offset, 
			unsigned int numberOfInstances, 
			bool instanced, 
			bool ind
		);
		RenderOperation() = default;

		PrimitiveType getPrimitiveShapeType();
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

		void reduceInstancesToRender(unsigned int count)
		{
			numberOfInstances -= count;

		}

		void setNumberOfInstancesToRender(unsigned int count)
		{
			numberOfInstances = count;
		}

		void setVerticesToRender(unsigned int count)
		{
			verticesToRender = count;
		}

		unsigned int getVerticesToRender()
		{
			return verticesToRender;
		}

	private:
		RenderOperationAttributes attributes;
		VertexIndices indices;
		PrimitiveType primitiveShapeType;
		unsigned int verticesToRender;
		unsigned int vBufferOffset;
		unsigned int numberOfInstances;
		bool isInstanced;
		bool initialized{ false };
		unsigned int operationID{ 0 };
		bool isIndexed;
	};

	/*
		This is the class that all the OpenGl calls are called. Localize the GL call to this class so that
		the other classes can be used by other graphics apis.
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

		/*
			Change existing data in the buffer. No additional data added.
		*/
		void updateRenderOperationVertexAttribute(RenderOperation& renderOperation, unsigned int index, unsigned int offset, void* data);

		/*
			Add data to an over allocated buffer.
		*/
		void addSizeBytesToBuffer(RenderOperation& renderOperation, unsigned int index, unsigned int offset, unsigned int size, void* data);


		void _activateRenderOperation(RenderOperation& renderOperation);
		void _deactivateRenderOperation(RenderOperation& renderOperation);

		void _activateRenderOperationAttributes(RenderOperation& renderOperation);
	private:
	};

	

}
