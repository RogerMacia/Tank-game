using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;

namespace Tank_game
{
    public partial class Form1 : Form
    {
        Socket server;
        public Form1()
        {
            InitializeComponent();
        }

        private void Send_Btn_Click(object sender, EventArgs e)
        {
            //Creamos un IPEndPoint con el ip del servidor y puerto del servidor al que deseamos conectarnos
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, 9000);

            //Creamos el secket
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);   //Intentamos conectar el socket
                this.BackColor = Color.Green;
                //MessageBox.Show("Connected");

                int code = Convert.ToInt32(Peticion_txtBox.Text.Split('/')[0]);

                if (code == 1)
                {
                    string message = Peticion_txtBox.Text;

                    //Enviamos al servidor el nombre tecleado
                    byte[] msg = Encoding.ASCII.GetBytes(message);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    message = Encoding.ASCII.GetString(msg2).Split('\0')[0];

                    MessageBox.Show("The IDs of the maps are: " + message);
                }

                this.BackColor = Color.Gray;
                server.Shutdown(SocketShutdown.Both);
                server.Close();
            }
            catch (SocketException ex)
            {
                MessageBox.Show("Cannot connect with the server\n" + ex.Message);
            }
        }
    }
}
