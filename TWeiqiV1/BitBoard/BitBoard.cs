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
        static public bool HasFalse(this BitArray bits)
        {
            foreach (bool value in bits) if (!value) return false;
            return true;
        }

        // Return the logical Or of all of the entries in the array.
        static public bool HasTrue(this BitArray bits)
        {
            foreach (bool value in bits) if (value) return true;
            return false;
        }

        // Return if the first array is a subset of the second array
        static public bool SubSetOf(this BitArray bits, BitArray other)
        {
            for (int i = 0; i < bits.Length; i++)
            {
                if (bits[i] && (!other[i])) return false;
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

        // Or with true mask
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

        // Equals
        static public bool BitEquals(this BitArray bits, BitArray other)
        {
            for (int i = 0; i < bits.Length; i++)
            {
                if (bits[i] != other[i])
                {
                    return false;
                }
            }
            return true;
        }

        // ToString
        static public string ToIndexString(this BitArray myList)
        {
            string ret = string.Empty;
            if (myList == null)
            {
                return ret;
            }
            for (int i = 0; i < myList.Length; i++)
            {
                if (myList[i])
                {
                    ret += (i.ToString() + " ");
                }
            }
            return ret;
        }
    }

    public class BitBoard : ICloneable, IEquatable<BitBoard>
    {
        // int = int32
        private int Size;
        private int Size2;

        private BitArray[] stones;
        private BitArray emptyStones;

        private List<BitArray>[] groups;
        private List<BitArray>[] liberties;

        public BitBoard() : this(19)
        {

        }

        public BitBoard (int size)
        {
            Size = size;
            Size2 = (Size * Size / 32 + 1) * 32;
            stones = new BitArray[2];
            stones[Constants.BLACK] = new BitArray(Size2);
            stones[Constants.WHITE] = new BitArray(Size2);
            emptyStones = new BitArray(Size2);
            emptyStones.SetAll(true);
            groups = new List<BitArray>[2];
            groups[Constants.BLACK] = new List<BitArray>();
            groups[Constants.WHITE] = new List<BitArray>();
            liberties = new List<BitArray>[2];
            liberties[Constants.BLACK] = new List<BitArray>();
            liberties[Constants.WHITE] = new List<BitArray>();
        }

        public object Clone()
        {
            var retBoard = new BitBoard(Size);
            retBoard.stones = new BitArray[2];
            retBoard.stones[Constants.BLACK] = (BitArray)this.stones[Constants.BLACK].Clone();
            retBoard.stones[Constants.WHITE] = (BitArray)this.stones[Constants.WHITE].Clone();
            retBoard.emptyStones = (BitArray)this.emptyStones.Clone();
            retBoard.groups = new List<BitArray>[2];
            retBoard.groups[Constants.BLACK] = this.groups[Constants.BLACK].Select(i => (BitArray)i.Clone()).ToList();
            retBoard.groups[Constants.WHITE] = this.groups[Constants.WHITE].Select(i => (BitArray)i.Clone()).ToList();
            retBoard.liberties = new List<BitArray>[2];
            retBoard.liberties[Constants.BLACK] = this.liberties[Constants.BLACK].Select(i => (BitArray)i.Clone()).ToList();
            retBoard.liberties[Constants.WHITE] = this.liberties[Constants.WHITE].Select(i => (BitArray)i.Clone()).ToList();
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
                    if (CheckBoard(stones[Constants.BLACK], i, j))
                    {
                        ret += "X ";
                    }
                    else if (CheckBoard(stones[Constants.WHITE], i, j))
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
                (stones[Constants.BLACK].BitEquals(board.stones[Constants.BLACK])) &&
                (stones[Constants.WHITE].BitEquals(board.stones[Constants.WHITE]));
        }

        public bool Move(int i, int j, int color, List<BitBoard> koCheck = null)
        {
            return Move(GetMove(i, j), color, koCheck);
        }

        public bool Move(int move, int color, List<BitBoard> koCheck)
        {
            if (!emptyStones[move])
            {
                return false;
            }
            var liberty = GetLiberty(move).And(emptyStones);
            
            var group = new BitArray(Size2);
            SetBoard(group, move);


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

            return true;
        }

        private void RecalculateLiberty(BitArray capturedStones)
        {
            var captureLiberty = new BitArray(Size2);
            for (int j = 0; j < capturedStones.Length; j++)
            {
                if (capturedStones[j])
                {
                    captureLiberty.Or(GetLiberty(j));
                }
            }
            for (int c = 0; c < 2; c++)
            {
                for (int i = 0; i < groups[c].Count; i++)
                {
                    if (((BitArray)groups[c][i].Clone()).And(captureLiberty).HasTrue())
                    {
                        var newLiberty = new BitArray(Size2);
                        for (int j = 0; j < groups[c][i].Length; j++)
                        {
                            if (groups[c][i][j])
                            {
                                newLiberty.Or(GetLiberty(j));
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
            // make sure it's descending order, otherwise List<T>.Remove(index) doesn't work
            for (int i = libertyCandidate.Count - 1; i >= 0; i--)
            {
                if (libertyCandidate[i].SubSetOf(opponentStones))
                {
                    captured.Add(i);
                }
            }
            if (captured.Count > 0)
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
                if(((BitArray)group.Clone()).And(libertiesByColor[i]).HasTrue())
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
            return new Tuple<List<BitArray>, List<BitArray>, BitArray>(newGroups, newLiberties, newStones);
        }

        // Private Helpers
        private BitArray GetLiberty(int move)
        {
            return (BitArray)(Constants.LibertyMap[move]).Clone();
        }

        private BitArray GetLibertyGeneric(int move)
        {
            var ret = new BitArray(Size2);
            // top
            if (move >= Size)
            {
                SetBoard(ret, move - Size);
            }
            // bottom
            if (move + Size < Size*Size)
            {
                SetBoard(ret, move + Size);
            }
            // left
            if (move % Size > 0)
            {
                SetBoard(ret, move - 1);
            }
            // right
            if (move % Size < Size - 1)
            {
                SetBoard(ret, move + 1);
            }
            return ret;
        }

        private bool CheckBoard(BitArray p, int i, int j)
        {
            return CheckBoard(p, GetMove(i, j));
        }

        private bool CheckBoard(BitArray p, int move)
        {
            // move / 16 defines the byte, move % 16 defines the bit
            return p[move];
        }

        private void SetBoard(BitArray p, int i, int j)
        {
            SetBoard(p, GetMove(i, j));
        }

        private void SetBoard(BitArray p, int move)
        {
            // move / 16 defines the byte, move % 16 defines the bit
            p[move] = true;
        }

        private int GetMove(int i, int j)
        {
            // i * 19 + j
            return i * Size + j;
        }
    }
}
