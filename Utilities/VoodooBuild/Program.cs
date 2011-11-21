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
            String solutionDir = args[0].Trim();
            String buildFile = solutionDir + @"\Framework\Core\Version_Build.hpp";

            Int32 buildCount = 0;

            String[] lines = null;
            DateTime lastRun = new DateTime(0);

            try
            {
                if (File.Exists(buildFile))
                {
                    lines = File.ReadAllLines(buildFile);
                    if (lines.Length == 3)
                    {
                        String lastTime = lines[0].Substring(3);
                        lastRun = Convert.ToDateTime(lastTime);

                        Regex buildCountExpr = new Regex(@"\#define VOODOO_GLOBAL_VERSION_BUILD (?<count>[0-9]+)");
                        String buildLine = buildCountExpr.Match(lines[1]).Groups["count"].Value;
                        buildCount = Convert.ToInt32(buildLine);
                    }
                }
            }
            catch (Exception) {  }

            if ((DateTime.Now - lastRun).TotalHours > 1)
            {
                ++buildCount;
            }

            Process p = new Process();
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.FileName = "cmd";
            p.StartInfo.Arguments = "/c git describe";
            p.Start();
            String gitDesc = p.StandardOutput.ReadToEnd();
            p.WaitForExit();

            Console.Write("Time since last counted build: {0}\nBuild number: {1}\nGit ID: {2}", DateTime.Now - lastRun, buildCount, gitDesc);

            lines = new String[3];
            lines[0] = String.Format("// {0}", DateTime.Now);
            lines[1] = String.Format("#define VOODOO_GLOBAL_VERSION_BUILD {0}", buildCount);
            lines[2] = String.Format("#define VOODOO_GLOBAL_GITID VOODOO_META_STRING(\"{0}\")", gitDesc.TrimEnd('\n'));
            File.WriteAllLines(buildFile, lines);
        }
    }
}