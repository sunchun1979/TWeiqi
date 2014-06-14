using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BitBoardUtils
{
    public static class BitArrayExtension
    {
        // Return the logical And of all of the entries in the array.
        static public bool AndAll(this BitArray bits)
        {
            foreach (bool value in bits) if (!value) return false;
            return true;
        }

        // Return the logical Or of all of the entries in the array.
        static public bool OrAll(this BitArray bits)
        {
            foreach (bool value in bits) if (value) return true;
            return false;
        }
    }

    public class BitBoard : ICloneable, IEquatable<BitBoard>
    {
        // int = int32
        public const int BLACK = 0;
        public const int WHITE = 1;

        private int Size;
        private int Size2;

        private BitArray[] stones;
        private BitArray emptyStones;
        private BitArray mask;

        private List<BitArray>[] groups;
        private List<BitArray>[] liberties;
        private int[] numGroups;

        public BitBoard() : this(19)
        {

        }

        public BitBoard (int size)
        {
            Size = size;
            Size2 = Size * Size;
            stones = new BitArray[2];
            stones[BLACK] = new BitArray(Size2);
            stones[WHITE] = new BitArray(Size2);
            emptyStones = new BitArray(Size2);
            mask = new BitArray(Size2);
            groups = new List<BitArray>[2];
            groups[BLACK] = new List<BitArray>();
            groups[WHITE] = new List<BitArray>();
            liberties = new List<BitArray>[2];
            liberties[BLACK] = new List<BitArray>();
            liberties[WHITE] = new List<BitArray>();
            numGroups = new int[2];
        }

        public object Clone()
        {
            var retBoard = new BitBoard(Size);
            retBoard.stones = new BitArray[2];
            retBoard.stones[BLACK] = (BitArray)this.stones[BLACK].Clone();
            retBoard.stones[WHITE] = (BitArray)this.stones[WHITE].Clone();
            retBoard.emptyStones = (BitArray)this.emptyStones.Clone();
            retBoard.mask = (BitArray)this.mask.Clone();
            retBoard.groups = new List<BitArray>[2];
            retBoard.groups[BLACK] = this.groups[BLACK].Select(i => (BitArray)i.Clone()).ToList();
            retBoard.groups[WHITE] = this.groups[WHITE].Select(i => (BitArray)i.Clone()).ToList();
            retBoard.liberties = new List<BitArray>[2];
            retBoard.liberties[BLACK] = this.liberties[BLACK].Select(i => (BitArray)i.Clone()).ToList();
            retBoard.liberties[WHITE] = this.liberties[WHITE].Select(i => (BitArray)i.Clone()).ToList();
            retBoard.numGroups = new int[2];
            retBoard.numGroups[BLACK] = this.numGroups[BLACK];
            retBoard.numGroups[WHITE] = this.numGroups[WHITE];
            return retBoard;
        }

        public override string ToString()
        {
            string ret = string.Empty;
            for (int i = 0; i < Size; i++)
            {
                for (int j = 0; j < Size; j++)
                {
                    if (Check19(stones[BLACK], i, j))
                    {
                        ret += "X ";
                    }
                    else if (Check19(stones[WHITE], i, j))
                    {
                        ret += "O ";
                    }
                    else
                    {
                        ret += ". ";
                    }
                }
                ret += "\n";
            }
            return ret;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            if (obj is BitBoard)
            {
                return Equals((BitBoard)obj);
            }
            else
            {
                return false;
            }
        }

        public bool Equals(BitBoard board)
        {
            return (Size == board.Size) &&
                (stones[BLACK].Equals(board.stones[BLACK])) &&
                (stones[WHITE].Equals(board.stones[WHITE]));
        }

        public bool Move19(int i, int j, int color, List<BitBoard> koCheck = null)
        {
            return Move19(GetMove19(i, j), color, koCheck);
        }

        public bool Move19(int move, int color, List<BitBoard> koCheck)
        {
            var liberty = LibertyConst.LibertyMap[move];
            
            var group = new BitArray(Size2);
            Set19(group, move);

            // merge group
            var newGroups = MergeGroup(groups[color], liberties[color], group);
            var newStones = MergeStones(groups[color]);
            // check opponent liberty
            // check self liberty
            // update opponent liberty
            // update self liberty
            // update group
            // update legal moves
            // update board

            return true;
        }

        private BitArray MergeStones(List<BitArray> list)
        {
            var ret = (BitArray)list[0].Clone();
            for (int i = 1; i < list.Count; i++)
            {
                ret = ret.Or(list[i]);
            }
            return ret;
        }

        private List<BitArray> MergeGroup(List<BitArray> groupsByColor, List<BitArray> libertiesByColor, BitArray group)
        {
            var newGroups = new List<BitArray>();
            newGroups.Add(group);
            for (int i = 0; i < groupsByColor.Count; i++)
            {
                if (libertiesByColor[i].And(group).OrAll())
                {
                    newGroups[0] = newGroups[0].And(groupsByColor[i]);
                }
                else
                {
                    newGroups.Add(groupsByColor[i]);
                }
            }
            return newGroups;
        }

        // Private Helpers
        private int[] GetLiberty19(int move)
        {
            var ret = new List<int>();
            // top
            if (move >= 19)
            {
                ret.Add(move - 19);
            }
            // bottom
            if (move + 19 <= 361)
            {
                ret.Add(move + 19);
            }
            // left
            if (move % 19 > 0)
            {
                ret.Add(move - 1);
            }
            // right
            if (move % 19 < 18)
            {
                ret.Add(move + 1);
            }
            return ret.ToArray();
        }

        private bool Check19(BitArray p, int i, int j)
        {
            return Check19(p, GetMove19(i, j));
        }

        private bool Check19(BitArray p, int move)
        {
            // move / 16 defines the byte, move % 16 defines the bit
            return p[move];
        }

        private void Set19(BitArray p, int i, int j)
        {
            Set19(p, GetMove19(i, j));
        }

        private void Set19(BitArray p, int move)
        {
            // move / 16 defines the byte, move % 16 defines the bit
            p[move] = true;
        }

        private static int GetMove19(int i, int j)
        {
            // i * 19 + j
            return (i << 4) + (i << 1) + i + j;
        }


    }
}
