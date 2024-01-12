using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace MTEOEMPLauncher
{
    public class INIConfig
    {
        //public static readonly string Format = "[Online]\nMultiplayer = 1\nIP = %s\nName = %s\nPort = %d\n[Input]\nCustomInput = %d\nAccelerateKey = %d\nBrakeKey = %d\nLeftKey = %d\nRightKey = %d";

        public string IP = "127.0.0.1";
        public string Name = "Player";
        public int Port = 56000;
        // Input
        public int CustomInput = 0;
        public int AccelerateKey = 13;
        public int BrakeKey = 161;
        public int LeftKey = 37;
        public int RightKey = 39;

        private string GetConfigurationValue(string entry)
        {
            string[] entries = entry.Split('=');
            if (entries.Length > 1)
            {
                return entries[1].Replace(" ", "");
            }
            else
            {
                return null;
            }
        }

        private string GetConfigurationName(string entry)
        {
            string[] entries = entry.Split('=');
            if (entries.Length > 0)
            {
                return entries[0].Replace(" ", "");
            }
            else
            {
                return null;
            }
        }

        private void ParseLine(string line)
        {
            string name = GetConfigurationName(line);
            string value = GetConfigurationValue(line);

            switch (name)
            {
                case "IP":
                    IP = value;
                    break;
                case "Name":
                    Name = value;
                    break;
                case "Port":
                    Port = Convert.ToInt32(value);
                    break;
                case "CustomInput":
                    CustomInput = Convert.ToInt32(value);
                    break;
                case "AccelerateKey":
                    AccelerateKey = Convert.ToInt32(value);
                    break;
                case "BrakeKey":
                    BrakeKey = Convert.ToInt32(value);
                    break;
                case "LeftKey":
                    LeftKey = Convert.ToInt32(value);
                    break;
                case "RightKey":
                    RightKey = Convert.ToInt32(value);
                    break;
            }
        }

        public void Save(string filename)
        {
            TextWriter tr = File.CreateText(filename);
            string w = $"[Online]\nMultiplayer = 1\nIP = {IP}\nName = {Name}\nPort = {Port}\n[Input]\nCustomInput = {CustomInput}\nAccelerateKey = {AccelerateKey}\nBrakeKey = {BrakeKey}\nLeftKey = {LeftKey}\nRightKey = {RightKey}";

            tr.Write(w);
            tr.Close();
        }

        public void Load(string filename)
        {
            TextReader textReader = File.OpenText(filename);

            while (true)
            {
                string ln = textReader.ReadLine();
                if (ln == null)
                {
                    break;
                }
                ParseLine(ln);
            }

            textReader.Close();
        }
    }

    public partial class LauncherWindow : Form
    {
        public INIConfig ini = new INIConfig();
        public readonly string configFileName = "MTEOEHook.ini";
        public readonly string gameFileName = "Monster Trux Extreme - Offroad Edition.exe";

        public LauncherWindow()
        {
            InitializeComponent();
            this.CenterToScreen();
            if (File.Exists(configFileName)) {
                try
                {
                    ini.Load(configFileName);

                    IPtb.Text = ini.IP;
                    Porttb.Text = ini.Port.ToString();
                    Nametb.Text = ini.Name;
                }
                catch (Exception ex)
                {
                    var st = new StackTrace(ex, true);
                    var frame = st.GetFrame(0);
                    var line = frame.GetFileLineNumber();

                    MessageBox.Show(configFileName + " could not be loaded!\n\nException Message:\n"+ex.Message+"\nLine: "+line, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show(configFileName + " was not found", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void LaunchButtonClick(object sender, EventArgs e)
        {
            if (!File.Exists(gameFileName))
            {
                MessageBox.Show(gameFileName + " was not found!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            else
            {
                ini.IP = IPtb.Text;
                ini.Port = Convert.ToUInt16(Porttb.Text);
                ini.Name = Nametb.Text;

                ini.Save(configFileName);
                this.Close();
                Process.Start(gameFileName,"-quiet");
            }
        }

        private void exitBtn_Click(object sender, EventArgs e)
        {
            Environment.Exit(0);
        }
    }
}
