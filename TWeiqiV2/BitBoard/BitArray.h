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
	int m_bitIntDiff;

	int *m_bits;

public:
	BitArrayBase(int bitLength, int intLength, int* bitMem) :
		m_bitLength(bitLength),
		m_intLength(intLength),
		m_bits(bitMem)
	{
		memset(m_bits, 0, m_intLength * sizeof(int));
		m_bitIntDiff = m_intLength * sizeof(int) * 8 - m_bitLength;
		m_lastIntMask = (0xFFFFFFFF >> m_bitIntDiff);
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

	string ToBinaryString()
	{
		stringstream ss;
		for (int i = 0; i < m_intLength; i++)
		{
			ss << bitset<32>(m_bits[i]);
		}
		return ss.str();
	}

	string ToPositionString()
	{
		stringstream ss;
		for (int i = 0; i < m_bitLength; i++)
		{
			if (Get(i))
			{
				ss << i << " ";
			}
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

	bool Intersects(const BitArrayBase& other)
	{
		for (int i = 0; i < m_intLength; i++)
		{
			if ((m_bits[i] & other.m_bits[i]) != 0)
			{
				return true;
			}
		}
		return false;
	}

	void XorTrue(const BitArrayBase& other)
	{
		for (int i = 0; i < m_intLength; i++)
		{
			m_bits[i] &= ( m_bits[i] ^ other.m_bits[i]);
		}
	}

	bool XorTrueCheck(const BitArrayBase& other)
	{
		for (int i = 0; i < m_intLength; i++)
		{
			if ((m_bits[i] & ( m_bits[i] ^ other.m_bits[i])) != 0)
			{
				return true;
			}
		}
		return false;
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

	int GetNumEmptyPositions()
	{
		int ret = 0;
		for (int i = 0; i < m_intLength; i++)
		{
			int c;
			int v = m_bits[i];
			v = v - ((v >> 1) & 0x55555555);                    // reuse input as temporary
			v = (v & 0x33333333) + ((v >> 2) & 0x33333333);     // temp
			c = ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
			ret += c;
		}
		return ret - m_bitIntDiff;
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

class BitArray5 : public BitArrayBase
{
private:
	int m_myBits[1];

public:
	BitArray5() : BitArrayBase(25, 1, m_myBits)
	{
	}
	~BitArray5()
	{
	}

	BitArray5(const BitArray5& other) : BitArrayBase(other)
	{
		*this = other;
	}

	BitArray5& operator = (const BitArray5& other)
	{
		BitArrayBase::operator=(other);
		memcpy(m_myBits, other.m_myBits, sizeof(int)*1);
		m_bits = m_myBits;
		return *this;
	}

	void operator &= (const BitArray5& other)
	{
		BitArrayBase::operator&=(other);
	}

	void operator |= (const BitArray5& other)
	{
		BitArrayBase::operator|=(other);
	}

	void operator ^= (const BitArray5& other)
	{
		BitArrayBase::operator^=(other);
	}

	bool operator == (const BitArray5& other)
	{
		return memcmp(m_bits, other.m_bits, m_intLength) == 0;
	}
};
