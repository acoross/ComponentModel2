using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    static class TypeTranslator
    {
        static Dictionary<Type, string> registeredTypes = new Dictionary<Type, string>
        {
            {typeof(Boolean), "bool" },
            {typeof(UInt16), "uint16_t" },
            {typeof(UInt32), "uint32_t" },
            {typeof(UInt64), "uint64_t" },
            {typeof(Int16), "int16_t" },
            {typeof(Int32), "int32_t" },
            {typeof(Int64), "int64_t" },
            {typeof(Char), "char" },
            {typeof(Byte), "char" },
            {typeof(float), "float" },
            {typeof(double), "double" },
            {typeof(string), "std::wstring" },
        };

        public static void registerType(Type t, string name)
        {
            registeredTypes.Add(t, name);
        }

        public static string getTypeName(Type t)
        {
            string ret;
            if (registeredTypes.TryGetValue(t, out ret))
            {
                return ret;
            }

            return t.Name;
        }
    }
}
