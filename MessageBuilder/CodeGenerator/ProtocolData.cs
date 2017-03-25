using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    class ProtocolDirectionData
    {
        public Direction direction = Direction.Command;
        int lastNumber = 0;
        public Dictionary<int, Type> messageTypes = new Dictionary<int, Type>();
        //public Dictionary<Type, int> messageIds = new Dictionary<Type, int>();

        public void add(Type msgT)
        {
            int messageId = lastNumber++;
            //messageIds.Add(msgT, messageId);
            messageTypes.Add(messageId, msgT);
        }
    }

    class ProtocolData
    {
        public string name = "";
        public Dictionary<Direction, ProtocolDirectionData> directionDataMap = new Dictionary<Direction, ProtocolDirectionData>();
    }
}
