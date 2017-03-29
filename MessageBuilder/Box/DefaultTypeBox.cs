using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    static class DefaultTypeBox
    {
        public static HashSet<Type> defaultTypes = new HashSet<Type>()
        {
            typeof(bool), typeof(byte), typeof(char),
            typeof(int), typeof(Int16), typeof(Int32), typeof(Int64),
            typeof(uint), typeof(UInt16), typeof(UInt32), typeof(UInt64),
            typeof(float), typeof(double), 
            typeof(string)
        };
    }
}
