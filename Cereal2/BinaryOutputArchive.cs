using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using MessageBuilderLib;

namespace CerealMini
{
    public class BinaryOutputArchive : IDisposable
    {
        MemoryStream stream;
        BinaryWriter bw;

        public BinaryOutputArchive(MemoryStream stream)
        {
            this.stream = stream;
            bw = new BinaryWriter(stream);
        }

        public void Dispose()
        {
            if (bw != null)
            {
                ((IDisposable)bw).Dispose();
                bw = null;
                stream = null;
            }
        }

        public void Write(char v)
        {
            bw.Write(v);
        }

        public void Write(short v)
        {
            bw.Write(v);
        }

        public void Write(ushort v)
        {
            bw.Write(v);
        }

        public void Write(Int32 v)
        {
            bw.Write(v);
        }

        public void Write(UInt32 v)
        {
            bw.Write(v);
        }

        public void Write(Int64 v)
        {
            bw.Write(v);
        }

        public void Write(UInt64 v)
        {
            bw.Write(v);
        }
        
        public void Write(float v)
        {
            bw.Write(v);
        }

        public void Write(double v)
        {
            bw.Write(v);
        }

        public void Write(string v)
        {
            byte[] bytes = Encoding.Unicode.GetBytes(v);
            bw.Write((Int64)bytes.Length);
            stream.Write(bytes, 0, bytes.Length);
        }
        
        public void Write(List<int> l)
        {
            bw.Write((Int64)l.Count);

            int[] array = l.ToArray();
            byte[] bytes = new byte[array.Length * sizeof(int)];
            Buffer.BlockCopy(array, 0, bytes, 0, bytes.Length);
            stream.Write(bytes, 0, bytes.Length);
        }
        
        public void WriteVal(object v)
        {
            //dynamic vv = v;
            //Write(vv);

            foreach (var method in this.GetType().GetMethods())
            {
                if (method.Name == "Write" )
                {
                    var ps = method.GetParameters();
                    if (ps.Length == 1)
                    {
                        if (ps[0].ParameterType == v.GetType())
                        {
                            method.Invoke(this, new object[] { v });
                            return;
                        }
                    }
                }
            }

            throw new Exception();
        }

        public void Write<T>(T v)
        {
            var attr = typeof(T).ReflectedType?.GetCustomAttributes(typeof(MessageAttribute), false);
            if (attr == null)
            {
                throw new Exception();
            }

            foreach (var f in typeof(T).GetFields())
            {
                if (f.FieldType.IsNumeric())
                {
                    WriteVal(f.GetValue(v));
                }
                else
                {
                    //dynamic fv = f.GetValue(v);
                    //Write(fv);

                    bool invoked = false;
                    object fv = f.GetValue(v);
                    foreach (var method in this.GetType().GetMethods())
                    {
                        if (method.Name == "Write" && method.IsGenericMethod)
                        {
                            var generic = method.MakeGenericMethod(fv.GetType());
                            generic.Invoke(this, new object[] { fv });
                            invoked = true;
                            break;
                        }
                    }

                    if (!invoked) throw new Exception();
                }
            }
        }
    }
}
