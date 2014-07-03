using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.IO;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestGUI
{
    public partial class Form1 : Form
    {
        class DBoard
        {
            public BitBoardG blackStones;
            public BitBoardG whiteStones;
            public int size;
            public Process engine;

            public BitBoardG blackEst;
            public BitBoardG whiteEst;

            public DBoard(int _size)
            {
                size = _size;
                blackStones = new BitBoardG(size);
                whiteStones = new BitBoardG(size);

                blackEst = new BitBoardG(size);
                whiteEst = new BitBoardG(size);

                ProcessStartInfo startInfo = new ProcessStartInfo();
                startInfo.UseShellExecute = false;
                startInfo.RedirectStandardInput = true;
                startInfo.RedirectStandardOutput = true;
                startInfo.CreateNoWindow = true;
                //startInfo.WindowStyle = ProcessWindowStyle.Hidden;
                if (size == 19)
                {
                    startInfo.FileName = @"..\..\..\x64\Release\ConsoleDriver.exe";
                }
                else if (size == 9)
                {
                    startInfo.FileName = @"..\..\..\x64\Release\ConsoleDriver9.exe";
                }

                engine = new Process();
                engine.StartInfo = startInfo;
                engine.Start();
            }
        }

        private DBoard[] board = new DBoard[2];

        public Form1()
        {
            InitializeComponent();
        }

        private void paintBoard(Panel panel, DBoard db)
        {
            BufferedGraphics myBuffer;
            BufferedGraphicsContext currentContext;
            currentContext = BufferedGraphicsManager.Current;
            myBuffer = currentContext.Allocate(panel.CreateGraphics(), panel.DisplayRectangle);

            Graphics G = myBuffer.Graphics;

            int size = db.size;

            #region painting the board
            // Painting the board itself
            G.DrawImage(panel.BackgroundImage, new Point(0, 0));

            using (var p = new Pen(Color.Black, 1))
            {
                for (int i = 0; i < size; i++)
                {
                    int x1 = Constants.CellSize;
                    int y1 = Constants.CellSize + i * Constants.CellSize;
                    int x2 = size * Constants.CellSize;
                    int y2 = y1;
                    G.DrawLine(p, new Point(x1, y1), new Point(x2, y2));
                }
                for (int i = 0; i < size; i++)
                {
                    int x1 = Constants.CellSize + i * Constants.CellSize;
                    int y1 = Constants.CellSize;
                    int x2 = x1;
                    int y2 = size * Constants.CellSize;
                    G.DrawLine(p, new Point(x1, y1), new Point(x2, y2));
                }
            }
            if (size == 19)
            {
                using (var p = new SolidBrush(Color.Black))
                {
                    int[] dotx = { 4, 10, 16, 4, 10, 16, 4, 10, 16 };
                    int[] doty = { 4, 4, 4, 10, 10, 10, 16, 16, 16 };
                    for (int i = 0; i < dotx.Length; i++)
                    {
                        int x0 = Constants.CellSize * dotx[i];
                        int y0 = Constants.CellSize * doty[i];
                        G.FillEllipse(p, x0 - 3, y0 - 3, 6, 6);
                    }
                }
            }
            #endregion

            // Painting the stones
            DrawStone(G, db.blackStones, new Bitmap(TestGUI.Properties.Resources._24));
            DrawStone(G, db.whiteStones, new Bitmap(TestGUI.Properties.Resources._24_2));
            MarkStonePosition(G, db.blackEst, Color.Blue, Color.White);
            MarkStonePosition(G, db.whiteEst, Color.Red, Color.Black);
            myBuffer.Render();
            myBuffer.Render(panel.CreateGraphics());
        }

        private void DrawStone(Graphics G, BitBoardG bits, Bitmap map)
        {
            foreach (var stone in bits.GetPositions())
            {
                var gpos = GetDrawCoord(stone);
                G.DrawImage(map, (float)(gpos.Item1 - 0.5), (float)(gpos.Item2 - 0.5));
            }
        }

        private void MarkStonePosition(Graphics G, BitBoardG bits, Color fillColor, Color edgeColor)
        {
            foreach (var stone in bits.GetPositions())
            {
                int x0 = Constants.CellSize * (stone.Item1 + 1);
                int y0 = Constants.CellSize * (stone.Item2 + 1);
                SolidBrush brush = new SolidBrush(fillColor);
                Pen edgePen = new Pen(edgeColor, 1);
                G.FillRectangle(brush, x0 - 4, y0 - 4, 8, 8);
                G.DrawRectangle(edgePen, x0 - 4, y0 - 4, 8, 8);
            }
        }

        private Tuple<int, int> GetDrawCoord(Tuple<int, int> stone)
        {
            int x, y;
            x = Constants.CellSize * (stone.Item1 + 1) - Constants.CellSize / 2 + 3;
            y = Constants.CellSize * (stone.Item2 + 1) - Constants.CellSize / 2 + 3;
            return new Tuple<int, int>(x, y);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string ret;

            board[0].engine.StandardInput.WriteLine("loadsgf C:\\Users\\chunsun\\SkyDrive\\Projects\\Weiqi\\DataSet\\single\\66.sgf");
            ret = board[0].engine.StandardOutput.ReadLine();

            //Program.process.StandardInput.WriteLine("move 101 1");
            //ret = Program.process.StandardOutput.ReadLine();
            //Program.process.StandardInput.WriteLine("move 99 0");
            //ret = Program.process.StandardOutput.ReadLine();
            board[0].engine.StandardInput.WriteLine("getcurrent 0");
            ret = board[0].engine.StandardOutput.ReadLine();
            var rawBlack = ret.Split(',').Select(n => Convert.ToUInt64(n)).ToArray();
            board[0].blackStones.LoadFrom(rawBlack);
            board[0].engine.StandardInput.WriteLine("getcurrent 1");
            ret = board[0].engine.StandardOutput.ReadLine();
            var rawWhite = ret.Split(',').Select(n => Convert.ToUInt64(n)).ToArray();
            board[0].whiteStones.LoadFrom(rawWhite);
            paintBoard(panel1, board[0]);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            board[0].engine.StandardInput.WriteLine("exit");
            board[1].engine.StandardInput.WriteLine("exit");
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            board[0] = new DBoard(19);
            board[1] = new DBoard(9);
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            paintBoard(panel1, board[0]);
        }

        private void panel2_Paint(object sender, PaintEventArgs e)
        {
            paintBoard(panel2, board[1]);
        }

        private void panel2_MouseClick(object sender, MouseEventArgs e)
        {
            int x = (int)Math.Round((double)e.X / Constants.CellSize - 1, MidpointRounding.AwayFromZero);
            int y = (int)Math.Round((double)e.Y / Constants.CellSize - 1, MidpointRounding.AwayFromZero);
            int move = board[1].blackStones.GetMove(x, y);
            if ((move >=0) && (move<board[1].size*board[1].size))
            {
                board[1].engine.StandardInput.WriteLine("move " + move + " 0");
                string ret = board[1].engine.StandardOutput.ReadLine();
                var rawStones = ret.Split(',').Select(n => Convert.ToUInt64(n)).ToArray();
                board[1].blackStones.LoadFrom(rawStones);
                ret = board[1].engine.StandardOutput.ReadLine();
                rawStones = ret.Split(',').Select(n => Convert.ToUInt64(n)).ToArray();
                board[1].whiteStones.LoadFrom(rawStones);
            }
            paintBoard(panel2, board[1]);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            board[1].engine.StandardInput.WriteLine("getestimate");
            string ret = board[1].engine.StandardOutput.ReadLine();
            this.Text = ret;
            ret = board[1].engine.StandardOutput.ReadLine();
            var rawEstimate = ret.Split(',').Select(n => Convert.ToUInt64(n)).ToArray();
            board[1].blackEst.LoadFrom(rawEstimate);
            ret = board[1].engine.StandardOutput.ReadLine();
            rawEstimate = ret.Split(',').Select(n => Convert.ToUInt64(n)).ToArray();
            board[1].whiteEst.LoadFrom(rawEstimate);
            paintBoard(panel2, board[1]);
        }
    }
}
