using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using XGLWrapper;
using XGLEvent;

namespace XGL3D
{
    public partial class XGL3D : XGLControl
    {
        public XGL3D()
        {
            InitializeComponent();

            XGLCreator creator = new XGLCreator();
            this.BindXGLInterface(creator.createXGL());
        }
    }
}
