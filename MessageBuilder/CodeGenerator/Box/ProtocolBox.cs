using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    static class ProtocolBox
    {
        static Dictionary<string, ProtocolData> protocolMap = new Dictionary<string, ProtocolData>();

        public static void Parse()
        {
            foreach (var assem in AppDomain.CurrentDomain.GetAssemblies())
            {
                foreach (var type in assem.GetTypes())
                {
                    var attr = type.GetCustomAttributes(typeof(ProtocolAttribute), false);
                    if (attr != null && attr.Length == 1)
                    {
                        var protocolAttribute = attr[0] as ProtocolAttribute;
                        setProtocolData(type, protocolAttribute);
                    }
                }
            }
        }

        public static IEnumerable<ProtocolData> GetProtocols()
        {
            return protocolMap.Values;
        }

        static void setProtocolData(Type protocol, ProtocolAttribute protocolAttr)
        {
            //var protocolName = protocolAttr.protocolName;
            var protocolName = protocol.Name;
            var direction = protocolAttr.direction;

            ProtocolData protocolData = null;
            if (!protocolMap.TryGetValue(protocolName, out protocolData))
            {
                protocolData = new ProtocolData();
                protocolData.name = protocolName;
                protocolMap[protocolName] = protocolData;
            }

            ProtocolDirectionData dirData = null;
            if (!protocolData.directionDataMap.TryGetValue(direction, out dirData))
            {
                dirData = new ProtocolDirectionData();
                dirData.direction = direction;
                protocolData.directionDataMap[direction] = dirData;
            }

            foreach (var msgType in protocol.GetNestedTypes())
            {
                dirData.add(msgType);
            }
        }
    }
}
