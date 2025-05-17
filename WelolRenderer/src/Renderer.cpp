#include "Renderer.h"
#include <iostream>

namespace Welol {

	/*
	*   A render operation could represent a single mesh rendering.
	* 
		A render operation is done using:
			- single draw call
			- multiple vertex data representing the variaous attributes (e.g position, color)
			- can be instanced
				- draw multiple objects using a single draw call
			- indexed
				- using indices to save on the number of vertex data (e.g position) that need to be sent to the gpu
	*/

	RenderOperation::RenderOperation(PrimitiveType shapeType, unsigned int numVertices,
		unsigned int offset, unsigned int numberOfInstances, bool instanced, bool ind)
		: primitiveShapeType(shapeType),
		  numberOfVertices(numVertices),
		  vBufferOffset(offset),
		  numberOfInstances(numberOfInstances),
		  
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



	/*
	* A vertex attribute represent an attribute of a vertex data.
	* For example position attribute, color attribute.
	* A vertex attribute has:
	*	- binding index for the gpu/shader program to know where to find it.
	*
	*/

	VertexAttribute::VertexAttribute()
	{

	}

	VertexAttribute::VertexAttribute(unsigned int index, VertexDataType type, void* data_ptr, unsigned int verticesCount, bool instanced)
		: bindingIndex(index),
		typeOfData(type),
		dataPtr(data_ptr),
		numberOfVertices(verticesCount),
		isInstanced(instanced),
		sizeOfDataInBytes(getSizeOfDataType(type) * verticesCount)
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

	/*
	* The renderer houses all the direct openGL calls.
	* It uses a render operation to get the data it need to for a draw call.
	*/

	Renderer::Renderer()
	{

	}

	void Renderer::render(RenderOperation& renderOperation)
	{

		_activateRenderOperation(renderOperation);
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
		case WL_LINE_LOOP:
			shape = GL_LINE_LOOP;
			break;
		case WL_TRIANGLE_STRIP:
			shape = GL_TRIANGLE_STRIP;
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
		_deactivateRenderOperation(renderOperation);
	}



	void Renderer::initializeRenderOperation(RenderOperation& renderOperation)
	{
		glGenVertexArrays(1, &renderOperation.getOperationID());
		renderOperation.setInitialized(true);

		// 
		_activateRenderOperation(renderOperation);
		_activateRenderOperationAttributes(renderOperation);
	}

	void Renderer::_activateRenderOperation(RenderOperation& renderOperation)
	{
		glBindVertexArray(renderOperation.getOperationID());
	}

	void Renderer::_deactivateRenderOperation(RenderOperation& renderOperation)
	{
		glBindVertexArray(0);
	}


	void Renderer::_activateRenderOperationAttributes(RenderOperation& renderOperation)
	{


		if (!renderOperation.isInitialized())
		{
			std::cout << "This render operation has not been initialized." << std::endl;
			std::cout << "Initialize the render operation before trying to activate the attributes." << std::endl;
			return;
		}


		_activateRenderOperation(renderOperation);
		for (auto& attribute : renderOperation.getAttributes())
		{
			VertexAttribute& va = attribute.second;

			unsigned int vbo;
			glGenBuffers(1, &vbo);
			va.setBufferID(vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, va.getSizeOfData(), va.getDataPtr(), GL_STATIC_DRAW);


			glVertexAttribPointer(va.getIndex(), getTypeCount(va.getTypeOfData()), getGlDataType(va.getTypeOfData()), GL_FALSE, 0, nullptr);

			// REVISIT: Add instance rate to vertex attribute, for now we assume instanced attributes are
			// going to be repeated at every vertex shader program run on the GPU.
			// glVertexAttribDivisor(va.getIndex(), instanceRate);
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
			unsigned int ebo;
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * renderOperation.getVertexCount(), renderOperation.getIndices().data(), GL_STATIC_DRAW);
			// REVISIT: Should i clear this from here?
			renderOperation.clearIndices();
		}

		
		_deactivateRenderOperation(renderOperation);
	}

	void Renderer::updateRenderOperationVertexAttribute(RenderOperation& renderOperation, unsigned int index, unsigned int offset, void* data)
	{
		VertexAttribute& va = renderOperation.getAttribute(index);
		glBindBuffer(GL_ARRAY_BUFFER, va.getBufferID());
		glBufferSubData(GL_ARRAY_BUFFER, offset, va.getSizeOfData(), data);
	}

	void Renderer::addSizeBytesToBuffer(RenderOperation& renderOperation, unsigned int index, unsigned int offset, unsigned int size, void* data)
	{
		VertexAttribute& va = renderOperation.getAttribute(index);
		assert(size <= va.getSizeOfData() && "The number of bytes to be written is larger than the size of the buffer.");
		glBindBuffer(GL_ARRAY_BUFFER, va.getBufferID());
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}
}
