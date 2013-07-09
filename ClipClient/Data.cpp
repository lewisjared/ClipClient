#include "stdafx.h"
#include "Data.h"


Data::Data(void)
	:m_data(NULL), m_size(0)
{
}

Data::Data(void* data, size_t n)
{
	m_data = std::shared_ptr<void>(operator new(n));
	memcpy(m_data.get(),data, n);
	m_size = n;
}


Data::~Data(void)
{
}

arb_ptr Data::deflate()
{
	return m_deflated;
}

void Data::inflate(void* data, size_t n)
{

}