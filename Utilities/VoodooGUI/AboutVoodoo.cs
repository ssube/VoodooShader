using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Reflection;
using System.Windows.Forms;

namespace VoodooGUI
{
    public partial class AboutVoodoo : Form
    {
        public AboutVoodoo()
        {
            InitializeComponent();

            try
            {
                Assembly assembly = Assembly.GetExecutingAssembly();
                Stream textStream = assembly.GetManifestResourceStream("VoodooGUI.Resources.about.rtf");
                StreamReader textStreamReader = new StreamReader(textStream);
                richTextBox1.LoadFile(textStream, RichTextBoxStreamType.RichText);
                textStreamReader.Close();
            }
            catch (Exception exc)
            {
                richTextBox1.Text = "Voodoo Shader Framework\nCopyright (c) 2010-2011 by Sean Sube, All Rights Reserved.\n\nError loading info:\n\t" + exc.Message;
            }
        }

        private void LinkClicked(object sender, LinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start(e.LinkText);
        }
    }
}
