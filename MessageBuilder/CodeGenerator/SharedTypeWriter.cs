using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    class SharedTypeWriter
    {
        public void Run(string dirpath, string includeFile = null)
        {
            SharedTypeBox.Parse();
            GenerateCode(dirpath, includeFile);
        }

        void GenerateCode(string dirpath, string includeFile = null)
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

            foreach (var shareFile in SharedTypeBox.GetSharedFiles())
            {
                var filepath = dirpath + "/" + shareFile.filename + ".shared.h";
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

                    foreach (var sharedNamespace in shareFile.namespaces)
                    {
                        // file
                        gen.print($"namespace {sharedNamespace.namespaceName}");
                        using (gen.br())
                        {
                            var types = sharedNamespace.type.GetNestedTypes();

                            // messages
                            foreach (var msgT in sharedNamespace.type.GetNestedTypes())
                            {
                                if (msgT.IsEnum)
                                {
                                    MessageWriter.PrintEnumType(gen, msgT);
                                }
                                else
                                {
                                    MessageWriter.PrintMessage(gen, msgT);
                                }
                                gen.print("");
                            }
                        }
                    }

                }
            }
        } //GenerateCode

    }
}
