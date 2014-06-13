using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using BitBoardUtils;

namespace ConsoleTester
{
    class Program
    {
        static void Main(string[] args)
        {
            BitBoard b = new BitBoard();
            Console.WriteLine(b);
            Console.WriteLine();
            b.Move19(9, 9, BitBoard.BLACK);
            b.Move19(0, 0, BitBoard.BLACK);
            b.Move19(0, 4, BitBoard.BLACK);
            b.Move19(0, 18, BitBoard.BLACK);
            b.Move19(18, 18, BitBoard.WHITE);
            b.Move19(18, 4, BitBoard.WHITE);
            Console.WriteLine(b);
        }
    }
}
