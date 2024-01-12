namespace MTEOEMPLauncher
{
    partial class LauncherWindow
    {
        /// <summary>
        /// Variável de designer necessária.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpar os recursos que estão sendo usados.
        /// </summary>
        /// <param name="disposing">true se for necessário descartar os recursos gerenciados; caso contrário, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código gerado pelo Windows Form Designer

        /// <summary>
        /// Método necessário para suporte ao Designer - não modifique 
        /// o conteúdo deste método com o editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.IPtb = new System.Windows.Forms.TextBox();
            this.IPlbl = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.Porttb = new System.Windows.Forms.TextBox();
            this.Portlbl = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.exitBtn = new System.Windows.Forms.Button();
            this.launchBtn = new System.Windows.Forms.Button();
            this.Nametb = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // IPtb
            // 
            this.IPtb.Location = new System.Drawing.Point(35, 19);
            this.IPtb.Name = "IPtb";
            this.IPtb.Size = new System.Drawing.Size(100, 20);
            this.IPtb.TabIndex = 0;
            this.IPtb.Text = "127.0.0.1";
            // 
            // IPlbl
            // 
            this.IPlbl.AutoSize = true;
            this.IPlbl.Location = new System.Drawing.Point(15, 22);
            this.IPlbl.Name = "IPlbl";
            this.IPlbl.Size = new System.Drawing.Size(17, 13);
            this.IPlbl.TabIndex = 1;
            this.IPlbl.Text = "IP";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.Porttb);
            this.groupBox1.Controls.Add(this.Portlbl);
            this.groupBox1.Controls.Add(this.IPtb);
            this.groupBox1.Controls.Add(this.IPlbl);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(200, 74);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Connection";
            // 
            // Porttb
            // 
            this.Porttb.Location = new System.Drawing.Point(35, 43);
            this.Porttb.Name = "Porttb";
            this.Porttb.Size = new System.Drawing.Size(100, 20);
            this.Porttb.TabIndex = 2;
            this.Porttb.Text = "56000";
            // 
            // Portlbl
            // 
            this.Portlbl.AutoSize = true;
            this.Portlbl.Location = new System.Drawing.Point(7, 46);
            this.Portlbl.Name = "Portlbl";
            this.Portlbl.Size = new System.Drawing.Size(26, 13);
            this.Portlbl.TabIndex = 3;
            this.Portlbl.Text = "Port";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.exitBtn);
            this.groupBox2.Controls.Add(this.launchBtn);
            this.groupBox2.Controls.Add(this.Nametb);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Location = new System.Drawing.Point(218, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(200, 74);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "User";
            // 
            // exitBtn
            // 
            this.exitBtn.Location = new System.Drawing.Point(110, 47);
            this.exitBtn.Name = "exitBtn";
            this.exitBtn.Size = new System.Drawing.Size(85, 23);
            this.exitBtn.TabIndex = 6;
            this.exitBtn.Text = "Exit";
            this.exitBtn.UseVisualStyleBackColor = true;
            this.exitBtn.Click += new System.EventHandler(this.exitBtn_Click);
            // 
            // launchBtn
            // 
            this.launchBtn.Location = new System.Drawing.Point(18, 46);
            this.launchBtn.Name = "launchBtn";
            this.launchBtn.Size = new System.Drawing.Size(85, 23);
            this.launchBtn.TabIndex = 5;
            this.launchBtn.Text = "Launch Game";
            this.launchBtn.UseVisualStyleBackColor = true;
            this.launchBtn.Click += new System.EventHandler(this.LaunchButtonClick);
            // 
            // Nametb
            // 
            this.Nametb.Location = new System.Drawing.Point(48, 19);
            this.Nametb.Name = "Nametb";
            this.Nametb.Size = new System.Drawing.Size(100, 20);
            this.Nametb.TabIndex = 0;
            this.Nametb.Text = "Player";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 22);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Name";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(424, 93);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.ShowIcon = false;
            this.Text = "Monster Trux Extreme: Offroad Edition Multiplayer";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox IPtb;
        private System.Windows.Forms.Label IPlbl;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox Porttb;
        private System.Windows.Forms.Label Portlbl;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button launchBtn;
        private System.Windows.Forms.TextBox Nametb;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button exitBtn;
    }
}

