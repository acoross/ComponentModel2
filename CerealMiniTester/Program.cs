using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ConsoleApplication1
{
    class Program
    {
        static void printBytes(byte[] buf, int len)
        {
            for (int i = 0; i < len; ++i)
            {
                Console.Write("{0,4} ", buf[i]);
                if (i % 10 == 9)
                {
                    Console.Write('\n');
                }
            }
            Console.Write('\n');

        }

        static void Main(string[] args)
        {
            using (var ms = new MemoryStream())
            using (var ar = new CerealMini.BinaryOutputArchive(ms))
            {
                ar.Write('c');
                ar.Write((short)50);
                ar.Write((Int64)100);
                ar.Write("shin 일환");
                ar.Write((char)1);
                List<int> l = new List<int>
                {
                    1,2,3,4,5
                };
                ar.Write(l);
                ar.WriteVal(123);
            }

            byte[] buf = new byte[1024];
            long len = 0;
            using (var ms = new MemoryStream(buf))
            using (var ar = new CerealMini.BinaryOutputArchive(ms))
            {
                var msg = new NetworkMessage.SCProtocol.NotiEnterZone
                {
                    gameObjectId = 1,
                    position = new NetworkMessage.Shared.NetVector
                    { X = 3, Y = 4, Z = 5 },
                    velocity = new NetworkMessage.Shared.NetVector
                    { X = -1, Y = 0, Z = 1 }
                };

                ar.Write(msg);
                len = ms.Position;
            }

            printBytes(buf, (int)len);

            using (var ms = new MemoryStream(buf))
            using (var ar = new CerealMini.BinaryInputArchive(ms))
            {
                var msg = ar.Read<NetworkMessage.SCProtocol.NotiEnterZone>();
                Console.WriteLine($"{msg.gameObjectId}, {msg.position}, {msg.velocity}");
            }
        }
    }
}
