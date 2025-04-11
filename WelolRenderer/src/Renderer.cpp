#include "Renderer.h"
#include <iostream>

namespace Welol {

	RenderOperation::RenderOperation(PrimitiveType shapeType, unsigned int numVertices,
		unsigned int offset, unsigned int numberOfInstance, bool instanced, bool ind)
		: primitiveShapeType(shapeType),
		  numberOfVertices(numVertices),
		  vBufferOffset(offset),
		  numberOfInstances(numberOfInstance),
		  
		  isInstanced(instanced),
		  isIndexed(ind)
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

	unsigned int RenderOperation::getInstanceCount()
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

	void RenderOperation::addVertexIndices(std::vector<unsigned int>& ind)
	{
		indices = ind;
	}



	// ---------------------------------------- vertex attribute

	VertexAttribute::VertexAttribute()
	{

	}

	VertexAttribute::VertexAttribute(unsigned int index, VertexDataType type, void* data_ptr, unsigned int verticesCount, bool instanced)
		: bindingIndex(index),
		  typeOfData(type),
		  dataPtr(data_ptr),
		  numberOfVertices(verticesCount),
		  isInstanced(instanced)
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


	bool VertexAttribute::getIsInstanced()
	{
		return isInstanced;
	}

	// ---------------------------------------------------------



	Renderer::Renderer()
	{

	}

	void Renderer::render(RenderOperation& renderOperation)
	{

		activateRenderOperation(renderOperation);
		PrimitiveType shapeType = renderOperation.getPrimitiveShapeType();
		GLenum shape;
		switch (shapeType)
		{
		case WL_TRIANGLES:
			shape = GL_TRIANGLES;
			break;
		case WL_LINES:
			shape = GL_LINES;
			break;
		case WL_POINTS:
			shape = GL_POINTS;
			break;
		default:
			std::cout << "Shape primitive has not been found" << std::endl;
			break;
		}

		bool isInst = renderOperation.getIsInstanced();
		bool isInd = renderOperation.getIsIndexed();

		if (isInd)
		{
			if (isInst)
			{
				glDrawElementsInstanced(shape, renderOperation.getVertexCount(), GL_UNSIGNED_INT, 0, renderOperation.getInstanceCount());
			}
			else 
			{
				glDrawElements(shape, renderOperation.getVertexCount(), GL_UNSIGNED_INT, 0);
			}
		}
		else 
		{
			if (isInst)
			{
				glDrawArraysInstanced(shape, renderOperation.getOffset(), renderOperation.getVertexCount(), renderOperation.getInstanceCount());
			}
			else
			{
				glDrawArrays(shape, renderOperation.getOffset(), renderOperation.getVertexCount());
			}
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

		std::vector<unsigned int> vertexIndices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
		};

		activateRenderOperation(renderOperation);
		for (auto& attribute : renderOperation.getAttributes())
		{
			VertexAttribute& va = attribute.second;
			unsigned int vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, va.getVertexCount() * getSizeOfDataType(va.getTypeOfData()), va.getDataPtr(), GL_STATIC_DRAW);


			glVertexAttribPointer(va.getIndex(), getTypeCount(va.getTypeOfData()), getGlDataType(va.getTypeOfData()), GL_FALSE, 0, nullptr);

			if (va.getIsInstanced())
			{
				glVertexAttribDivisor(va.getIndex(), 1);
			}

			glEnableVertexAttribArray(va.getIndex());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// REVISIT: Confirm if element buffer object can be initialized from here.

		if (renderOperation.getIsIndexed())
		{
			std::cout << "number of indices: " << renderOperation.getIndices().size() << std::endl;
			unsigned int ebo;
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * renderOperation.getVertexCount(), vertexIndices.data(), GL_STATIC_DRAW);
			// REVISIT: Should i clear this from here?
			//renderOperation.clearIndices();
		}

		
		deactivateRenderOperation(renderOperation);
	}
}
