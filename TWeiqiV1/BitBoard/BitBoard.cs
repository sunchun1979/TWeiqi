using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BitBoardUtils
{
    public class BitBoard : ICloneable, IEquatable<BitBoard>
    {
        public const int BLACK = 0;
        public const int WHITE = 1;

        private int Size;
        private int bSize;

        private int[][] stones;
        private int[] emptyStones;
        private int[][] legalMoves;
        private int[] mask;

        private List<int[]>[] groups;
        private List<int[]>[] liberties;
        private int[] numGroups;

        public BitBoard() : this(19)
        {

        }

        public BitBoard (int size)
        {
            Size = size;
            bSize = (Size * Size / sizeof(int) + 1);
            stones = new int[2][];
            stones[BLACK] = new int[bSize];
            stones[WHITE] = new int[bSize];
            emptyStones = new int[bSize];
            legalMoves = new int[2][];
            legalMoves[BLACK] = new int[bSize];
            legalMoves[WHITE] = new int[bSize];
            mask = new int[bSize];
            groups = new List<int[]>[2];
            groups[BLACK] = new List<int[]>();
            groups[WHITE] = new List<int[]>();
            liberties = new List<int[]>[2];
            liberties[BLACK] = new List<int[]>();
            liberties[WHITE] = new List<int[]>();
            numGroups = new int[2];
        }

        public object Clone()
        {
            var retBoard = new BitBoard(Size);
            retBoard.stones = new int[2][];
            retBoard.stones[BLACK] = (int[])this.stones[BLACK].Clone();
            retBoard.stones[WHITE] = (int[])this.stones[WHITE].Clone();
            retBoard.emptyStones = (int[])this.emptyStones.Clone();
            retBoard.legalMoves = new int[2][];
            retBoard.legalMoves[BLACK] = (int[])this.legalMoves[BLACK].Clone();
            retBoard.legalMoves[WHITE] = (int[])this.legalMoves[WHITE].Clone();
            retBoard.mask = (int[])this.mask.Clone();
            retBoard.groups = new List<int[]>[2];
            retBoard.groups[BLACK] = this.groups[BLACK].Select(i => (int[])i.Clone()).ToList();
            retBoard.groups[WHITE] = this.groups[WHITE].Select(i => (int[])i.Clone()).ToList();
            retBoard.liberties = new List<int[]>[2];
            retBoard.liberties[BLACK] = this.liberties[BLACK].Select(i => (int[])i.Clone()).ToList();
            retBoard.liberties[WHITE] = this.liberties[WHITE].Select(i => (int[])i.Clone()).ToList();
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
                (Enumerable.SequenceEqual(stones[BLACK], board.stones[BLACK])) &&
                (Enumerable.SequenceEqual(stones[WHITE], board.stones[WHITE]));
        }

        public bool Move19(int i, int j, int color, List<BitBoard> koCheck = null)
        {
            return Move19(GetMove19(i, j), color, koCheck);
        }

        public bool Move19(int move, int color, List<BitBoard> koCheck)
        {
            int[] liberty = GetLiberty19(move);
            // to change the following two lines
            Set19(stones[color], move);
            Array.ForEach(liberty, x => Set19(stones[1 - color], x));
            return true;
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

        private bool Check19(int[] p, int i, int j)
        {
            return Check19(p, GetMove19(i, j));
        }

        private bool Check19(int[] p, int move)
        {
            // move / 16 defines the byte, move % 16 defines the bit
            return ((p[move >> 4]) & (1 << (move & 15))) != 0;
        }

        private void Set19(int[] p, int i, int j)
        {
            Set19(p, GetMove19(i, j));
        }

        private void Set19(int[] p, int move)
        {
            // move / 8 defines the byte, move % 8 defines the bit
            (p[move >> 4]) |= (1 << (move & 15));
        }

        private static int GetMove19(int i, int j)
        {
            // i * 19 + j
            return (i << 4) + (i << 1) + i + j;
        }
    }
}
