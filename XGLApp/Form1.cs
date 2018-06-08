using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;


namespace XGLApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            this.Load += Form1_Load;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.threeD = new XGL3D.XGL3D();
            this.threeD.Dock = System.Windows.Forms.DockStyle.Fill;
            this.threeD.EventXGL += ThreeD_EventXGL; ;
            this.splitContainer1.Panel1.Controls.Add(this.threeD);

            this.comboBox1.TextChanged += new System.EventHandler(this.comboBox1_TextChanged);
        }

        private void ThreeD_EventXGL(int id, string msg)
        {
            switch(id)
            {
                case 1:
                    this.richTextBox1.Text = msg;
                    break;
                case 2:
                    this.richTextBox2.Text = msg;
                    break;
                case 3:
                    this.richTextBox3.Text = msg;
                    break;
                case 4:
                    this.richTextBox4.Text = msg;
                    break;
            }
            this.toolStripStatusLabel1.Text = string.Format("加载shader!");
        }

        private XGL3D.XGL3D threeD;

        private void comboBox1_TextChanged(object sender, EventArgs e)
        {
            this.threeD.Setup(this.comboBox1.Text.Trim());
            this.threeD.Render();
            this.threeD.SendMsg(0,"test");
        }

        private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
