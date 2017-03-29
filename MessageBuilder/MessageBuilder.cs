using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    public static class MessageBuilder
    {
        public static void Build(string outputDir, string includeFile)
        {
            SchemaBox.Parse();

            GroupWriter groupWriter = new GroupWriter();
            groupWriter.GenerateCode(outputDir, includeFile);
        }
    }
}
