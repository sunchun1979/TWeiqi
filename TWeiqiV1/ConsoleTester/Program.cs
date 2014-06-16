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
            const int size = 19;
            BitBoard b = new BitBoard(size);

            ////b.Move19(3, 3, BitBoard.BLACK);
            ////b.Move19(1, 1, BitBoard.WHITE);
            ////b.Move19(3, 4, BitBoard.WHITE);
            ////b.Move19(3, 2, BitBoard.WHITE);
            ////b.Move19(4, 3, BitBoard.WHITE);
            ////Console.WriteLine(b);
            ////b.Move19(2, 3, BitBoard.WHITE);

            var rand = new Random(1001);
            int move = BitBoard.BLACK;
            for (int i = 0; i < 260; i++)
            {
                bool legal = false;
                while (!legal)
                {
                    int x = rand.Next(size);
                    int y = rand.Next(size);
                    legal = b.Move19(x, y, move);
                }
                move = 1 - move;
                //Console.WriteLine(" ===");
                //Console.WriteLine(i);
                //Console.WriteLine(b);
                //Console.WriteLine(" ===");
            }
            Console.WriteLine(b);
        }
    }
}
