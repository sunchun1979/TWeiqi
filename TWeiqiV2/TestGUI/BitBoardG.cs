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
        
        public BitBoardG()
        {
            _positions = new List<Tuple<int, int>>();
        }

        public BitBoardG(UInt64[] bitRep)
        {
            _positions = new List<Tuple<int, int>>();
            LoadFrom(bitRep);
        }

        public void LoadFrom(UInt64[] bitRep)
        {
            _positions.Clear();
            for (int i = 0; i < Constants.Size; i++)
            {
                for (int j = 0; j < Constants.Size; j++)
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
            int length = ( Constants.Size * Constants.Size ) >> 6 + 1;
            var ret = new UInt64[length];
            foreach (var item in GetPositions())
            {
                int move = GetMove(item.Item1, item.Item2);
                SetBit(ret, move, true);
            }
            return ret;
        }

        public static void SetBit(UInt64[] bitRep, int index, bool value)
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

        public static bool CheckBit(UInt64[] bitRep, int index)
        {
            UInt64 lg = 1;
            return ((bitRep[index >> 6] & (lg << (index & 63))) != 0);
        }

        public static int GetMove(int i, int j)
        {
            return i * Constants.Size + j;
        }

        public static Tuple<int, int> GetPosition(int move)
        {
            return new Tuple<int, int>(move / Constants.Size, move % Constants.Size);
        }
    }
}
