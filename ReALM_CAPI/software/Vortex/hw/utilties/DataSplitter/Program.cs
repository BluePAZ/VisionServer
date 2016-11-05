using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataSplitter
{
    class Program
    {
        static void Main(string[] args)
        {
            StreamReader sr = new StreamReader(@"G:\Research\ReALM_CAPI\hardware\Projects\SURF_CAPI\simulation\input_image_can_192_252.dat");
            StreamWriter sw = new StreamWriter(@"G:\Research\ReALM_CAPI\hardware\Projects\SURF_CAPI\simulation\input_image_can_192_252_single_line.dat");
            while (!sr.EndOfStream)
            {
                string line = sr.ReadLine();
                Stack<string> pixel_stack = new Stack<string>();

                for (int i = 0; i < 128; i+=2)
                {
                    pixel_stack.Push(line.Substring(i, 2));
                }

                while (pixel_stack.Count > 0)
                    sw.WriteLine(pixel_stack.Pop());
            }

            sw.Close();
            sr.Close();
        }
    }
}
