﻿using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    public class GroupWriter
    {
        public static string getFileName(string groupName)
        {
            return groupName + ".message.h";
        }
        
        public void GenerateCode(string dirpath, string includeFile = null)
        {

            string dirFullPath = Path.GetFullPath(dirpath);

            if (!Directory.Exists(dirFullPath))
            {
                Directory.CreateDirectory(dirFullPath);
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
                var filepath = dirFullPath + "/" + getFileName(group.Name);
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
                        var uint32 = TypeTranslator.getTypeName(typeof(UInt32));
                        // PacketType
                        using (c.type($"enum class PacketType : {uint32}"))
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

                Console.WriteLine($"{group.Name} generated ({filepath})");
            }
        } //GenerateCode

    }
}
