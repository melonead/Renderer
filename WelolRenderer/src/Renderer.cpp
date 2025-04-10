#include "Renderer.h"
#include <iostream>

namespace Welol {

	RenderOperation::RenderOperation(PrimitiveType shapeType, unsigned int numVertices,
		unsigned int offset, unsigned int numberOfInstance, bool instanced)
		: primitiveShapeType(shapeType),
		numberOfVertices(numVertices),
		vBufferOffset(offset),
		isInstanced(instanced)
	{

	}


	PrimitiveType RenderOperation::getPrimitiveShapeType()
	{
		return primitiveShapeType;
	}

	unsigned int RenderOperation::getVertexCount()
	{
		return numberOfVertices;
	}

	unsigned int RenderOperation::getOffset()
	{
		return vBufferOffset;
	}

	unsigned int RenderOperation::getInstances()
	{
		return numberOfInstances;
	}

	bool RenderOperation::getIsInstanced()
	{
		return isInstanced;
	}


	unsigned int& RenderOperation::getOperationID()
	{
		return operationID;
	}

	bool RenderOperation::isInitialized()
	{
		return initialized;
	}

	void RenderOperation::setInitialized(bool init)
	{
		initialized = init;
	}

	// unsigned int index, VertexDataType type, unsigned int typeCount
	void RenderOperation::addVertexAttribute(VertexAttribute& attribute)
	{
		attributes[attribute.getIndex()] = attribute;
	}

	RenderOperation::RenderOperationAttributes& RenderOperation::getAttributes()
	{
		return attributes;
	}


	// ---------------------------------------- vertex attribute

	VertexAttribute::VertexAttribute()
	{

	}

	VertexAttribute::VertexAttribute(unsigned int index, VertexDataType type, void* data_ptr, unsigned int verticesCount)
		: bindingIndex(index),
		  typeOfData(type),
		  dataPtr(data_ptr),
		  numberOfVertices(verticesCount)
	{

	}


	unsigned int VertexAttribute::getIndex()
	{
		return bindingIndex;
	}


	VertexDataType VertexAttribute::getTypeOfData()
	{
		return typeOfData;
	}




	// ---------------------------------------------------------



	Renderer::Renderer()
	{

	}

	void Renderer::render(RenderOperation& renderOperation)
	{

		activateRenderOperation(renderOperation);
		PrimitiveType shapeType = renderOperation.getPrimitiveShapeType();
		switch (shapeType)
		{
		case WL_TRIANGLES:
			glDrawArrays(GL_TRIANGLES, renderOperation.getOffset(), renderOperation.getVertexCount());
			break;
		case WL_LINES:
			glDrawArrays(GL_LINES, renderOperation.getOffset(), renderOperation.getVertexCount());
			break;
		case WL_POINTS:
			glDrawArrays(GL_POINTS, renderOperation.getOffset(), renderOperation.getVertexCount());
			break;
		default:
			break;
		}
		deactivateRenderOperation(renderOperation);
	}



	void Renderer::initializeRenderOperation(RenderOperation& renderOperation)
	{
		glGenVertexArrays(1, &renderOperation.getOperationID());
		renderOperation.setInitialized(true);
	}

	void Renderer::activateRenderOperation(RenderOperation& renderOperation)
	{
		glBindVertexArray(renderOperation.getOperationID());
	}

	void Renderer::deactivateRenderOperation(RenderOperation& renderOperation)
	{
		glBindVertexArray(0);
	}


	void Renderer::activateRenderOperationAttributes(RenderOperation& renderOperation)
	{


		if (!renderOperation.isInitialized())
		{
			std::cout << "This render operation has not been initialized." << std::endl;
			std::cout << "Initialize the render operation before trying to activate the attributes." << std::endl;
			return;
		}

		activateRenderOperation(renderOperation);
		for (auto& attribute : renderOperation.getAttributes())
		{
			VertexAttribute& va = attribute.second;
			unsigned int vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, va.getVertexCount() * getSizeOfDataType(va.getTypeOfData()), va.getDataPtr(), GL_STATIC_DRAW);
			//glBufferData(GL_ARRAY_BUFFER, getSizeOfDataType(va.getTypeOfData()), vertexPositions.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(va.getIndex(), getTypeCount(va.getTypeOfData()), getGlDataType(va.getTypeOfData()), GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(va.getIndex());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		deactivateRenderOperation(renderOperation);
	}
}
