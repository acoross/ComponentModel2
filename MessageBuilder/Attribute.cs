using System;

namespace MessageBuilderLib
{
    public enum Direction
    {
        Command,
        Noti,
    }

    public class ProtocolAttribute : Attribute
    {
        public string protocolName;
        public Direction direction;

        public ProtocolAttribute(string protocolType, Direction direction)
        {
            this.protocolName = protocolType;
            this.direction = direction;
        }
    }

    public class SharedTypeAttribute : Attribute
    {
        public string fileName;
        public string namespaceName;
        public SharedTypeAttribute(string fileName, string namespaceName)
        {
            this.fileName = fileName;
            this.namespaceName = namespaceName;
        }
    }
}
