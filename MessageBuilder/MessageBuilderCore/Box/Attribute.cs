using System;
using System.Collections.Generic;

namespace MessageBuilderLib
{
    /// <summary>
    /// Serializable types
    /// </summary>
    public class MessageAttribute : Attribute
    {
        public ICollection<Type> dependencies = null;

        public MessageAttribute()
        {
        }

        public MessageAttribute(params Type[] dependencies)
        {
            this.dependencies = dependencies;
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
