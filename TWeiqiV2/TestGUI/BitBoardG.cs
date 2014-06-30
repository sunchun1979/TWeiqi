using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestGUI
{
    class BitBoardG
    {
        private List<Tuple<int, int>> _positions;
        private int _size;
        
        public BitBoardG(int size)
        {
            _size = size;
            _positions = new List<Tuple<int, int>>();
        }

        public BitBoardG(int size, UInt64[] bitRep) : this(size)
        {
            LoadFrom(bitRep);
        }

        public void LoadFrom(UInt64[] bitRep)
        {
            _positions.Clear();
            for (int i = 0; i < _size; i++)
            {
                for (int j = 0; j < _size; j++)
                {
                    int move = GetMove(i, j);
                    if (CheckBit(bitRep, move))
                    {
                        _positions.Add(new Tuple<int, int>(i, j));
                    }
                }
            }
        }

        public IEnumerable<Tuple<int, int>> GetPositions()
        {
            return _positions;
        }

        public UInt64[] ToBitArray()
        {
            int length = ( _size * _size ) >> 6 + 1;
            var ret = new UInt64[length];
            foreach (var item in GetPositions())
            {
                int move = GetMove(item.Item1, item.Item2);
                SetBit(ret, move, true);
            }
            return ret;
        }

        public void SetBit(UInt64[] bitRep, int index, bool value)
        {
            UInt64 lg = 1;
            if (value)
            {
                bitRep[index >> 6] |= (lg << (index & 63));
            }
            else
            {
                bitRep[index >> 6] &= (~(lg << (index & 63)));
            }
        }

        public bool CheckBit(UInt64[] bitRep, int index)
        {
            UInt64 lg = 1;
            return ((bitRep[index >> 6] & (lg << (index & 63))) != 0);
        }

        public int GetMove(int i, int j)
        {
            return i * _size + j;
        }

        public Tuple<int, int> GetPosition(int move)
        {
            return new Tuple<int, int>(move / _size, move % _size);
        }
    }
}
