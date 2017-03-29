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
            if (args.Length < 1)
            {
                Console.WriteLine("need input args: [dirpath] (optional)[includefile]");
                return;
            }

            string outputDir = args[0];

            string includeFile = null;
            if (args.Length > 1)
            {
                includeFile = args[1];
            }

            MessageBuilderLib.MessageBuilder.Build(outputDir, includeFile);
        }
    }
}
