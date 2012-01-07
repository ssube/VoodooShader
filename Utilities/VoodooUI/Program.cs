/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
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
using NDesk.Options;
using VoodooSharp;
using System.Reflection;
using System.Diagnostics;

namespace VoodooUI
{
    static class Program
    {
        [DllImport("user32.dll")]
        public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        [DllImport("user32.dll")]
        static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

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

            Console.Title = "Voodoo GUI";

            if (args == null || args.Length == 0)
            {
                ConsoleVisible(Console.Title, false);

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

                ConsoleVisible(Console.Title, true);
            }

            // Command line mode
            string run_hook = null, run_file = null;
            bool help = false, version = false, logo = false, nologo = false, update_all = false, list = false;
            List<String> sync = new List<String>();
            Dictionary<String, String> install = new Dictionary<String, String>();
            Dictionary<String, String> installzip = new Dictionary<String, String>();
            List<String> update = new List<String>();
            List<String> remove = new List<String>();

            OptionSet options = new OptionSet();
            options.Add("v|version",    "show the version info and exit",
                v => version = v != null);
            options.Add("h|help",       "show help and exit",
                v => help = v != null);
            options.Add("l|list",       "list installed packages",
                v => list = v != null);
            options.Add("r|remove=",    "removes the given {PACKAGE}",
                v => remove.Add(v));
            options.Add("i|install=",   "install the given {PACKAGE}, at the optional version",
                v => { int la = v.LastIndexOf('@'); if (la == -1) { install[v] = null; } else { install[v.Substring(0, la)] = v.Substring(la + 1); } });
            options.Add("z|zip=",       "install a valid zip {ARCHIVE} as a package, at the optional version",
                v => { int la = v.LastIndexOf('@'); if (la == -1) { installzip[v] = null; } else { installzip[v.Substring(0, la)] = v.Substring(la + 1); } });
            options.Add("u|update:",    "updates the given {PACKAGE}, or all installed packages if none specified",
                v => { if (v == null) { update_all = true; } else { update.Add(v); } });
            options.Add("runhook=",     "run the named {HOOK}, passing all extra arguments to the process", 
                v => run_hook = v);
            options.Add("runfile=",     "run the given {FILE}, passing all extra arguments to the process", 
                v => run_file = v);

            List<String> extras;
            try
            {
                extras = options.Parse(args);
            }
            catch (OptionException e)
            {
                Console.WriteLine("Voodoo Shader Framework GUI");
                Console.WriteLine("  Error: {0}", e.Message);
                Console.WriteLine("Try voodoo_ui --help for more information.");
                return;
            }

            if (version)
            {
                Console.WriteLine("VoodooUI-0.5.4");
                return;
            }
            else if (!nologo)
            {
                Console.WriteLine("Voodoo Shader Framework GUI");
                Console.WriteLine("version 0.5.4");
                Console.WriteLine("Copyright (c) 2010-2012 by Sean Sube, All Rights Reserved. See COPYING for license details.\n");

                if (logo)
                {
                    return;
                }
            }

            if (help)
            {
                Console.WriteLine("Options:");
                options.WriteOptionDescriptions(Console.Out);
                return;
            }

            if (remove.Count > 0)
            {
                foreach (String r in remove)
                {
                    Console.WriteLine("Remove: {0}", r);
                }
            }

            if (install.Count > 0)
            {
                foreach (String key in install.Keys)
                {
                    String target = install[key];
                    Console.WriteLine("Install: {0}: {1}", key, target);

                }
            }

            if (installzip.Count > 0)
            {
                foreach (String key in installzip.Keys)
                {
                    Console.WriteLine("Install Zip [NOT IMPLEMENTED]: {0}: {1}", key, installzip[key]);
                }
            }

            if (update_all)
            {
                Console.WriteLine("Update All");
            }

            if (update.Count > 0)
            {
                foreach (String u in update)
                {
                    Console.WriteLine("Update: {0}", u);
                }
            }

            if (run_hook != null)
            {
                Console.WriteLine("Run Hook: {0}", run_hook);
                return;
            }

            if (run_file != null)
            {
                Console.WriteLine("Run File: {0}", run_file);
                return;
            }
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

        static void pm_OnLogEvent(string msg, params object[] args)
        {
            Console.WriteLine(msg, args);
        }

        static void ConsoleVisible(String title, bool visible)
        {
            IntPtr hWnd = FindWindow(null, title);
            if (hWnd != IntPtr.Zero)
            {
                ShowWindow(hWnd, visible ? 1 : 0);
            }
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
