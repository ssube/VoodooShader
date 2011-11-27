using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;
using System.Xml.Xsl;
using System.Diagnostics;

namespace VoodooLogViewer
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                Console.WriteLine
                (
                    "Voodoo Log Viewer\n"+
                    "  Formats and displays Xml log files from Voodoo/XmlLogger."+
                    "Usage:\n"+
                    "  VoodooLogViewer.exe <log file> [, ...]"
                );
                return;
            }

            String resourceDir = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().CodeBase) + @"\Resources\";
            if (resourceDir.StartsWith("file:\\"))
            {
                resourceDir = resourceDir.Substring(6);
            }
            
            XslCompiledTransform xslTransform;
            try
            {
                XsltSettings settings = new XsltSettings();
                settings.EnableScript = true;

                xslTransform = new XslCompiledTransform(true);
                xslTransform.Load(resourceDir + "VoodooXmlLog.xsl", settings, new XmlUrlResolver());
            }
            catch (System.Exception exc)
            {
                Console.WriteLine("Unable to compile XSL transform: {0}", exc.Message);
                return;
            }

            for (int i = 0; i < args.Length; ++i)
            {
                try
                {
                    String tempFile = resourceDir + Path.GetFileNameWithoutExtension(Path.GetTempFileName()) + ".html";
                    Console.Write("Processing file '{0}' to '{1}'... ", args[i], tempFile);

                    xslTransform.Transform(args[i], tempFile);
                    Console.WriteLine("succeeded.");

                    if (i == 1)
                    {
                        ProcessStartInfo startParams = new ProcessStartInfo(tempFile);
                        startParams.UseShellExecute = true;

                        Process.Start(startParams);
                    }
                }
                catch (Exception exc)
                {
                    Console.WriteLine("failed to convert log {0}: {1}", i, exc.Message);
                }
            }
        }
    }
}
