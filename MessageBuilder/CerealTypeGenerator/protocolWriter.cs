using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    class GroupWriter
    {
        string getFileName(string groupName)
        {
            return groupName + ".message.h";
        }

        public void GenerateCode(string dirpath, string includeFile = null)
        {
            if (!Directory.Exists(dirpath))
            {
                Directory.CreateDirectory(dirpath);
            }

            string includeContents = null;
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

            foreach (var group in SchemaBox.GetSchemas())
            {
                var filepath = dirpath + "/" + getFileName(group.Name);
                using (var fs = File.Create(filepath))
                using (var sw = new StreamWriter(fs))
                {
                    var c = new CodeWriter(sw);
                    c.prn("#pragma once");
                    c.prn();

                    // include
                    if (includeContents != null)
                    {
                        c.prn(includeContents);
                        c.prn();
                    }

                    // dependencies
                    foreach (var dep in group.dependencies)
                    {
                        c.prn($"#include \"{getFileName(dep.Name)}\"");
                    }
                    c.prn();

                    // file
                    using (c.func($"namespace {group.Name}"))
                    {
                        // PacketType
                        using (c.type($"enum class PacketType : uint32"))
                        {
                            foreach (var msgT in group.messageMap)
                            {
                                c.prn($"{msgT.Value.Name}, ");
                            }
                            c.prn("MAX");
                        }
                        c.prn();

                        // enums
                        foreach (var eT in group.enumTypes)
                        {
                            EnumWriter writer = new EnumWriter(c, eT);
                            writer.PrintEnumType();
                            c.prn("");
                        }

                        // messages
                        foreach (var msgT in group.messageMap.Values)
                        {
                            MessageWriter writer = new MessageWriter(c, group, msgT);
                            writer.PrintMessage();
                            c.prn("");
                        }
                    }
                }
            }
        } //GenerateCode

    }
}
