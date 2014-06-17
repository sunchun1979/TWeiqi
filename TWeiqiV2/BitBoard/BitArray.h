#include <bitset>
#include <string>
#include <sstream>
#include <memory>

#pragma once

using namespace std;

class BitArrayBase
{
private:
	BitArrayBase();

protected:

	int m_bitLength;
	int m_intLength;
	int m_lastIntMask;

	int *m_bits;

public:
	BitArrayBase(int bitLength, int intLength, int* bitMem) :
		m_bitLength(bitLength),
		m_intLength(intLength),
		m_bits(bitMem)
	{
		memset(m_bits, 0, m_intLength * sizeof(int));
		m_lastIntMask = (0xFFFFFFFF << (m_bitLength & 31));
	}

	~BitArrayBase(void)
	{
	}

	//// Shallow copy good enough
	//// BitArrayBase operator = (const BitArrayBase&);
	//// BitArrayBase(const BitArrayBase&);

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

	bool operator == (const BitArrayBase& other)
	{
		return memcmp(m_bits, other.m_bits, m_intLength) == 0;
	}

	void operator &= (const BitArrayBase& other)
	{
		for (int i = 0; i < m_intLength; i++)
		{
			m_bits[i] &= other.m_bits[i];
		}
	}

	void operator |= (const BitArrayBase& other)
	{
		for (int i = 0; i < m_intLength; i++)
		{
			m_bits[i] |= other.m_bits[i];
		}
	}

	void operator ^= (const BitArrayBase& other)
	{
		for (int i = 0; i < m_intLength; i++)
		{
			m_bits[i] ^= other.m_bits[i];
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

	bool HasTrue()
	{
		for (int i = 0; i < m_intLength - 1; i++)
		{
			if (m_bits[i] != 0)
			{
				return true;
			}
		}
		return ((m_bits[m_intLength - 1] & m_lastIntMask) != 0);
	}

	bool HasFalse()
	{
		for (int i = 0; i < m_intLength - 1; i++)
		{
			if (m_bits[i] != 0xFFFFFFFF)
			{
				return true;
			}
		}
		return (((~m_bits[m_intLength - 1]) & m_lastIntMask) != 0);
	}
};

class BitArray19 : public BitArrayBase
{
private:
	int m_myBits[12];

public:
	BitArray19() : BitArrayBase(361, 12, m_myBits)
	{
	}
	~BitArray19()
	{
	}

	BitArray19(const BitArray19& other) : BitArrayBase(other)
	{
		*this = other;
	}

	BitArray19& operator = (const BitArray19& other)
	{
		BitArrayBase::operator=(other);
		memcpy(m_myBits, other.m_myBits, sizeof(int)*12);
		m_bits = m_myBits;
		return *this;
	}

	void operator &= (const BitArray19& other)
	{
		BitArrayBase::operator&=(other);
	}

	void operator |= (const BitArray19& other)
	{
		BitArrayBase::operator|=(other);
	}

	void operator ^= (const BitArray19& other)
	{
		BitArrayBase::operator^=(other);
	}

	bool operator == (const BitArray19& other)
	{
		return memcmp(m_bits, other.m_bits, m_intLength) == 0;
	}
};

class BitArray9 : public BitArrayBase
{
private:
	int m_myBits[3];

public:
	BitArray9() : BitArrayBase(81, 3, m_myBits)
	{
	}
	~BitArray9()
	{
	}

	BitArray9(const BitArray9& other) : BitArrayBase(other)
	{
		*this = other;
	}

	BitArray9& operator = (const BitArray9& other)
	{
		BitArrayBase::operator=(other);
		memcpy(m_myBits, other.m_myBits, sizeof(int)*3);
		m_bits = m_myBits;
		return *this;
	}

	void operator &= (const BitArray9& other)
	{
		BitArrayBase::operator&=(other);
	}

	void operator |= (const BitArray9& other)
	{
		BitArrayBase::operator|=(other);
	}

	void operator ^= (const BitArray9& other)
	{
		BitArrayBase::operator^=(other);
	}

	bool operator == (const BitArray9& other)
	{
		return memcmp(m_bits, other.m_bits, m_intLength) == 0;
	}
};
