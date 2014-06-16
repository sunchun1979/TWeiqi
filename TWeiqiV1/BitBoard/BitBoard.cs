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

        // Return if the first array is a subset of the second array
        static public bool SubSetOf(this BitArray bits, BitArray other)
        {
            for (int i = 0; i < bits.Length; i++)
            {
                if (bits[i])
                {
                    if (!other[i])
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        // Xor with true mask
        static public void XorTrue(this BitArray bits, BitArray other)
        {
            for (int i = 0; i < bits.Length; i++)
            {
                if (bits[i])
                {
                    bits[i] ^= other[i];
                }
            }
        }

        // Xor with true mask
        static public void OrTrue(this BitArray bits, BitArray other, BitArray pmask = null)
        {
            var mask = (pmask == null) ? bits : pmask;
            for (int i = 0; i < bits.Length; i++)
            {
                if (mask[i])
                {
                    bits[i] |= other[i];
                }
            }
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
            Size2 = (Size * Size / 32 + 1) * 32;
            stones = new BitArray[2];
            stones[BLACK] = new BitArray(Size2);
            stones[WHITE] = new BitArray(Size2);
            emptyStones = new BitArray(Size2);
            emptyStones.SetAll(true);
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
                ret += " ";
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
            if ( (i==3) && (j==1) )
            {
                var str = "str";
            }
            return Move19(GetMove19(i, j), color, koCheck);
        }

        public static void PrintValues(BitArray myList)
        {
            if (myList == null)
            {
                return;
            }
            for (int i = 0; i < myList.Length; i++)
            {
                if (myList[i])
                {
                    Console.Write(i + " ");
                }
            }
            Console.WriteLine();
        }

        public bool Move19(int move, int color, List<BitBoard> koCheck)
        {
            if (!emptyStones[move])
            {
                return false;
            }
            //var liberty = LibertyConst.LibertyMap[move].And(emptyStones);
            var liberty = GetLiberty19(move).And(emptyStones);
            
            var group = new BitArray(Size2);
            Set19(group, move);


            // merge group
            var newResult = MergeGroup(groups[color], liberties[color], group, liberty);
            var newGroups = newResult.Item1;
            var newLiberties = newResult.Item2;
            var newStones = newResult.Item3;
            // check opponent liberty
            var captured = CheckCapture(liberties[1 - color], newStones);
            // check self liberty
            BitArray capturedStones = null;
            if (captured == null)
            {
                var selfCapture = CheckCapture(newLiberties, stones[1 - color]);
                if (selfCapture != null)
                {
                    //Console.WriteLine("illegal2 " + move);
                    //for (int c = 0; c < 2; c++)
                    //{
                    //    Console.WriteLine("color = " + c);
                    //    for (int i = 0; i < groups[c].Count; i++)
                    //    {
                    //        PrintValues(groups[c][i]);
                    //        PrintValues(liberties[c][i]);
                    //    }
                    //}
                    //Console.WriteLine("current color: " + color);
                    //for (int i = 0; i < newGroups.Count; i++)
                    //{
                    //    PrintValues(newGroups[i]);
                    //    PrintValues(newLiberties[i]);
                    //}
                    //Console.ReadLine();
                    return false;
                }
            }   
                // capture
            else
            {
                capturedStones = new BitArray(Size2);
                foreach (var item in captured)
                {
                    capturedStones.Or(groups[1 - color][item]);
                    stones[1 - color] = stones[1 - color].Xor(groups[1 - color][item]);
                    groups[1 - color].RemoveAt(item);
                    liberties[1 - color].RemoveAt(item);
                }
            }
            // update opponent liberty
            for (int i = 0; i < liberties[1 - color].Count; i++)
            {
                liberties[1 - color][i].XorTrue(group);
            }
            // update self liberty
            liberties[color] = newLiberties;
            // update group
            groups[color] = newGroups;
            // update board
            stones[color] = (BitArray)newStones.Clone();
            emptyStones.Xor(group);

            if (captured != null)
            {
                RecalculateLiberty(capturedStones);
            }

            //for (int c = 0; c < 2; c++)
            //{
            //    Console.WriteLine("color = " + c);
            //    for (int i = 0; i < groups[c].Count; i++)
            //    {
            //        PrintValues(groups[c][i]);
            //        PrintValues(liberties[c][i]);
            //    }
            //}

            return true;
        }

        private void RecalculateLiberty(BitArray capturedStones)
        {
            var captureLiberty = new BitArray(Size2);
            for (int j = 0; j < capturedStones.Length; j++)
            {
                if (capturedStones[j])
                {
                    captureLiberty.Or(GetLiberty19(j));
                }
            }
            for (int c = 0; c < 2; c++)
            {
                for (int i = 0; i < groups[c].Count; i++)
                {
                    if (((BitArray)groups[c][i].Clone()).And(captureLiberty).OrAll())
                    {
                        var newLiberty = new BitArray(Size2);
                        for (int j = 0; j < groups[c][i].Length; j++)
                        {
                            if (groups[c][i][j])
                            {
                                newLiberty.Or(GetLiberty19(j));
                            }
                        }
                        newLiberty.XorTrue(stones[1 - c]);
                        newLiberty.XorTrue(stones[c]);
                        liberties[c][i] = (BitArray)newLiberty.Clone();
                    }
                }
            }
        }

        private List<int> CheckCapture(List<BitArray> libertyCandidate, BitArray opponentStones)
        {
            var captured = new List<int>();
            bool ret = false;
            for (int i = 0; i < libertyCandidate.Count; i++)
            {
                if (libertyCandidate[i].SubSetOf(opponentStones))
                {
                    captured.Add(i);
                    ret = true;
                }
            }
            if (ret)
            {
                return captured;
            }
            else
            {
                return null;
            }
        }

        private Tuple<List<BitArray>, List<BitArray>, BitArray> MergeGroup(List<BitArray> groupsByColor, List<BitArray> libertiesByColor, BitArray group, BitArray liberty)
        {
            var newGroups = new List<BitArray>();
            newGroups.Add((BitArray)group.Clone());
            var newLiberties = new List<BitArray>();
            newLiberties.Add((BitArray)liberty.Clone());
            var newStones = (BitArray)group.Clone();
            for (int i = 0; i < groupsByColor.Count; i++)
            {
                if(((BitArray)group.Clone()).And(libertiesByColor[i]).OrAll())
                {
                    newGroups[0].Or(groupsByColor[i]);
                    newLiberties[0].Or(libertiesByColor[i]);
                }
                else
                {
                    newGroups.Add((BitArray)groupsByColor[i].Clone());
                    newLiberties.Add((BitArray)libertiesByColor[i].Clone());
                }
                newStones.Or(groupsByColor[i]);
            }
            newLiberties[0].XorTrue(newStones);
            return new Tuple<List<BitArray>,List<BitArray>,BitArray>(newGroups, newLiberties, newStones);
        }

        // Private Helpers
        private BitArray GetLiberty19(int move)
        {
            var ret = new BitArray(Size2);
            // top
            if (move >= Size)
            {
                Set19(ret, move - Size);
            }
            // bottom
            if (move + Size < Size*Size)
            {
                Set19(ret, move + Size);
            }
            // left
            if (move % Size > 0)
            {
                Set19(ret, move - 1);
            }
            // right
            if (move % Size < Size - 1)
            {
                Set19(ret, move + 1);
            }
            return ret;
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

        private int GetMove19(int i, int j)
        {
            // i * 19 + j
            return i * Size + j;
        }
    }
}
