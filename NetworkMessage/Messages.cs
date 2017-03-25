using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MessageBuilderLib;

namespace NetworkMessage
{
    using static Shared;

    [SharedType("shared", "shared")]
    class Shared
    {
        public struct FVector
        {
            public float X;
            public float Y;
            public float Z;
        }

        public enum ESex : Int32
        {
            Male,
            Female,
            Max
        }
    }

    namespace CSProtocol
    {
        [Protocol("CSProtocol", Direction.Command)]
        struct Command
        {
            public struct RequestLogin
            {
                public String id;
                public String password;
            }

            public struct Move
            {
                public FVector position;
                public FVector velocity;
            }
        }

        [Protocol("CSProtocol", Direction.Noti)]
        struct Noti
        {
            public struct ResponseLogin
            {
                public bool ok;
                public UInt64 gameObjectId;
            }

            public struct NotiMove
            {
                public UInt64 gameObjectId;
                public FVector position;
                FVector velocity;
            }

            public struct NotiEnterZone
            {
                public UInt64 gameObjectId;
                public FVector position;
                public FVector velocity;
            }

            public struct NotiLeaveZone
            {
                public UInt64 gameObjectId;
            }
        }
    }
}
