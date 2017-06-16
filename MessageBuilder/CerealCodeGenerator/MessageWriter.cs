using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    class EnumWriter
    {
        readonly CodeWriter c;
        readonly EnumSchema enumSchema;

        public EnumWriter(CodeWriter c, EnumSchema enumSchema)
        {
            this.c = c;
            this.enumSchema = enumSchema;
        }

        public void PrintEnumType()
        {
            string title = $"enum class {enumSchema.Name}";

            var underlying = TypeTranslator.getTypeName(enumSchema.underlying);
            if (underlying != null)
            {
                title += $" : {underlying}";
            }

            using (c.type(title))
            {
                foreach (var enumName in enumSchema.EnumNames)
                {
                    c.prn($"{enumName}, ");
                }
            }
        } //PrintEnumType
    }

    class MessageWriter
    {
        readonly CodeWriter c;
        readonly GroupSchema group;
        readonly MessageSchema msg;

        public MessageWriter(CodeWriter c, GroupSchema group, MessageSchema msg)
        {
            this.c = c;
            this.group = group;
            this.msg = msg;
        }

        string tName(Type t)
        {
            var tname = TypeTranslator.getTypeName(t);

            if (!group.havingTypes.Contains(t))
            {
                foreach (var dep in group.dependencies)
                {
                    if (dep.havingTypes.Contains(t))
                    {
                        return $"{dep.Name}::{tname}";
                    }
                }
            }

            return tname;
        }

        string stringBuildParameterList()
        {
            var sb = new StringBuilder();
            bool first = true;
            foreach (var m in msg.Members)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    sb.Append(", ");
                }
                sb.Append($"{tName(m.MemberType)} {In(m.MemberName)}");
            }
            return sb.ToString();
        }

        string stringBuildArgumentList()
        {
            var sb = new StringBuilder();
            bool first = true;
            foreach (var m in msg.Members)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    sb.Append(", ");
                }
                sb.Append(m.MemberName);
            }
            return sb.ToString();
        }

        static string In(string name)
        {
            return "in" + name;
        }

        void prnConstructors()
        {
            // null constructor
            c.prn($"{msg.Name}()");
            using (c.br())
            { }
            c.prn();

            // valued constructor
            var paramList = stringBuildParameterList();
            c.prn($"{msg.Name}({paramList})");
            using (c.indent())
            {
                bool first = true;
                foreach (var m in msg.Members)
                {
                    string typeName = TypeTranslator.getTypeName(m.MemberType);
                    if (first)
                    {
                        first = false;
                        c.prn($": {m.MemberName}({In(m.MemberName)})");
                    }
                    else
                    {
                        c.prn($", {m.MemberName}({In(m.MemberName)})");
                    }
                }
            }
            using (c.br())
            { }
        }

        void prnMembers()
        {
            foreach (var m in msg.Members)
            {
                c.prn($"{tName(m.MemberType)} {m.MemberName};");
            }
        }

        void prnCereal()
        {
            c.prn("template <class Ar>");
            c.prn("void serialize(Ar& ar)");
            using (c.br())
            {
                var args = stringBuildArgumentList();
                if (args != "")
                {
                    c.prn($"ar({args});");
                }
            }
        }

        public void PrintMessage()
        {
            using (c.type($"struct {msg.Name}"))
            {
                if (msg.Id >= 0)
                {
                    var uint32 = TypeTranslator.getTypeName(typeof(UInt32));

                    c.prn($"const static {uint32} PacketId = ({uint32})PacketType::{msg.Name};");
                    c.prn();
                }

                prnConstructors();
                c.prn();

                prnMembers();
                c.prn();

                prnCereal();
            }
        } //PrintMessage
    }
}
