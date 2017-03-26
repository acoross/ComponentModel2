using System;
using System.Collections.Generic;

namespace MessageBuilderLib
{
    public enum Direction
    {
        Command,
        Noti,
    }

    /// <summary>
    /// Serializable types
    /// </summary>
    public class ProtocolAttribute : Attribute
    {
        //public string protocolName;
        public Direction direction;

        public ProtocolAttribute(/*string protocolType, */Direction direction)
        {
            //this.protocolName = protocolType;
            this.direction = direction;
        }
    }

    /// <summary>
    /// 다른 모든 MessageType 들에서 참조 가능한 serializable types
    /// </summary>
    public class SharedTypeAttribute : Attribute
    {
        //public string fileName;
        //public string namespaceName;
        public SharedTypeAttribute(/*string fileName, string namespaceName*/)
        {
            //this.fileName = fileName;
            //this.namespaceName = namespaceName;
        }
    }

    public enum RpcMethodType
    {
        Rpc,
        SCPush
    }

    public class RpcAttribute : Attribute
    {
        public string ServiceName;
        public ICollection<Type> types;

        public RpcAttribute(string serviceName, params Type[] types)
        {
            this.ServiceName = serviceName;
            this.types = types;
        }
    }
}
