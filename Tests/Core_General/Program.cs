using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CoreLib;

namespace Core_General
{
    class Program
    {
        static void Main(string[] args)
        {
            IVoodooCore core = new VoodooCore();
            core.Initialize(@"M:\VoodooShader\VoodooConfig.xmlconfig");

            IVoodooParser parser = core.Parser;
            parser.AddVariable("language", "C#", 0);
            String parsed = parser.Parse("Today WE Use $(language).", ParseFlags.PF_Lowercase);

            Console.WriteLine("Output: '{0}'", parsed); // Will be "Output: 'today we use c#.'"
        }
    }
}
