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
            this.splitContainer1.Panel1.Controls.Add(this.threeD);

            this.comboBox1.TextChanged += new System.EventHandler(this.comboBox1_TextChanged);
        }
        private XGL3D.XGL3D threeD;

        private void comboBox1_TextChanged(object sender, EventArgs e)
        {
            this.threeD.Setup(this.comboBox1.Text.Trim());
            this.threeD.Render();
            this.threeD.SendMsg(0,"test");
        }
    }
}
