using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    static class MessageWriter
    {
        static string stringBuildParameterList(Type t)
        {
            var sb = new StringBuilder();
            bool first = true;
            foreach (var field in t.GetFields())
            {
                string typeName = RegisteredTypeBox.getTypeName(field.FieldType);
                if (typeName != null)
                {
                    if (first)
                    {
                        first = false;
                    }
                    else
                    {
                        sb.Append(", ");
                    }
                    sb.Append($"{typeName} {field.Name}");
                }
            }
            return sb.ToString();
        }

        static string stringBuildArgumentList(Type t)
        {
            var sb = new StringBuilder();
            bool first = true;
            foreach (var field in t.GetFields())
            {
                string typeName = RegisteredTypeBox.getTypeName(field.FieldType);
                if (typeName != null)
                {
                    if (first)
                    {
                        first = false;
                    }
                    else
                    {
                        sb.Append(", ");
                    }
                    sb.Append(field.Name);
                }
            }
            return sb.ToString();
        }

        public static void PrintEnumType(CodeWriter gen, Type enumT)
        {
            if (!enumT.IsEnum)
            {
                throw new Exception();
            }

            var underlying = RegisteredTypeBox.getTypeName(enumT.GetEnumUnderlyingType());
            if (underlying != null)
            {
                using (gen.type($"enum class {enumT.Name} : {underlying}"))
                {
                    foreach (var enumName in enumT.GetEnumNames())
                    {
                        gen.print($"{enumName}, ");
                    }
                }
            }
        } //PrintEnumType

        public static void PrintMessage(CodeWriter gen, Type t, int id = -1)
        {
            var paramList = stringBuildParameterList(t);

            using (gen.type($"struct {t.Name}"))
            {
                if (id >= 0)
                {
                    gen.print($"const static uint32 PacketId = (uint32)PacketType::{t.Name};");
                    gen.print();
                }

                // null constructor
                gen.print($"{t.Name}()");
                using (gen.br())
                { }
                gen.print();

                // valued constructor
                gen.print($"{t.Name}({paramList})");
                using (gen.indent())
                {
                    bool first = true;
                    foreach (var field in t.GetFields())
                    {
                        string typeName = RegisteredTypeBox.getTypeName(field.FieldType);
                        if (typeName != null)
                        {
                            if (first)
                            {
                                first = false;
                                gen.print($": {field.Name}({field.Name})");
                            }
                            else
                            {
                                gen.print($", {field.Name}({field.Name})");
                            }
                        }
                    }
                }
                using (gen.br())
                {}
                gen.print();
                
                // fields
                foreach (var field in t.GetFields())
                {
                    string typeName = RegisteredTypeBox.getTypeName(field.FieldType);
                    if (typeName != null)
                    {
                        gen.print($"{typeName} {field.Name};");
                    }
                    else
                    {
                        throw new Exception();
                    }
                }
                gen.print();

                // cerealize
                gen.print("template <class Ar>");
                gen.print("void serialize(Ar& ar)");
                using (gen.br())
                {
                    var args = stringBuildArgumentList(t);
                    if (args != "")
                    {
                        gen.print($"ar({args});");
                    }
                }
            }
        } //PrintMessage
    }
}
