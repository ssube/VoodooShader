using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace VoodooUuid
{
    class Program
    {
        [DllImport("rpcrt4.dll", SetLastError = true)]
        static extern int UuidCreateSequential(out Guid guid);

        [STAThread]
        static void Main(string[] args)
        {
            int count = Convert.ToInt32(args[0]);
            bool sequential = (args.Length > 1) ? args[1] == "seq" : false;

            String uuids = "/* Voodoo UUID Gen */\n";
            for (int i = 0; i < count; ++i)
            {
                Guid guid;
                if (sequential)
                {
                    UuidCreateSequential(out guid);
                } else {
                    guid = Guid.NewGuid();
                }
                
                Console.WriteLine("UUID {0}: {1}", i, guid);
                
                uuids += String.Format("/* {0} */\nDEFINE_UUID(uuid_{1}", guid, i);
                byte[] bytes = guid.ToByteArray();
                foreach (byte b in bytes)
                {
                    uuids += ", 0x"+b.ToString("X2");
                }
                uuids += ");\n";
            }

            System.Windows.Forms.Clipboard.SetText(uuids);
        }
    }
}
