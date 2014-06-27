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
        private BitBoardG blackStones;
        private BitBoardG whiteStones;

        public Form1()
        {
            InitializeComponent();
            blackStones = new BitBoardG();
            whiteStones = new BitBoardG();
        }

        private void paintBoard(Panel panel)
        {
            BufferedGraphics myBuffer;
            BufferedGraphicsContext currentContext;
            currentContext = BufferedGraphicsManager.Current;
            myBuffer = currentContext.Allocate(panel.CreateGraphics(), panel.DisplayRectangle);

            Graphics G = myBuffer.Graphics;

            #region painting the board
            // Painting the board itself
            G.DrawImage(panel.BackgroundImage, new Point(0, 0));
            using (var p = new Pen(Color.Black, 1))
            {
                for (int i = 0; i < Constants.Size; i++)
                {
                    int x1 = Constants.CellSize;
                    int y1 = Constants.CellSize + i * Constants.CellSize;
                    int x2 = Constants.Size * Constants.CellSize;
                    int y2 = y1;
                    G.DrawLine(p, new Point(x1, y1), new Point(x2, y2));
                }
                for (int i = 0; i < Constants.Size; i++)
                {
                    int x1 = Constants.CellSize + i * Constants.CellSize;
                    int y1 = Constants.CellSize;
                    int x2 = x1;
                    int y2 = Constants.Size * Constants.CellSize;
                    G.DrawLine(p, new Point(x1, y1), new Point(x2, y2));
                }
            }
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
            #endregion

            // Painting the stones
            DrawStone(G, blackStones, new Bitmap(TestGUI.Properties.Resources._24));
            DrawStone(G, whiteStones, new Bitmap(TestGUI.Properties.Resources._24_2));
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

            Program.process.StandardInput.WriteLine("move 100 0");
            ret = Program.process.StandardOutput.ReadLine();
            Program.process.StandardInput.WriteLine("move 101 1");
            ret = Program.process.StandardOutput.ReadLine();
            Program.process.StandardInput.WriteLine("move 99 0");
            ret = Program.process.StandardOutput.ReadLine();
            Program.process.StandardInput.WriteLine("getcurrent 0");
            ret = Program.process.StandardOutput.ReadLine();
            var rawBlack = ret.Split(',').Select(n => Convert.ToUInt64(n)).ToArray();
            blackStones.LoadFrom(rawBlack);
            Program.process.StandardInput.WriteLine("getcurrent 1");
            ret = Program.process.StandardOutput.ReadLine();
            var rawWhite = ret.Split(',').Select(n => Convert.ToUInt64(n)).ToArray();
            whiteStones.LoadFrom(rawWhite);
            paintBoard(panel1);
        }
    }
}
