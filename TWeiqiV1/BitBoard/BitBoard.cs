using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BitBoard
{
    public class BitBoard : ICloneable
    {
        private int Size;
        private int bSize;

        private byte[] blackStones;
        private byte[] whiteStones;
        private byte[] emptyStones;
        private byte[] blackLegal;
        private byte[] whiteLegal;
        private byte[] mask;

        private List<byte[]> blackGroups;
        private List<byte[]> whiteGroups;
        private List<byte[]> blackLiberties;
        private List<byte[]> whiteLiberties;
        private int numGroups;

        public BitBoard() : this(19)
        {

        }

        public BitBoard (int size)
        {
            Size = size;
            bSize = Size * Size / 8;
            blackStones = new byte[bSize];
            whiteStones = new byte[bSize];
            emptyStones = new byte[bSize];
            blackLegal = new byte[bSize];
            whiteLegal = new byte[bSize];
            mask = new byte[bSize];
            blackGroups = new List<byte[]>();
            whiteGroups = new List<byte[]>();
            blackLiberties = new List<byte[]>();
            whiteLiberties = new List<byte[]>();
            numGroups = 0;
        }

        public object Clone()
        {
            var retBoard = new BitBoard(Size);
            retBoard.blackStones = (byte[])this.blackStones.Clone();
            retBoard.whiteStones = (byte[])this.whiteStones.Clone();
            retBoard.emptyStones = (byte[])this.emptyStones.Clone();
            retBoard.blackLegal = (byte[])this.blackLegal.Clone();
            retBoard.whiteLegal = (byte[])this.whiteLegal.Clone();
            retBoard.mask = (byte[])this.mask.Clone();
            retBoard.blackGroups = this.blackGroups.Select(i => (byte[])i.Clone()).ToList();
            retBoard.whiteGroups = this.whiteGroups.Select(i => (byte[])i.Clone()).ToList();
            retBoard.blackLiberties = this.blackLiberties.Select(i => (byte[])i.Clone()).ToList();
            retBoard.whiteLiberties = this.whiteLiberties.Select(i => (byte[])i.Clone()).ToList();
            retBoard.numGroups = this.numGroups;
            return retBoard;
        }
    }
}
