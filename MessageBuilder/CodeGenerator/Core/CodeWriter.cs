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
                gen.print("{");
                gen.indent_state++;
                this.postfix = appix;
            }

            public void Dispose()
            {
                gen.indent_state--;
                gen.print($"}}{postfix}");
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

        void prnIndent()
        {
            for (int i = 0; i < indent_state; ++i)
            {
                sw.Write("\t");
            }
        }
        public void print()
        {
            prnIndent();
            sw.WriteLine();
        }

        public Block type(string title)
        {
            print(title);
            return new Block(this, ";");
        }

        public void print(string content)
        {
            prnIndent();
            sw.WriteLine(content);
        }

        public Block br()
        {
            return new Block(this);
        }

        public Indent indent()
        {
            return new Indent(this);
        }
    }
}
