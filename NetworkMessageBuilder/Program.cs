using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MessageBuilderLib;

namespace NetworkMessage
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("NetworkMessageBuilder run");

            string outputDir = "./";
            if (args.Length > 0)
            {
                outputDir = args[0];
            }

            string includeFile = null;
            if (args.Length > 1)
            {
                includeFile = args[1];
            }

            Console.WriteLine(outputDir);
            Console.WriteLine(includeFile);

            AppDomain.CurrentDomain.Load("NetworkMessage");
            SchemaBox.Parse();

            GroupWriter groupWriter = new GroupWriter();
            groupWriter.GenerateCode(outputDir, includeFile);
        }
    }
}
