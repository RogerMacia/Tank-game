using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Tank_game
{
    public partial class Form2 : Form
    {
        private const int gridSizeH = 10;  // Height del mapa 
        private const int gridSizeW = 15;  // Width del mapa 
        private const int cellSize = 40;  // Tamaño de cada celda en píxeles
        private int[,] mapa;              // Arreglo bidimensional para representar el tipo de celda

        public Form2()
        {

            InitializeComponent();
            this.Width = gridSizeW * cellSize + 16;   // Ajusta el ancho de la ventana
            this.Height = gridSizeH * cellSize + 39;  // Ajusta la altura de la ventana
            this.Text = "Mapa del juego de los tanques";

            // Inicializa el mapa con diferentes tipos de bloques
            mapa = new int[gridSizeW, gridSizeH];
            for (int x = 0; x < gridSizeW; x++)
            {
                for (int y = 0; y < gridSizeH; y++)
                {
                    mapa[x, y] = 0; // Espacio vacío

                }
            }

            // Añade bloques destructibles en algunas posiciones (aleatoriamente o por diseño)

            mapa[2, 1] = 2; // Ejemplo de bloque indestructible
            mapa[2, 2] = 2;
            mapa[2, 3] = 2;
            mapa[7, 4] = 2;
            mapa[7, 5] = 2;
            mapa[7, 6] = 2;
            mapa[10, 2] = 2;
            mapa[11, 2] = 2;
            mapa[12, 2] = 2;
            mapa[13, 2] = 2;
            mapa[2, 5] = 1; // Ejemplo de bloque destructible
            mapa[2, 8] = 1;
            mapa[5, 7] = 1;
            mapa[6, 1] = 1;
            mapa[8, 8] = 1;
            mapa[12, 6] = 1;
            // Puedes agregar más bloques destructibles de acuerdo a la lógica del juego

            // Maneja el evento de dibujo
            this.Paint += new PaintEventHandler(Form2_Paint);
        }

        // Método que se llama cuando se necesita repintar el formulario
        private void Form2_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;

            // Dibuja el mapa según el tipo de bloque
            for (int x = 0; x < gridSizeW; x++)
            {
                for (int y = 0; y < gridSizeH; y++)
                {
                    Rectangle cell = new Rectangle(x * cellSize, y * cellSize, cellSize, cellSize);

                    // Determina qué tipo de celda es y dibuja el color correspondiente
                    if (mapa[x, y] == 0)
                    {
                        g.FillRectangle(Brushes.LawnGreen, cell); // Espacio vacío
                    }
                    else if (mapa[x, y] == 1)
                    {
                        g.FillRectangle(Brushes.DarkOliveGreen, cell); // Bloque destructible
                        g.DrawRectangle(Pens.Black, cell); // Dibuja el borde de la celda
                    }
                    else if (mapa[x, y] == 2)
                    {
                        g.FillRectangle(Brushes.SaddleBrown, cell); // Bloque indestructible
                        g.DrawRectangle(Pens.Black, cell); // Dibuja el borde de la celda
                    }

                }
            }
        }
    }
}