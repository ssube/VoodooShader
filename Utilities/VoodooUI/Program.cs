/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Runtime.InteropServices;
using System.Security.Permissions;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Reflection;
using System.Diagnostics;
using VoodooSharp;

namespace VoodooUI
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread, SecurityPermission(SecurityAction.Demand, Flags=SecurityPermissionFlag.ControlAppDomain)]
        static void Main(string[] args)
        {
            AppDomain domain = AppDomain.CurrentDomain;
            if (domain != null)
            {
                domain.UnhandledException += new UnhandledExceptionEventHandler(domain_UnhandledException);
            }

            // Get the culture
            string languageID = GlobalRegistry.Instance.Language;
            try
            {
                CultureInfo culture = new CultureInfo(languageID);
                Thread.CurrentThread.CurrentCulture = culture;
                Thread.CurrentThread.CurrentUICulture = culture;
            }
            catch (System.Exception)
            {
                MessageBox.Show(String.Format("Error setting language to {0}, defaulting to en-US.", languageID), "Language Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }

        static void domain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            Exception exc = (Exception)e.ExceptionObject;
            String report = CreateExceptionReport(exc);

            MessageBox.Show("The Voodoo Shader UI has encountered a problem and needs to close.\n" +
             "To prevent this error from occurring in the future, please file a bug reports with the details.\n" +
             "Full error details can be found in the file: " + report, 
             "Voodoo UI Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            Environment.Exit(1);
        }

        static String CreateExceptionReport(Exception e)
        {
            StringBuilder data = new StringBuilder();
            data.AppendLine("Voodoo UI Error Report");

            try
            {
                Assembly asm = Assembly.GetExecutingAssembly();
                data.AppendLine("UI File: " + asm.Location);
                FileVersionInfo fvi = FileVersionInfo.GetVersionInfo(asm.Location);
                data.AppendLine("UI Version: " + fvi);
            } 
            catch (Exception exc)
            {
                data.AppendLine("UI Error: " + exc.Message);
            }

            data.AppendLine("CLR Version: " + Environment.Version);
            data.AppendLine("OS Version: " + Environment.OSVersion);

            data.AppendLine("Error Time: " + DateTime.Now);
            data.AppendLine(e.ToString());

            try
            {
                String report = Path.GetTempFileName();
                File.WriteAllText(report, data.ToString(), Encoding.UTF8);
                Process.Start("notepad.exe", report);
                return report;
            }
            catch (Exception)
            {
                return "(error writing report)";
            }
        }
    }
}
