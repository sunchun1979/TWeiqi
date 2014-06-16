using System;
using System.Collections.Generic;
using System.Diagnostics;
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
            var rand = new Random();
            int size = 19;

            const int totalMove = 200;

            Console.ReadLine();
            Stopwatch sw = new Stopwatch();
            sw.Start();
            Parallel.For(0, 2000, i =>
            {
                Console.WriteLine("game  " + i);
                RandomGame(i, size, totalMove, rand);
            });
            sw.Stop();
            Console.WriteLine("Elapsed = {0}", sw.Elapsed);
        }

        private static void RandomGame(int gameNo, int size, int totalMove, Random rand)
        {
            BitBoard b = new BitBoard();
            int move = Constants.BLACK;
            for (int i = 0; i < totalMove; i++)
            {
                bool legal = false;
                while (!legal)
                {
                    int x = rand.Next(size);
                    int y = rand.Next(size);
                    legal = b.Move(x, y, move);
                }
                move = 1 - move;
            }
            //Console.WriteLine(b);
        }
    }
}
