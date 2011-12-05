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

            Console.WriteLine("Voodoo Shader Framework GUI");
            Console.WriteLine("version 0.5.0");
            Console.WriteLine("Copyright (c) 2010-2011 by Sean Sube, All Rights Reserved. See COPYING for license details.");
            Console.WriteLine("");

            // Command line mode
            string help_cmd = null, run_hook = null, run_file = null;
            bool verbose = false, help = false, update_self = false;
            List<String> sync = new List<String>();
            List<String> install = new List<String>();
            List<String> update = new List<String>();
            List<String> remove = new List<String>();
            List<HookDef> hooks_add = new List<HookDef>();

            var options = new OptionSet();
            options.Add("v|verbose", "display detailed messages", v => { verbose = true; });
            options.Add("h|?|help", "show this help and exit", v => { help = v != null; });
            options.Add("c|command=", "show help for a specific {COMMAND}", v => help_cmd = v);
            options.Add("s|sync=", "download manifests from the given {URI}", v => sync.Add(v));
            options.Add("i|install=", "install the given {PACKAGE}", v => install.Add(v));
            options.Add("u|update=", "updates the given {PACKAGE} and/or the gui", v => { if (v != null && v == "<self>") { update_self = true; } else { update.Add(v); }});
            options.Add("r|remove=", "removes the given {PACKAGE}", v => remove.Add(v));
            options.Add("run=", "run the named {HOOK}, passing all extra arguments to the process", v => run_hook = v);
            options.Add("runfile=", "run the given {FILE}, passing all extra arguments to the process", v => run_file = v);
            //options.Add("addhook=", "create a new hook with the given {TARGET}, {CONFIG} and {NAME}", (t, c, n) => hooks_add.Add({t, c, n}));
            //options.Add("removehook=");

            List<String> extras;
            try
            {
                extras = options.Parse(args);
            }
            catch (OptionException e)
            {
                Console.WriteLine("Arguments error: ");
                Console.WriteLine("\t" + e.Message);
                Console.WriteLine("Try `voodoo_ui --help` for more information.");
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

            if (update.Count > 0)
            {
                foreach (String u in update)
                {
                    Console.WriteLine("Update: {0}", u);
                }
            }

            if (help)
            {
                PrintHelp(options, help_cmd);
                return;
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

        static void PrintHelp(OptionSet p, String command)
        {
            if (command == "sync")
            {
                Console.WriteLine("voodoo_ui.exe sync [remote [...]]");
                Console.WriteLine("Downloads manifests from the given list, or known remotes if none are given.");
            }
            else if (command == "install")
            {
                Console.WriteLine("voodoo_ui.exe install [-v|--verbose] <package> [...]");
                Console.WriteLine("Attempt to install the given packages. List may be name or PackId.");
            }
            else if (command == "update")
            {
                Console.WriteLine("voodoo_ui.exe update [-v|--verbose] [-self|<package> [...]]");
                Console.WriteLine("Checks for and installs updates to all packages or the given list. List may be named or PackId.");
                Console.WriteLine("\tIf -self is given, checks for GUI updates instead of packages.");
            }
            else if (command == "remove")
            {
                Console.WriteLine("voodoo_ui.exe remove [-v|--verbose] <package> [...]");
                Console.WriteLine("Attempt to uninstall the given packages. List may be named or PackId.");
            }
            else if (command == "run")
            {
                Console.WriteLine("voodoo_ui.exe run [-s|--suspend] <name> [<args>]");
                Console.WriteLine("Run the named hook's target. All arguments after the name are passed to the process.");
                Console.WriteLine("\tIf -suspend is given, start the process suspended and forcibly inject the loader.");
            }
            else if (command == "runfile")
            {
                Console.WriteLine("voodoo_ui.exe run [-s|--suspend] <name> [<args>]");
                Console.WriteLine("Run the given file. All arguments after the path are passed to the process.");
                Console.WriteLine("\tIf -suspend is given, start the process suspended and forcibly inject the loader.");
            }
            else if (command == "addhook")
            {
                Console.WriteLine("voodoo_ui.exe addhook <target> [<config> [<name>]]");
                Console.WriteLine("Create a new hook with the given target.");
                Console.WriteLine("\tIf no config or name is given, a default will be searched for.");
                Console.WriteLine("\tIf a target and config are given, the target will be used as the name.");
            }
            else if (command == "removehook ")
            {
                Console.WriteLine("voodoo_ui.exe removehook <name> [...]");
                Console.WriteLine("Remove the named hooks.");
            }
            else
            {
                Console.WriteLine("Options:");
                p.WriteOptionDescriptions(Console.Out);
            }
        }
    }
}
