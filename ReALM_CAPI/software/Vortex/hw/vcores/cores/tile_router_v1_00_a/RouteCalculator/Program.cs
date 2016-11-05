using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RouteCalculator
{
    class Program
    {
        static void Main(string[] args)
        {
            while (true)
            {
                string route = Console.ReadLine();
                string[] routeParts = route.Split(' ');
                if (routeParts.Length < 2 || routeParts.Length > 2)
                {
                    Console.WriteLine("Invalid Route Specification");
                    continue;
                }

                string[] SrcPart = routeParts[0].Split(',');
                if (SrcPart.Length < 3 || SrcPart.Length > 3)
                {
                    Console.WriteLine("Invalid Source Specification");
                    continue;
                }

                int src_Y = Convert.ToInt32(SrcPart[0]);
                int src_X = Convert.ToInt32(SrcPart[1]);
                int src_P = Convert.ToInt32(SrcPart[2]);

                string[] DstPart = routeParts[1].Split(',');
                if (DstPart.Length < 3 || DstPart.Length > 3)
                {
                    Console.WriteLine("Invalid Target Specification");
                    continue;
                }

                int dst_Y = Convert.ToInt32(DstPart[0]);
                int dst_X = Convert.ToInt32(DstPart[1]);
                int dst_P = Convert.ToInt32(DstPart[2]);

                int current_X = src_X;
                int current_Y = src_Y;

                byte routedReg = 1;
                switch (src_P)
                {
                    case 0 :
                        if (current_X == dst_X && current_Y == dst_Y)
                            routedReg = (byte)(routedReg << src_P);
                        else if (current_X == dst_X && current_Y != dst_Y)
                            routedReg = (byte)(routedReg << 6);
                        else if (current_X != dst_X && current_Y == dst_Y)
                            routedReg = (byte)(routedReg << 5);
                        else
                            routedReg = (byte)(routedReg << 5);
                        break;
                    case 1:
                        if (current_X == dst_X && current_Y == dst_Y)
                            routedReg = (byte)(routedReg << src_P);
                        else if (current_X == dst_X && current_Y != dst_Y)
                            routedReg = (byte)(routedReg << 7);
                        else if (current_X != dst_X && current_Y == dst_Y)
                            routedReg = (byte)(routedReg << 4);
                        else
                            routedReg = (byte)(routedReg << 4);
                        break;
                    case 2:
                        if (current_Y == dst_Y && current_X == dst_X)
                            routedReg = (byte)(routedReg << src_P);
                        else if (current_Y == dst_Y && current_X != dst_X)
                            routedReg = (byte)(routedReg << 4);
                        else if (current_Y != dst_Y && current_X == dst_X)
                            routedReg = (byte)(routedReg << 7);
                        else
                            routedReg = (byte)(routedReg << 7);
                        break;
                    case 3:
                        if (current_Y == dst_Y && current_X == dst_X)
                            routedReg = (byte)(routedReg << src_P);
                        else if (current_Y == dst_Y && current_X != dst_X)
                            routedReg = (byte)(routedReg << 5);
                        else if (current_Y != dst_Y && current_X == dst_X)
                            routedReg = (byte)(routedReg << 6);
                        else
                            routedReg = (byte)(routedReg << 6);
                        break;
                }

                Console.WriteLine("{0} {1} -> {2}", routeParts[0], routeParts[1], Convert.ToString(routedReg, 2).PadLeft(8, '0'));
            }

        }
    }
}
