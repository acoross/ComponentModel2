using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CerealMini
{
    public static class TypeChecker
    {
        public static bool IsNumeric(this Type type)
        {
            var d = Type.GetTypeCode(type);
            switch (d)
            {
                case TypeCode.Byte:
                case TypeCode.SByte:
                case TypeCode.UInt16:
                case TypeCode.UInt32:
                case TypeCode.UInt64:
                case TypeCode.Int16:
                case TypeCode.Int32:
                case TypeCode.Int64:
                case TypeCode.Decimal:
                case TypeCode.Double:
                case TypeCode.Single:
                    return true;
                default:
                    return false;
            }
        }
    }
}
