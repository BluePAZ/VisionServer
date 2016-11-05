using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace module_rename
{
    class Program
    {
        static void Main(string[] args)
        {
            
            string SearchRoot = args[0].Replace("\"","");
            string prefixName = args[1];
            string[] Files = Directory.GetFiles(SearchRoot, "*.*", SearchOption.AllDirectories).Where(x => x.EndsWith(".v")).ToArray();

            List<string> module_names = new List<string>();

            foreach (string filename in Files)
            {
                FileInfo fi = new FileInfo(filename);
                
                module_names.Add(fi.Name.Replace(".v", ""));
            }

            foreach (string filename in Files)
            {
                StreamReader originalFile = new StreamReader(filename);
                string originalFileContents = originalFile.ReadToEnd();
                
                foreach (string originalModuleName in module_names)
                {
                    string newModuleName = prefixName + "_" + originalModuleName;
                    originalFileContents = originalFileContents.Replace(originalModuleName, newModuleName);
                }

                FileInfo fi = new FileInfo(filename);
                string newFileName = fi.DirectoryName + @"\updated\" + prefixName + "_" + fi.Name;

                if (!Directory.Exists(fi.DirectoryName + @"\updated\"))
                    Directory.CreateDirectory(fi.DirectoryName + @"\updated\");

                StreamWriter newFile = new StreamWriter(newFileName);
                newFile.Write(originalFileContents);

                newFile.Close();
                originalFile.Close();
                
            }
        }

    }
}
