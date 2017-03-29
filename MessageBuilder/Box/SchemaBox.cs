using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    static class SchemaBox
    {
        static Dictionary<string, GroupSchema> groupMap = new Dictionary<string, GroupSchema>();

        public static void Parse()
        {
            foreach (var assem in AppDomain.CurrentDomain.GetAssemblies())
            {
                foreach (var type in assem.GetTypes())
                {
                    var attr = type.GetCustomAttributes(typeof(MessageAttribute), false);
                    if (attr != null && attr.Length == 1)
                    {
                        var msgAttribute = attr[0] as MessageAttribute;
                        addSchema(type, msgAttribute);
                    }
                }
            }

            resolveInterGroupDependencies();
            resolveMessageTypeDependencies();
        }

        public static IEnumerable<GroupSchema> GetSchemas()
        {
            return groupMap.Values;
        }

        static void addSchema(Type GroupType, MessageAttribute messageAttr)
        {
            if (groupMap.ContainsKey(GroupType.Name))
            {
                throw new Exception($"Group name {GroupType.Name} is duplicated.");
            }

            GroupSchema group = new GroupSchema(GroupType, messageAttr);
            groupMap.Add(group.Name, group);
        }

        static void resolveInterGroupDependencies()
        {
            // check invalid dependencies
            foreach (var group in groupMap.Values)
            {
                if (group.dependencies_type != null)
                {
                    foreach (var depType in group.dependencies_type)
                    {
                        if (!groupMap.ContainsKey(depType.Name))
                        {
                            throw new Exception($"{group.Name} has invalid dependency to {depType}");
                        }
                    }
                }
            }

            // check circular dependencies
            Dictionary<string, GroupSchema> resolved = new Dictionary<string, GroupSchema>();
            Queue<GroupSchema> notresolved = new Queue<GroupSchema>();
            Queue<GroupSchema> notyet = new Queue<GroupSchema>();

            foreach (var group in groupMap.Values)
            {
                notresolved.Enqueue(group);
            }

            for (;;)
            {
                int resolvCount = 0;
                while (notresolved.Count > 0)
                {
                    var group = notresolved.Dequeue();
                    if (group.dependencies_type == null)
                    {
                        resolved.Add(group.Name, group);
                        resolvCount++;
                        continue;
                    }

                    bool success = true;
                    foreach (var depType in group.dependencies_type)
                    {
                        if (!resolved.ContainsKey(depType.Name))
                        {   
                            success = false;
                            break;
                        }
                    }
                    
                    if (success)
                    {
                        foreach (var depType in group.dependencies_type)
                        {
                            var depSchema = resolved[depType.Name];
                            group.dependencies.Add(depSchema);
                        }

                        resolved.Add(group.Name, group);
                        resolvCount++;
                    }
                    else
                    {
                        notyet.Enqueue(group);
                    }
                }
                if (notyet.Count == 0)
                {
                    return;
                }
                if (notyet.Count > 0 && resolvCount == 0)
                {
                    throw new Exception("circular dependency may exist!!");
                }

                var tmp = notyet;
                notyet = notresolved; // empty
                notresolved = tmp;
            }
        } //resolveInterGroupDependencies()

        static void resolveMessageTypeDependencies()
        {
            foreach (var group in groupMap.Values)
            {
                foreach (var msg in group.messageMap.Values)
                {
                    foreach (var mem in msg.Members)
                    {
                        bool resolved = false;

                        if (DefaultTypeBox.defaultTypes.Contains(mem.MemberType))
                        {
                            resolved = true;
                        }
                        else if (group.havingTypes.Contains(mem.MemberType))
                        {
                            resolved = true;
                        }
                        else
                        {
                            foreach (var depSchema in group.dependencies)
                            {
                                if (depSchema.havingTypes.Contains(mem.MemberType))
                                {
                                    resolved = true;
                                    break;
                                }
                            }
                        }

                        if (!resolved)
                        {
                            throw new Exception($"{msg} has invalid member({mem.MemberType} {mem.MemberName})");
                        }
                    } // foreach msg.Members
                }
            }
        } //resolveMessageTypeDependencies
    }
}
