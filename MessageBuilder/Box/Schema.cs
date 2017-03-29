using System;
using System.Collections.Generic;
using System.Reflection;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    class GroupSchema
    {
        public readonly string Name = "";
        public readonly ICollection<Type> dependencies_type;
        public readonly List<GroupSchema> dependencies = new List<GroupSchema>();

        public Dictionary<int, MessageSchema> messageMap = new Dictionary<int, MessageSchema>();
        public HashSet<EnumSchema> enumTypes = new HashSet<EnumSchema>();
        public HashSet<Type> havingTypes = new HashSet<Type>();

        int lastNumber = 0;

        public GroupSchema(Type type, MessageAttribute messageAttr)
        {
            Name = type.Name;
            dependencies_type = messageAttr.dependencies;

            foreach (var msgType in type.GetNestedTypes())
            {
                if (msgType.IsEnum)
                {
                    addEnumType(msgType);
                }
                else
                {
                    addMessageType(msgType);
                }
                havingTypes.Add(msgType);
            }
        }

        void addMessageType(Type msgT)
        {
            int messageId = lastNumber++;
            messageMap.Add(messageId, new MessageSchema(messageId, msgT));
        }

        void addEnumType(Type eT)
        {
            enumTypes.Add(new EnumSchema(eT));
        }
    }
    
    class EnumSchema
    {
        public readonly string Name;
        public readonly List<string> EnumNames = new List<string>();
        public readonly Type underlying;

        public EnumSchema(Type e)
        {
            if (!e.IsEnum)
                throw new Exception("e is not enum type");

            Name = e.Name;
            underlying = e.GetEnumUnderlyingType();

            foreach (var eName in e.GetEnumNames())
            {
                EnumNames.Add(eName);
            }
        }
    }

    class MemberSchema
    {
        public Type MemberType;
        public string MemberName;

        public MemberSchema(FieldInfo f)
        {
            MemberType = f.FieldType;
            MemberName = f.Name;
        }
    }

    class MessageSchema
    {
        public readonly Int32 Id;
        public readonly string Name;
        public readonly List<MemberSchema> Members = new List<MemberSchema>();

        public MessageSchema(Int32 msgId, Type t)
        {
            Id = msgId;

            if (t.IsEnum)
                throw new Exception("t is enum type");

            Name = t.Name;
            foreach (var field in t.GetFields())
            {
                Members.Add(new MemberSchema(field));
            }
        }
    }
}
