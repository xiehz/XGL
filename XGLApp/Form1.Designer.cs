namespace XGLApp
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.vs_tabPage1 = new System.Windows.Forms.TabPage();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.gs_tabPage2 = new System.Windows.Forms.TabPage();
            this.richTextBox2 = new System.Windows.Forms.RichTextBox();
            this.fs_tabPage4 = new System.Windows.Forms.TabPage();
            this.richTextBox4 = new System.Windows.Forms.RichTextBox();
            this.tcs_tabPage = new System.Windows.Forms.TabPage();
            this.richTextBox3 = new System.Windows.Forms.RichTextBox();
            this.tes_tabPage = new System.Windows.Forms.TabPage();
            this.richTextBox5 = new System.Windows.Forms.RichTextBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.vs_tabPage1.SuspendLayout();
            this.gs_tabPage2.SuspendLayout();
            this.fs_tabPage4.SuspendLayout();
            this.tcs_tabPage.SuspendLayout();
            this.tes_tabPage.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.splitContainer1.Location = new System.Drawing.Point(4, 2);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.splitContainer1_Panel1_Paint);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.BackColor = System.Drawing.SystemColors.Control;
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(1104, 569);
            this.splitContainer1.SplitterDistance = 879;
            this.splitContainer1.TabIndex = 0;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.BackColor = System.Drawing.SystemColors.Control;
            this.splitContainer2.Panel1.Controls.Add(this.groupBox1);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.tabControl1);
            this.splitContainer2.Size = new System.Drawing.Size(221, 569);
            this.splitContainer2.SplitterDistance = 83;
            this.splitContainer2.SplitterWidth = 8;
            this.splitContainer2.TabIndex = 2;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.comboBox1);
            this.groupBox1.Location = new System.Drawing.Point(3, 9);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(215, 71);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "XGL";
            // 
            // comboBox1
            // 
            this.comboBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "ModelGL",
            "AxesShape",
            "Tutorial2",
            "Tutorial3",
            "Tutorial4",
            "Tutorial5",
            "Tutorial6",
            "AmbientLight",
            "DiffuseLight",
            "SpecularLight",
            "PointLight",
            "SpotLight",
            "AssimpImport",
            "ShadowMap",
            "ShadowMap2",
            "SphereShape",
            "DepthImage",
            "SkyCube",
            "NormalMap",
            "Billboarding",
            "ParticleSystem",
            "PickingDemo",
            "BasicTesselation",
            "SimpleTesselation",
            "PNTessellation",
            "SimpleVAO",
            "VAOInstance",
            "DeferredShading0",
            "DeferredShading1",
            "DeferredShading2"});
            this.comboBox1.Location = new System.Drawing.Point(6, 20);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(203, 20);
            this.comboBox1.TabIndex = 0;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.vs_tabPage1);
            this.tabControl1.Controls.Add(this.gs_tabPage2);
            this.tabControl1.Controls.Add(this.fs_tabPage4);
            this.tabControl1.Controls.Add(this.tcs_tabPage);
            this.tabControl1.Controls.Add(this.tes_tabPage);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(221, 478);
            this.tabControl1.TabIndex = 1;
            // 
            // vs_tabPage1
            // 
            this.vs_tabPage1.Controls.Add(this.richTextBox1);
            this.vs_tabPage1.Location = new System.Drawing.Point(4, 22);
            this.vs_tabPage1.Name = "vs_tabPage1";
            this.vs_tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.vs_tabPage1.Size = new System.Drawing.Size(213, 452);
            this.vs_tabPage1.TabIndex = 0;
            this.vs_tabPage1.Text = "vertex";
            this.vs_tabPage1.UseVisualStyleBackColor = true;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBox1.Location = new System.Drawing.Point(3, 3);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(207, 446);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = "";
            // 
            // gs_tabPage2
            // 
            this.gs_tabPage2.Controls.Add(this.richTextBox2);
            this.gs_tabPage2.Location = new System.Drawing.Point(4, 22);
            this.gs_tabPage2.Name = "gs_tabPage2";
            this.gs_tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.gs_tabPage2.Size = new System.Drawing.Size(213, 452);
            this.gs_tabPage2.TabIndex = 1;
            this.gs_tabPage2.Text = "geometry";
            this.gs_tabPage2.UseVisualStyleBackColor = true;
            // 
            // richTextBox2
            // 
            this.richTextBox2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBox2.Location = new System.Drawing.Point(3, 3);
            this.richTextBox2.Name = "richTextBox2";
            this.richTextBox2.Size = new System.Drawing.Size(207, 446);
            this.richTextBox2.TabIndex = 1;
            this.richTextBox2.Text = "";
            // 
            // fs_tabPage4
            // 
            this.fs_tabPage4.Controls.Add(this.richTextBox4);
            this.fs_tabPage4.Location = new System.Drawing.Point(4, 22);
            this.fs_tabPage4.Name = "fs_tabPage4";
            this.fs_tabPage4.Size = new System.Drawing.Size(213, 452);
            this.fs_tabPage4.TabIndex = 3;
            this.fs_tabPage4.Text = "fragment";
            this.fs_tabPage4.UseVisualStyleBackColor = true;
            // 
            // richTextBox4
            // 
            this.richTextBox4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBox4.Location = new System.Drawing.Point(0, 0);
            this.richTextBox4.Name = "richTextBox4";
            this.richTextBox4.Size = new System.Drawing.Size(213, 452);
            this.richTextBox4.TabIndex = 1;
            this.richTextBox4.Text = "";
            // 
            // tcs_tabPage
            // 
            this.tcs_tabPage.Controls.Add(this.richTextBox3);
            this.tcs_tabPage.Location = new System.Drawing.Point(4, 22);
            this.tcs_tabPage.Name = "tcs_tabPage";
            this.tcs_tabPage.Size = new System.Drawing.Size(213, 452);
            this.tcs_tabPage.TabIndex = 4;
            this.tcs_tabPage.Text = "tcs";
            this.tcs_tabPage.UseVisualStyleBackColor = true;
            // 
            // richTextBox3
            // 
            this.richTextBox3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBox3.Location = new System.Drawing.Point(0, 0);
            this.richTextBox3.Name = "richTextBox3";
            this.richTextBox3.Size = new System.Drawing.Size(213, 452);
            this.richTextBox3.TabIndex = 0;
            this.richTextBox3.Text = "";
            // 
            // tes_tabPage
            // 
            this.tes_tabPage.Controls.Add(this.richTextBox5);
            this.tes_tabPage.Location = new System.Drawing.Point(4, 22);
            this.tes_tabPage.Name = "tes_tabPage";
            this.tes_tabPage.Padding = new System.Windows.Forms.Padding(3);
            this.tes_tabPage.Size = new System.Drawing.Size(213, 452);
            this.tes_tabPage.TabIndex = 5;
            this.tes_tabPage.Text = "tes";
            this.tes_tabPage.UseVisualStyleBackColor = true;
            // 
            // richTextBox5
            // 
            this.richTextBox5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBox5.Location = new System.Drawing.Point(3, 3);
            this.richTextBox5.Name = "richTextBox5";
            this.richTextBox5.Size = new System.Drawing.Size(207, 446);
            this.richTextBox5.TabIndex = 0;
            this.richTextBox5.Text = "";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripProgressBar1,
            this.toolStripStatusLabel1});
            this.statusStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
            this.statusStrip1.Location = new System.Drawing.Point(0, 574);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1116, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripProgressBar1
            // 
            this.toolStripProgressBar1.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.toolStripProgressBar1.Name = "toolStripProgressBar1";
            this.toolStripProgressBar1.Size = new System.Drawing.Size(100, 16);
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1116, 596);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.splitContainer1);
            this.Name = "Form1";
            this.Text = "XGL Beginning";
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.vs_tabPage1.ResumeLayout(false);
            this.gs_tabPage2.ResumeLayout(false);
            this.fs_tabPage4.ResumeLayout(false);
            this.tcs_tabPage.ResumeLayout(false);
            this.tes_tabPage.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage vs_tabPage1;
        private System.Windows.Forms.TabPage gs_tabPage2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.RichTextBox richTextBox2;
        private System.Windows.Forms.TabPage fs_tabPage4;
        private System.Windows.Forms.RichTextBox richTextBox4;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripProgressBar toolStripProgressBar1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.TabPage tcs_tabPage;
        private System.Windows.Forms.TabPage tes_tabPage;
        private System.Windows.Forms.RichTextBox richTextBox3;
        private System.Windows.Forms.RichTextBox richTextBox5;
    }
}

