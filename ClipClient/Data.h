#pragma once

#include <memory>

/**
 \class	Data

 \brief	Represents a data structure.
 
 The Data structure is used to send and recieve data over a stream.
 The data can represent a file or a serialisable stream and is the applications
 responsibility to interpert the data format. The data can be inflated and 
 deflated as required.

 This class shares a pointer to the same data. This ensures that excessive data
 copies do not occur.

 \date	7/3/2013
 */
 
typedef std::shared_ptr<void> arb_ptr;

class Data
{
public:
	Data(void);

	/**
	 \fn	Data::Data(const Data& other);
	
	 \brief	Copy constructor.
	
	 \date	7/4/2013
	
	 \param	other	The other object.
	 */
	//Data(Data& other);
	Data(void* data, size_t n);
	~Data(void);

	arb_ptr deflate();

	size_t getDeflatedSize() { return m_deflatedSize;};
	size_t getSize() { return m_size;};
	arb_ptr getData() {return m_data;};

	/**
	 \fn	void Data::inflate(void* data, size_t n);
	
	 \brief	Inflates a byte array.
	
	 \date	7/3/2013
	
	 \param [in,out]	data	The input data
	 \param	n					The size of the input data
	 */

	void inflate(void* data, size_t n);

private:
	arb_ptr m_deflated;
	arb_ptr m_data;
	size_t m_deflatedSize;
	size_t m_size;
};

