/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
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
using System.Runtime.InteropServices;
using System.Windows.Forms;
using NDesk.Options;
using VoodooSharp;
using System.Globalization;
using System.Threading;
using System.Deployment.Application;

namespace VoodooGUI
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
        [STAThread]
        static void Main(string[] args)
        {
            Console.Title = "Voodoo GUI";

            if (args == null || args.Length == 0)
            {
                ConsoleVisible(Console.Title, false);

                // Get the culture
                string languageID = VoodooRegistry.Instance.Language;
                try
                {
                    CultureInfo culture = new CultureInfo(languageID);
                    Thread.CurrentThread.CurrentCulture = culture;
                    Thread.CurrentThread.CurrentUICulture = culture;
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(String.Format("Error setting language to {0}, defaulting to en-US.\n{1}", languageID, ex.Message), "Language Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new MainForm());

                ConsoleVisible(Console.Title, true);
            }

            // Command line mode
            string run_hook = null, run_file = null;
            bool verbose = false, help = false, version = false, logo = false, nologo = false, sync_all = false, update_all = false, list = false;
            List<String> sync = new List<String>();
            Dictionary<String, String> install = new Dictionary<String, String>();
            Dictionary<String, String> installzip = new Dictionary<String, String>();
            List<String> update = new List<String>();
            List<String> remove = new List<String>();

            OptionSet options = new OptionSet();
            options.Add("v|version",      "show the version info and exit",
                v => version = v != null);
            options.Add("h|help",       "show help and exit",
                v => help = v != null);
            options.Add("l|list",       "list installed packages",
                v => list = v != null);
            options.Add("r|remove=",    "removes the given {PACKAGE}",
                v => remove.Add(v));
            options.Add("s|sync:",      "download manifests from the given {URI}, or all known remotes if none specified",
                v => { if (v == null) { sync_all = true; } else { sync.Add(v); } });
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
                Console.WriteLine("Arguments error: ");
                Console.WriteLine("\t" + e.Message);
                Console.WriteLine("Try voodoo_ui --help for more information.");
                return;
            }

            if (version)
            {
                Console.WriteLine("VoodooUI-0.5.2");
                return;
            }
            else if (!nologo)
            {
                Console.WriteLine("Voodoo Shader Framework GUI");
                Console.WriteLine("version 0.5.2");
                Console.WriteLine("Copyright (c) 2010-2011 by Sean Sube, All Rights Reserved. See COPYING for license details.\n");

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

            if (sync.Count > 0 || sync_all)
            {
                Console.WriteLine("Checking for Voodoo UI updates...");

                try
                {
                    ApplicationDeployment updateCheck = ApplicationDeployment.CurrentDeployment;
                    UpdateCheckInfo info = updateCheck.CheckForDetailedUpdate();
                    if (info.UpdateAvailable)
                    {
                        if (info.IsUpdateRequired)
                        {
                            Console.WriteLine("A critical update to the Voodoo UI is available.");
                        }
                        else
                        {
                            Console.WriteLine("An optional update to the Voodoo UI is available.");
                        }

                        updateCheck.Update();
                        Application.Restart();
                    }
                }
                catch (Exception exc)
                {
                    Console.WriteLine("An error occurred while updating the UI:\n\t{0}", exc.Message);
                }

                if (sync_all)
                {

                }
                foreach (String s in sync)
                {
                    Console.WriteLine("Sync: {0}", s);
                }
            }

            if (install.Count > 0)
            {
                foreach (String key in install.Keys)
                {
                    Console.WriteLine("Install: {0}: {1}", key, install[key]);
                }
            }

            if (installzip.Count > 0)
            {
                foreach (String key in installzip.Keys)
                {
                    Console.WriteLine("Install Zip: {0}: {1}", key, installzip[key]);
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
            }
        }

        static void ConsoleVisible(String title, bool visible)
        {
            IntPtr hWnd = FindWindow(null, title);
            if (hWnd != IntPtr.Zero)
            {
                ShowWindow(hWnd, visible ? 1 : 0);
            }
        }
    }
}
