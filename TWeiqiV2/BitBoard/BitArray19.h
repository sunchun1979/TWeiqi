#include <string>
#include <sstream>
#include <memory>

#pragma once

using namespace std;

class BitArray19
{
private:

	static const int m_bitLength = 361;
	static const int m_intLength = 12;

	int m_bits[12];

public:
	BitArray19()
	{
		memset(m_bits, 0, m_intLength * sizeof(int));
	}

	~BitArray19(void)
	{
	}

	//// Shallow copy good enough
	//// BitArray19 operator = (const BitArray19&);
	//// BitArray19(const BitArray19&);

	string ToString()
	{
		stringstream ss;
		for (int i = 0; i < m_intLength; i++)
		{
			ss << " " << m_bits[i];
		}
		return ss.str();
	}

	void SetAll(bool flag)
	{
		if (flag)
		{
			memset(m_bits, 0xFFFFFFFF, m_intLength * sizeof(int));
		}else
		{
			memset(m_bits, 0, m_intLength * sizeof(int));
		}
	}

	BitArray19 operator & (const BitArray19& others)
	{
		BitArray19 res(*this);
		for (int i = 0; i < m_intLength; i++)
		{
			res.m_bits[i] &= others.m_bits[i];
		}
		return res;
	}

	BitArray19 operator | (const BitArray19& others)
	{
		BitArray19 res(*this);
		for (int i = 0; i < m_intLength; i++)
		{
			res.m_bits[i] |= others.m_bits[i];
		}
		return res;
	}

	BitArray19 operator ^ (const BitArray19& others)
	{
		BitArray19 res(*this);
		for (int i = 0; i < m_intLength; i++)
		{
			res.m_bits[i] ^= others.m_bits[i];
		}
		return res;
	}

	void operator &= (const BitArray19& others)
	{
		for (int i = 0; i < m_intLength; i++)
		{
			m_bits[i] &= others.m_bits[i];
		}
	}

	void operator |= (const BitArray19& others)
	{
		for (int i = 0; i < m_intLength; i++)
		{
			m_bits[i] |= others.m_bits[i];
		}
	}

	void operator ^= (const BitArray19& others)
	{
		for (int i = 0; i < m_intLength; i++)
		{
			m_bits[i] ^= others.m_bits[i];
		}
	}

	void Set1(int index)
	{
		m_bits[index>>5] |= (1 << (index & 31));
	}

	void Set0(int index)
	{
		m_bits[index>>5] &= (~(1 << (index & 31)));
	}

	void Flip(int index)
	{
		m_bits[index>>5] ^= (1 << (index & 31));
	}

	bool Get(int index)
	{
		return (m_bits[index >> 5] & (1 << (index & 31))) != 0;
	}
};

