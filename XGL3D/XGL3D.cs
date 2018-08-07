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
        public delegate void XGLEventDelg(int id,string msg);
        public event XGLEventDelg EventXGL;
        public XGL3D()
        {
            InitializeComponent();

            XGLCreator creator = new XGLCreator();
            this.BindXGLInterface(creator.createXGL());
        }

        public string VertexShader
        {
            get;
            set;
        }

        public string GeometryShader
        {
            get;
            set;
        }

        public string TesselationShader
        {
            get;
            set;
        }

        public string FragmentShader
        {
            get;
            set;
        }


        public override int OnViewMsg(int id, string mgs)
        {
            switch(id)
            {
                case 1:
                    VertexShader = mgs;
                    break;
                case 2:
                    GeometryShader = mgs;
                    break;
                case 3:
                    TesselationShader = mgs;
                    break;
                case 4:
                    FragmentShader = mgs;
                    break;
            }
            EventXGL(id,mgs);
            return base.OnViewMsg(id,mgs);
        }

        void test1()
        {
            test(out byte[] tempvar);
        }
        void test(out byte[] msg)
        {
            msg = null;
        }
    }
}
