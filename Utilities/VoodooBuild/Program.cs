﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace VoodooBuild
{
    class Program
    {
        static void Main(string[] args)
        {
            String buildFile = args[0].Trim();

            bool ignoreTime = args.Length > 1 ? Convert.ToBoolean(args[1]) : false;

            Int32 buildCount = 0;

            String[] lines = null;
            DateTime lastRun = new DateTime(0);

            try
            {
                if (File.Exists(buildFile))
                {
                    lines = File.ReadAllLines(buildFile);
                    if (lines.Length > 2)
                    {
                        String lastTime = lines[1].Substring(3);
                        lastRun = Convert.ToDateTime(lastTime);

                        Regex buildCountExpr = new Regex(@"\#define VOODOO_PROJECT_VERSION_BUILD (?<count>[0-9]+)");
                        String buildLine = buildCountExpr.Match(lines[3]).Groups["count"].Value;
                        buildCount = Convert.ToInt32(buildLine);
                    }
                }
            }
            catch (Exception ex) 
            {
                Console.WriteLine("VoodooBuild: Error reading file: " + ex.Message);
            }

            if (ignoreTime || ((DateTime.UtcNow - lastRun).TotalHours > 1))
            {
                ++buildCount;

                Process p = new Process();
                p.StartInfo.UseShellExecute = false;
                p.StartInfo.RedirectStandardOutput = true;
                p.StartInfo.FileName = "cmd";
                p.StartInfo.Arguments = "/c git describe";
                p.Start();
                String gitDesc = p.StandardOutput.ReadToEnd().TrimEnd('\n');
                p.WaitForExit();

                Console.WriteLine("Time since last counted build: {0}", DateTime.UtcNow - lastRun);
                Console.WriteLine("Build number: {0}", buildCount);
                Console.WriteLine("Git ID: {0}", gitDesc);

                lines = new String[4];
                lines[0] = "// This file is automatically generated by the VoodooBuild tool, to track git revision and build count. Any changes will be lost.";
                lines[1] = String.Format("// {0}", DateTime.UtcNow);
                lines[2] = String.Format("#define VOODOO_PROJECT_VERSION_ID VSTR(\"{0}\")", gitDesc);
                lines[3] = String.Format("#define VOODOO_PROJECT_VERSION_BUILD {0}", buildCount);
                File.WriteAllLines(buildFile, lines);

                Console.WriteLine("VoodooBuild: Updated version header.");
            }
            else
            {
                Console.WriteLine("VoodooBuild: No header update needed.");
            }
        }
    }
}
