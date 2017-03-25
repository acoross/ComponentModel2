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
            SharedTypeWriter sharedWriter = new SharedTypeWriter();
            sharedWriter.Run(outputDir, includeFile);

            ProtocolWriter protocolWriter = new ProtocolWriter();
            protocolWriter.Run(outputDir, includeFile);
        }
    }
}
