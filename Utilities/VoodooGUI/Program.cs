using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using NDesk.Options;

namespace VoodooGUI
{
    static class Program
    {
        struct HookDef
        {
            String Target, Config, Name;
        }

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

                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new MainForm());

                ConsoleVisible(Console.Title, true);
            }

            // Command line mode
            string run_hook = null, run_file = null;
            bool verbose = false, help = false, version = false, logo = false, nologo = false, update_self = false;
            List<String> sync = new List<String>();
            List<String> install = new List<String>();
            List<String> update = new List<String>();
            List<String> remove = new List<String>();
            List<HookDef> hooks_add = new List<HookDef>();

            OptionSet options = new OptionSet();
            options.Add("v|verbose", "display detailed messages", v => { verbose = true; });
            options.Add("n|no|nologo", "hide version info", v => nologo = v != null);
            options.Add("version", "show the version info and exit", v => version = v != null);
            options.Add("logo", "show the version info and exit", v => logo = v != null);
            options.Add("h|?|help", "show help and exit", v => help = v != null);
            options.Add("s|sync:", "download manifests from the given {URI}, or all listed remotes if none specified", v => sync.Add(v));
            options.Add("r|remove=", "removes the given {PACKAGE}", v => remove.Add(v));
            options.Add("i|install=", "install the given {PACKAGE}", v => install.Add(v));
            options.Add("u|update:", "updates the given {PACKAGE} or the gui if {PACKAGE}='this', or all installed packages if none specified", 
                v => { if (v != null && v == "this") { update_self = true; } else { update.Add(v); }});
            options.Add("runhook=", "run the named {HOOK}, passing all extra arguments to the process", v => run_hook = v);
            options.Add("runfile=", "run the given {FILE}, passing all extra arguments to the process", v => run_file = v);

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
                Console.WriteLine("Try `voodoo_ui --help` for more information.");
                return;
            }

            if (version)
            {
                Console.WriteLine("VoodooGUI-0.5.0");
                return;
            }
            else if (!nologo)
            {
                Console.WriteLine("Voodoo Shader Framework GUI");
                Console.WriteLine("version 0.5.0");
                Console.WriteLine("Copyright (c) 2010-2011 by Sean Sube, All Rights Reserved. See COPYING for license details.");
                Console.WriteLine("");

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

            if (sync.Count > 0)
            {
                foreach (String s in sync)
                {
                    Console.WriteLine("Sync: {0}", s);
                }
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
                foreach (String i in install)
                {
                    Console.WriteLine("Install: {0}", i);
                }
            }

            if (update_self)
            {
                Console.WriteLine("Update GUI");
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
