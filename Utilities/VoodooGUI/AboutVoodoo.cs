using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Reflection;
using System.Windows.Forms;

namespace VoodooUI
{
    public partial class AboutVoodoo : Form
    {
        private bool m_NoNav = false;

        public AboutVoodoo()
        {
            InitializeComponent();

            try
            {
                Assembly assembly = Assembly.GetExecutingAssembly();
                Stream textStream = assembly.GetManifestResourceStream("VoodooUI.Resources.about.md");
                StreamReader textStreamReader = new StreamReader(textStream);

                String text = new StreamReader(textStream).ReadToEnd();

                MarkdownSharp.MarkdownOptions mdopt = new MarkdownSharp.MarkdownOptions();
                mdopt.AutoHyperlink = true; mdopt.LinkEmails = true;
                MarkdownSharp.Markdown parser = new MarkdownSharp.Markdown(mdopt);

                text = parser.Transform(text);
                webBrowser1.DocumentText = text;

                textStreamReader.Close();
            }
            catch (Exception exc)
            {
                webBrowser1.DocumentText = "Voodoo Shader Framework<br>\nCopyright (c) 2010-2011 by Sean Sube, All Rights Reserved.<br>\n<br>\nError loading info:<br>\n\t" + exc.Message;
            }

            m_NoNav = true;
        }

        private void WebNavigating(object sender, WebBrowserNavigatingEventArgs e)
        {
            if (m_NoNav)
            {
                System.Diagnostics.Process.Start(e.Url.ToString());
                e.Cancel = true;
            }
        }

        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {

        }
    }
}
