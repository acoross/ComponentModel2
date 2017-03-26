using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    class ProtocolWriter
    {
        public void Run(string outputDir, string includeFile = null)
        {
            ProtocolBox.Parse();
            GenerateCode(outputDir, includeFile);
        }

        public void GenerateCode(string dirpath, string includeFile = null)
        {
            string includeContents = "";
            if (includeFile != null)
            {
                using (var fs = File.Open(includeFile, FileMode.Open))
                {
                    using (var sr = new StreamReader(fs))
                    {
                        includeContents = sr.ReadToEnd();
                    }
                }
            }

            if (!Directory.Exists(dirpath))
            {
                Directory.CreateDirectory(dirpath);
            }

            foreach (var pd in ProtocolBox.GetProtocols())
            {
                var filepath = dirpath + "/" + pd.name + ".message.h";
                using (var fs = File.Create(filepath))
                using (var sw = new StreamWriter(fs))
                {
                    var gen = new CodeWriter(sw);
                    gen.print("#pragma once");
                    gen.print();

                    // include
                    if (includeFile != null)
                    {
                        gen.print(includeContents);
                        gen.print();
                    }

                    // include shared
                    foreach (var shareFile in SharedTypeBox.GetSharedFiles())
                    {
                        gen.print($"#include \"{shareFile.filename}.shared.h\"");
                    }
                    gen.print();

                    // file
                    gen.print($"namespace {pd.name}");
                    using (gen.br())
                    {
                        foreach (var dirData in pd.directionDataMap.Values)
                        {
                            gen.print($"namespace {dirData.direction.ToString()}");
                            using (gen.br())
                            {
                                // enum
                                using (gen.type($"enum class PacketType : uint32"))
                                {
                                    foreach (var msgT in dirData.messageTypes)
                                    {
                                        gen.print($"{msgT.Value.Name}, ");
                                    }
                                    gen.print("MAX");
                                }
                                gen.print();

                                // messages
                                foreach (var msgT in dirData.messageTypes)
                                {
                                    if (msgT.Value.IsEnum)
                                    {
                                        MessageWriter.PrintEnumType(gen, msgT.Value);
                                    }
                                    else
                                    {
                                        MessageWriter.PrintMessage(gen, msgT.Value, msgT.Key);
                                    }
                                    gen.print("");
                                }
                            }
                        }
                    }
                }
            }
        } //GenerateCode

    }
}
