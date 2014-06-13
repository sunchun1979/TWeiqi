using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BitBoardUtils
{
    public class BitBoard : ICloneable, IEquatable<BitBoard>
    {
        public const short BADLIBERTY = 400;
        public const short BLACK = 0;
        public const short WHITE = 1;

        private short Size;
        private short bSize;

        private byte[][] stones;
        private byte[] emptyStones;
        private byte[][] legalMoves;
        private byte[] mask;

        private List<short[]>[] groups;
        private List<short[]>[] liberties;
        private short[] numGroups;

        public BitBoard() : this(19)
        {

        }

        public BitBoard (short size)
        {
            Size = size;
            bSize = (short)(Size * Size / 8 + 1);
            stones = new byte[2][];
            stones[BLACK] = new byte[bSize];
            stones[WHITE] = new byte[bSize];
            emptyStones = new byte[bSize];
            legalMoves = new byte[2][];
            legalMoves[BLACK] = new byte[bSize];
            legalMoves[WHITE] = new byte[bSize];
            mask = new byte[bSize];
            groups = new List<short[]>[2];
            groups[BLACK] = new List<short[]>();
            groups[WHITE] = new List<short[]>();
            liberties = new List<short[]>[2];
            liberties[BLACK] = new List<short[]>();
            liberties[WHITE] = new List<short[]>();
            numGroups = new short[2];
        }

        public object Clone()
        {
            var retBoard = new BitBoard(Size);
            retBoard.stones = new byte[2][];
            retBoard.stones[BLACK] = (byte[])this.stones[BLACK].Clone();
            retBoard.stones[WHITE] = (byte[])this.stones[WHITE].Clone();
            retBoard.emptyStones = (byte[])this.emptyStones.Clone();
            retBoard.legalMoves = new byte[2][];
            retBoard.legalMoves[BLACK] = (byte[])this.legalMoves[BLACK].Clone();
            retBoard.legalMoves[WHITE] = (byte[])this.legalMoves[WHITE].Clone();
            retBoard.mask = (byte[])this.mask.Clone();
            retBoard.groups = new List<short[]>[2];
            retBoard.groups[BLACK] = this.groups[BLACK].Select(i => (short[])i.Clone()).ToList();
            retBoard.groups[WHITE] = this.groups[WHITE].Select(i => (short[])i.Clone()).ToList();
            retBoard.liberties = new List<short[]>[2];
            retBoard.liberties[BLACK] = this.liberties[BLACK].Select(i => (short[])i.Clone()).ToList();
            retBoard.liberties[WHITE] = this.liberties[WHITE].Select(i => (short[])i.Clone()).ToList();
            retBoard.numGroups = new short[2];
            retBoard.numGroups[BLACK] = this.numGroups[BLACK];
            retBoard.numGroups[WHITE] = this.numGroups[WHITE];
            return retBoard;
        }

        public override string ToString()
        {
            string ret = string.Empty;
            for (short i = 0; i < Size; i++)
            {
                for (short j = 0; j < Size; j++)
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

        public bool Move19(short i, short j, short color, List<BitBoard> koCheck = null)
        {
            return Move19(GetMove19(i, j), color, koCheck);
        }

        public bool Move19(short move, short color, List<BitBoard> koCheck)
        {
            short[] liberty = GetLiberty19(move);
            // to change the following two lines
            Set19(stones[color], move);
            Array.ForEach(liberty, x => Set19(stones[1 - color], x));
            return true;
        }

        // Private Helpers
        private short[] GetLiberty19(short move)
        {
            var ret = new List<short>();
            // top
            if (move >= 19)
            {
                ret.Add((short)(move - 19));
            }
            // bottom
            if (move + 19 <= 361)
            {
                ret.Add((short)(move + 19));
            }
            // left
            if (move % 19 > 0)
            {
                ret.Add((short)(move - 1));
            }
            // right
            if (move % 19 < 18)
            {
                ret.Add((short)(move + 1));
            }
            return ret.ToArray();
        }

        private bool Check19(byte[] p, short i, short j)
        {
            return Check19(p, GetMove19(i, j));
        }

        private bool Check19(byte[] p, short move)
        {
            // move / 8 defines the byte, move % 8 defines the bit
            return ((p[move >> 3]) & (1 << (move & 7))) != 0;
        }

        private void Set19(byte[] p, short i, short j)
        {
            Set19(p, GetMove19(i, j));
        }

        private void Set19(byte[] p, short move)
        {
            // move / 8 defines the byte, move % 8 defines the bit
            (p[move >> 3]) |= (byte)(1 << (move & 7));
        }

        private static short GetMove19(short i, short j)
        {
            // i * 19 + j
            return (short)((i << 4) + (i << 1) + i + j);
        }
    }
}
