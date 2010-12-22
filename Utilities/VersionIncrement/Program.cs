using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace VersionIncrement
{
    class Program
    {
        static void Main(string[] args)
        {
            if ( args.Length < 2 || args.Length > 3 )
            {
                Console.WriteLine("Invalid number of arguments (" + args.Length + ").");
                return;
            }

            String filename = args[0];
            String mode = args[1];
            String amount = args[2];

            String[] lines = System.IO.File.ReadAllLines(filename);
            Char[] delims = {' ', '\t' };
            int curline = 0;

            while ( curline < lines.Length )
            {
                String line = lines[curline];
                String[] chunks = line.Split(delims);

                if ( chunks.Length == 3 && chunks[0] == "#define" )
                {
                    Console.WriteLine("Found def " + chunks[1]);

                    if (chunks[1] == mode)
                    {
                        Console.WriteLine("\tFound target def, has value "+chunks[2]);

                        try
                        {
                            int shift = Convert.ToInt32(amount);
                            int current = Convert.ToInt32(chunks[2]);

                            current += shift;

                            Console.WriteLine("\tAdjusted value to " + curline + ".");
                            lines[curline] = chunks[0] + " " + chunks[1] + "\t" + current;
                        } catch (Exception exc){
                            Console.WriteLine("Error shifting value: " + exc.Message);
                        }
                    }
                } else {
                    Console.WriteLine("Not a def/invalid: " + line);
                }

                ++curline;
            }

            Console.WriteLine("Done adjusting, writing file.");

            System.IO.File.WriteAllLines(filename, lines);
        }
    }
}
