#include <bitset>
#include <string>
#include <sstream>
#include <memory>

#pragma once

using namespace std;

class BitArray64Base
{
private:
	BitArray64Base();
protected:

	int m_bitLength;
	int m_int64Length;
	int m_lastInt64Mask;
	int m_bitInt64Diff;

	uint64_t *m_bits;

public:
	BitArray64Base(int bitLength, int intLength, uint64_t* bitMem) :
		m_bitLength(bitLength),
		m_int64Length(intLength),
		m_bits(bitMem)
	{
		memset(m_bits, 0, m_int64Length * sizeof(uint64_t));
		m_bitInt64Diff = m_int64Length * sizeof(uint64_t) * 8 - m_bitLength;
		m_lastInt64Mask = (0xFFFFFFFFFFFFFFFF >> m_bitInt64Diff);
	}

	~BitArray64Base(void)
	{
	}

	//// Shallow copy good enough
	//// BitArray64Base operator = (const BitArray64Base&);
	//// BitArray64Base(const BitArray64Base&);

	string ToString()
	{
		stringstream ss;
		for (int i = 0; i < m_int64Length; i++)
		{
			ss << " " << m_bits[i];
		}
		return ss.str();
	}

	string ToBinaryString()
	{
		stringstream ss;
		for (int i = 0; i < m_int64Length; i++)
		{
			ss << bitset<64>(m_bits[i]);
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
			memset(m_bits, 0xFFFFFFFF, m_int64Length * sizeof(uint64_t));
		}else
		{
			memset(m_bits, 0, m_int64Length * sizeof(uint64_t));
		}
	}

	bool operator == (const BitArray64Base& other)
	{
		return memcmp(m_bits, other.m_bits, m_int64Length * sizeof(uint64_t)) == 0;
	}

	void operator &= (const BitArray64Base& other)
	{
		for (int i = m_int64Length; i-- > 0;)
		{
			m_bits[i] &= other.m_bits[i];
		}
	}

	void operator |= (const BitArray64Base& other)
	{
		for (int i = m_int64Length; i-- > 0;)
		{
			m_bits[i] |= other.m_bits[i];
		}
	}

	void operator ^= (const BitArray64Base& other)
	{
		for (int i = m_int64Length; i-- > 0;)
		{
			m_bits[i] ^= other.m_bits[i];
		}
	}

	bool Intersects(const BitArray64Base& other)
	{
		for (int i = m_int64Length; i-- > 0;)
		{
			if ((m_bits[i] & other.m_bits[i]) != 0)
			{
				return true;
			}
		}
		return false;
	}

	void XorTrue(const BitArray64Base& other)
	{
		for (int i = m_int64Length; i-- > 0;)
		{
			m_bits[i] &= ( m_bits[i] ^ other.m_bits[i]);
		}
	}

	bool XorTrueCheck(const BitArray64Base& other)
	{
		for (int i = m_int64Length; i-- > 0;)
		{
			if ((m_bits[i] & ( m_bits[i] ^ other.m_bits[i])) != 0)
			{
				return true;
			}
		}
		return false;
	}

	void Set1(uint64_t index)
	{
		m_bits[index>>6] |= (1i64 << (index & 63UL));
	}

	void Set0(int index)
	{
		m_bits[index>>6] &= (~(1i64 << (index & 63UL)));
	}

	void Flip(int index)
	{
		m_bits[index>>6] ^= (1i64 << (index & 63UL));
	}

	bool Get(int index)
	{
		return (m_bits[index >> 6] & (1i64 << (index & 63UL))) != 0;
	}

	bool HasTrue()
	{
		for (int i = m_int64Length; i-- > 0;)
		{
			if (m_bits[i] != 0)
			{
				return true;
			}
		}
		return ((m_bits[m_int64Length - 1] & m_lastInt64Mask) != 0);
	}

	bool HasFalse()
	{
		for (int i = m_int64Length; i-- > 0;)
		{
			if (m_bits[i] != 0xFFFFFFFFFFFFFFFF)
			{
				return true;
			}
		}
		return (((~m_bits[m_int64Length - 1]) & m_lastInt64Mask) != 0);
	}

	int GetNumEmptyPositions()
	{
		int ret = 0;
		for (int i = 0; i < m_int64Length; i++)
		{
			int c;
			uint64_t v = m_bits[i];
			v = v - ((v >> 1) & 0x5555555555555555UL);
			v = (v & 0x3333333333333333UL) + ((v >> 2) & 0x3333333333333333UL);
			c = (int)(((v + (v >> 4) & 0xF0F0F0F0F0F0F0FUL) * 0x101010101010101UL) >> 56);
			ret += c;
		}
		return ret;
	}
};

template <int N>
class BitArray64 : public BitArray64Base
{
private:
	uint64_t m_myBits[((N*N)>>6)+1];
public:
	BitArray64() : BitArray64Base(N*N, ((N*N)>>6)+1, m_myBits)
	{
	}
	~BitArray64()
	{
	}
	BitArray64(const BitArray64& other) : BitArray64Base(other)
	{
		*this = other;
	}

	BitArray64& operator = (const BitArray64& other)
	{
		BitArray64Base::operator=(other);
		memcpy(m_myBits, other.m_myBits, sizeof(uint64_t)*(((N*N)>>6)+1));
		m_bits = m_myBits;
		return *this;
	}

	void operator &= (const BitArray64& other)
	{
		BitArray64Base::operator&=(other);
	}

	void operator |= (const BitArray64& other)
	{
		BitArray64Base::operator|=(other);
	}

	void operator ^= (const BitArray64& other)
	{
		BitArray64Base::operator^=(other);
	}

	bool operator == (const BitArray64& other)
	{
		return BitArray64Base::operator == (m_bits, other.m_bits);
	}
};