using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MessageBuilderLib
{
    class CodeWriter
    {
        StreamWriter sw = null;
        int indent_state = 0;

        public class Block : IDisposable
        {
            CodeWriter gen;
            string postfix;

            public Block(CodeWriter gen, string appix = "")
            {
                this.gen = gen;
                gen.prn("{");
                gen.indent_state++;
                this.postfix = appix;
            }

            public void Dispose()
            {
                gen.indent_state--;
                gen.prn($"}}{postfix}");
            }
        }

        public class Indent : IDisposable
        {
            CodeWriter gen;

            public Indent(CodeWriter gen)
            {
                this.gen = gen;
                gen.indent_state++;
            }

            public void Dispose()
            {
                gen.indent_state--;
            }
        }

        public CodeWriter(StreamWriter sw)
        {
            this.sw = sw;
        }
        
        public void prn()
        {
            prnIndent();
            sw.WriteLine();
        }

        public void prn(string content)
        {
            prnIndent();
            sw.WriteLine(content);
        }

        public Block type(string title)
        {
            prn(title);
            return new Block(this, ";");
        }

        public Block func(string title)
        {
            prn(title);
            return new Block(this);
        }
        
        public Block br()
        {
            return new Block(this);
        }

        public Indent indent()
        {
            return new Indent(this);
        }

        public void acc(string val)
        {
            indent_state--;
            prn(val);
            indent_state++;
        }

        void prnIndent()
        {
            for (int i = 0; i < indent_state; ++i)
            {
                sw.Write("\t");
            }
        }
    }
}
