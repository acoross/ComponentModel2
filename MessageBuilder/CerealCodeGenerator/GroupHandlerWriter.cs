using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    public class GroupHandlerWriter
    {
        string getHandlerFileName(string groupName)
        {
            return groupName + ".stub.h";
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
                var filepath = dirFullPath + "/" + getHandlerFileName(group.Name);
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
                        c.prn($"#include \"{getHandlerFileName(dep.Name)}\"");
                    }
                    c.prn();

                    // include this group message.h
                    c.prn($"#include \"{GroupWriter.getFileName(group.Name)}\"");
                    c.prn();

                    // file
                    using (c.func($"namespace {group.Name}"))
                    {
                        using (c.type("class IHandler"))
                        {
                            c.acc("public:");

                            // virtual destructor
                            c.prn($"virtual ~IHandler() {{}}");

                            // messages
                            foreach (var msgT in group.messageMap.Values)
                            {
                                c.prn($"virtual void Handle(const {msgT.Name}& msg) {{}}");
                            }
                        }
                    }
                }

                Console.WriteLine($"{group.Name} generated ({filepath})");
            }
        } //GenerateCode
    }
}
