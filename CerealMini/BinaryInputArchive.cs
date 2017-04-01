﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using MessageBuilderLib;

namespace CerealMini
{
    public class BinaryInputArchive : IDisposable
    {
        MemoryStream ms;
        BinaryReader br;

        public BinaryInputArchive(MemoryStream ms)
        {
            this.ms = ms;
            br = new BinaryReader(ms);
        }

        public void Dispose()
        {
            if (br != null)
            {
                br.Dispose();
                br = null;
                ms = null;
            }
        }

        object ReadNumeric(Type t)
        {
            switch (Type.GetTypeCode(t))
            {
                case TypeCode.Byte:
                    return br.ReadByte();
                case TypeCode.SByte:
                    return br.ReadSByte();
                case TypeCode.UInt16:
                    return br.ReadUInt16();
                case TypeCode.UInt32:
                    return br.ReadUInt32();
                case TypeCode.UInt64:
                    return br.ReadUInt64();
                case TypeCode.Int16:
                    return br.ReadUInt16();
                case TypeCode.Int32:
                    return br.ReadInt32();
                case TypeCode.Int64:
                    return br.ReadInt64();
                case TypeCode.Decimal:
                    return br.ReadDecimal();
                case TypeCode.Double:
                    return br.ReadDouble();
                case TypeCode.Single:
                    return br.ReadSingle();
                default:
                    throw new Exception();
            }
        }

        public T Read<T>()
            where T : new()
        {
            var msgT = typeof(T);
            return (T)Read(msgT);
        }

        object Read(Type msgT)
        {
            var attr = msgT.ReflectedType?.GetCustomAttributes(typeof(MessageAttribute), false);
            if (attr == null)
            {
                throw new Exception("invalid type");
            }

            var v = Activator.CreateInstance(msgT);

            foreach (var f in msgT.GetFields())
            {
                if (f.FieldType.IsNumeric())
                {
                    var val = ReadNumeric(f.FieldType);
                    f.SetValue(v, val);
                }
                else
                {
                    var val = Read(f.FieldType);
                    f.SetValue(v, val);
                }
            }

            return v;
        }
    }
}
