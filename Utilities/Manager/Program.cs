using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using Voodoo_Core;

namespace Installer
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            IVoodooCore core = new VoodooCore();
            try
            {
                core.Initialize("M:\\VoodooShader\\Test\\VoodooBasic.xmlconfig");

                IVoodooParser parser = core.Parser;
                parser.AddVariable("language", "C#");
                parser.AddVariable("date", System.Convert.ToString(System.DateTime.Today));

                String parsed = parser.Parse
                (
                    "On today, $(date), we USE THE PROGRAMMING language $(language).",
                    ParseFlags.Lowercase
                );

                System.Windows.Forms.MessageBox.Show(parsed);
            }
            catch (COMException ex)
            {
                System.Windows.Forms.MessageBox.Show(String.Format("Exception:\nMsg: {0}\nEC: {1}\nStack: {2}", ex.Message, ex.ErrorCode, ex.StackTrace));
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
