using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    class SharedNamespace
    {
        public string namespaceName;
        public Type type;
    }

    class SharedFile
    {
        public string filename;
        public HashSet<SharedNamespace> namespaces = new HashSet<SharedNamespace>();
    }

    static class SharedTypeBox
    {
        static Dictionary<string, SharedFile> sharedFiles = new Dictionary<string, SharedFile>();

        static public void Parse()
        {
            foreach (var assem in AppDomain.CurrentDomain.GetAssemblies())
            {
                foreach (var type in assem.GetTypes())
                {
                    var shareAttrs = type.GetCustomAttributes(typeof(SharedTypeAttribute), false);
                    if (shareAttrs != null && shareAttrs.Length > 0)
                    {
                        var shareAttr = shareAttrs[0] as SharedTypeAttribute;
                        setSharedInfo(type, shareAttr);
                    }
                }
            }
        }

        static void setSharedInfo(Type shared, SharedTypeAttribute shareAttr)
        {
            var filename = shareAttr.fileName;
            SharedFile shareFile;
            if (!sharedFiles.TryGetValue(filename, out shareFile))
            {
                shareFile = new SharedFile();
                shareFile.filename = filename;
                sharedFiles[filename] = shareFile;
            }

            shareFile.namespaces.Add(new SharedNamespace { namespaceName = shareAttr.namespaceName, type = shared });
            foreach (var msgT in shared.GetNestedTypes())
            {
                RegisteredTypeBox.registerType(msgT, $"{shareAttr.namespaceName}::{msgT.Name}");
            }
        }

        public static IEnumerable<SharedFile> GetSharedFiles()
        {
            return sharedFiles.Values;
        }
    }
}
