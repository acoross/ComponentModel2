using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MessageBuilderLib;

namespace NetworkMessage
{
    using static Shared;

    [SharedType]
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

    [Protocol(Direction.Command)]
    struct CSProtocol
    {
        public enum TestEnum : Int32
        {
            Foo,
            Bar,
            Max
        }

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

    [Protocol(Direction.Noti)]
    struct SCProtocol
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

    [Rpc("CSService", typeof(CSProtocol), typeof(SCProtocol))]
    interface CSServiceRpc
    {
        // client to server (and server to client response)
        SCProtocol.ResponseLogin Process(CSProtocol.RequestLogin msg);
        void Process(CSProtocol.Move msg);

        // server to client noti
        event Action<SCProtocol.NotiEnterZone> OnEnterZone;
        event Action<SCProtocol.NotiLeaveZone> OnLeaveZone;
        event Action<SCProtocol.NotiMove> OnMove;
    }
}
