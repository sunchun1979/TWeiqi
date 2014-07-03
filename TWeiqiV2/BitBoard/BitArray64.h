#include <bitset>
#include <string>
#include <stdint.h>
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
	uint64_t m_lastInt64Mask;
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

	virtual ~BitArray64Base()
	{
	}

	//// Shallow copy good enough
	//// BitArray64Base operator = (const BitArray64Base&);
	//// BitArray64Base(const BitArray64Base&);

	string ToString()
	{
		stringstream ss;
		for (int i = 0; i < m_int64Length - 1; i++)
		{
			ss << m_bits[i] << ", ";
		}
		ss << m_bits[m_int64Length - 1];
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

	const uint64_t* GetArray() const
	{
		return m_bits;
	}

	const int GetArrayLength() const
	{
		return m_int64Length;
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

	void operator |= (const uint64_t* other)
	{
		for (int i = m_int64Length; i-- > 0;)
		{
			m_bits[i] |= other[i];
		}
	}

	void operator ^= (const BitArray64Base& other)
	{
		for (int i = m_int64Length; i-- > 0;)
		{
			m_bits[i] ^= other.m_bits[i];
		}
	}

	bool Intersects(const BitArray64Base& other) const
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
		m_bits[index>>6] |= (1i64 << (index & 63));
	}

	void Set0(int index)
	{
		m_bits[index>>6] &= (~(1i64 << (index & 63)));
	}

	void Flip(int index)
	{
		m_bits[index>>6] ^= (1i64 << (index & 63));
	}

	bool Get(int index)
	{
		return (m_bits[index >> 6] & (1i64 << (index & 63))) != 0;
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

	int GetNumOfOnes()
	{
		int ret = 0;
		ret += GetOneEach(m_bits[m_int64Length - 1] & m_lastInt64Mask);
		for (int i = m_int64Length-1; i-- > 0;)
		{
			ret += GetOneEach(m_bits[i]);
		}
		return ret;
	}

	int GetNthOne(int r0)
	{
		int count = 0;
		int current = 0;
		int p = 0;
		do
		{
			current = GetOneEach(m_bits[p++]);
			count += current;
		} while (count < r0 + 1);
		return (p<<6) - GetNthOneEach(m_bits[p-1], count - r0);
	}

	static int GetNthOneEach(uint64_t v, int r)
	{
		//uint64_t v;          // Input value to find position with rank r.
		//unsigned int r;      // Input: bit's desired rank [1-64].
		unsigned int s;      // Output: Resulting position of bit with rank r [1-64]
		uint64_t a, b, c, d; // Intermediate temporaries for bit count.
		unsigned int t;      // Bit count temporary.

		// Do a normal parallel bit count for a 64-bit integer,                     
		// but store all intermediate steps.                                        
		// a = (v & 0x5555...) + ((v >> 1) & 0x5555...);
		a =  v - ((v >> 1) & ~0ULL/3);
		// b = (a & 0x3333...) + ((a >> 2) & 0x3333...);
		b = (a & ~0ULL/5) + ((a >> 2) & ~0ULL/5);
		// c = (b & 0x0f0f...) + ((b >> 4) & 0x0f0f...);
		c = (b + (b >> 4)) & ~0ULL/0x11;
		// d = (c & 0x00ff...) + ((c >> 8) & 0x00ff...);
		d = (c + (c >> 8)) & ~0ULL/0x101;
		t = (d >> 32) + (d >> 48);
		// Now do branchless select!                                                
		s  = 64;
		// if (r > t) {s -= 32; r -= t;}
		s -= ((t - r) & 256) >> 3; r -= (t & ((t - r) >> 8));
		t  = (d >> (s - 16)) & 0xff;
		// if (r > t) {s -= 16; r -= t;}
		s -= ((t - r) & 256) >> 4; r -= (t & ((t - r) >> 8));
		t  = (c >> (s - 8)) & 0xf;
		// if (r > t) {s -= 8; r -= t;}
		s -= ((t - r) & 256) >> 5; r -= (t & ((t - r) >> 8));
		t  = (b >> (s - 4)) & 0x7;
		// if (r > t) {s -= 4; r -= t;}
		s -= ((t - r) & 256) >> 6; r -= (t & ((t - r) >> 8));
		t  = (a >> (s - 2)) & 0x3;
		// if (r > t) {s -= 2; r -= t;}
		s -= ((t - r) & 256) >> 7; r -= (t & ((t - r) >> 8));
		t  = (v >> (s - 1)) & 0x1;
		// if (r > t) s--;
		s -= ((t - r) & 256) >> 8;
		s = 65 - s;
		return s;
	}

	int GetOneEach(uint64_t v)
	{
		v = v - ((v >> 1) & 0x5555555555555555);
		v = (v & 0x3333333333333333) + ((v >> 2) & 0x3333333333333333);
		return (int)(((v + (v >> 4) & 0xF0F0F0F0F0F0F0F) * 0x101010101010101) >> 56);
	}
};

template <int N>
class BitArray64 : public BitArray64Base
{
private:
	uint64_t m_myBits[((N*N)>>6)+1];
	//vector<uint64_t> m_myBits(((N*N)>>6)+1);
public:
	BitArray64() : BitArray64Base(N*N, ((N*N)>>6)+1, &m_myBits[0])
	{
	}

	BitArray64(uint64_t* t) : BitArray64Base(N*N, ((N*N)>>6)+1, &m_myBits[0])
	{
		memcpy(&m_myBits[0], t, sizeof(uint64_t)*(((N*N)>>6)+1));
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
		memcpy(&m_myBits[0], &other.m_myBits[0], sizeof(uint64_t)*(((N*N)>>6)+1));
		m_bits = &m_myBits[0];
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

	void operator |= (const uint64_t* other)
	{
		BitArray64Base::operator|=(other);
	}

	void operator ^= (const BitArray64& other)
	{
		BitArray64Base::operator^=(other);
	}

	bool operator == (const BitArray64& other)
	{
		return BitArray64Base::operator == (other);
	}
};